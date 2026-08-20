# ENARM Simulator Pro - API Reference

## Módulos

| Módulo | Librería | Descripción |
|--------|----------|-------------|
| ENARM.Common | `ENARM.Common.lib` | Logger, Result, UUID, Config, SpecialtyMapper |
| ENARM.Domain | `ENARM.Domain.lib` | ClinicalCase, Patient, Symptom, VitalSigns |
| ENARM.Physiology | `ENARM.Physiology.lib` | Hemodinámica, respiratorio, metabólico, ECG |
| ENARM.Pharmacology | `ENARM.Pharmacology.lib` | DrugDatabase, DrugAdministration, PK |
| ENARM.Simulation | `ENARM.Simulation.lib` | PatientSimulator, escenarios, códigos, quirófano |
| ENARM.Data | `ENARM.Data.lib` | DatabaseManager, repositorios, MCQEngine |
| ENARM.AI | `ENARM.AI.lib` | OllamaClient, LLMRouter, PatientAIController, PubMed, RAG |
| ENARM.Assessment | `ENARM.Assessment.lib` | AssessmentEngine, rúbricas, adherencia GPC |
| ENARM.Learning | `ENARM.Learning.lib` | SpacedRepetition, WeaknessDetector |
| ENARM.MCQ | `ENARM.MCQ.lib` | Sesiones MCQ |

---

## ENARM.Physiology

### HemodynamicModel
```cpp
HemodynamicModel heart;
heart.SimulateShock(0.8);              // Shock séptico severo
heart.ApplyVasoconstrictionBaseline(2.0); // NA
heart.Update(0.01);                      // tick 10ms
double map = heart.GetMAP();            // 65-100
double hr  = heart.GetHeartRate();
double co  = heart.GetCardiacOutput();
```

### RespiratoryModel
```cpp
RespiratoryModel lungs;
lungs.SetRespiratoryRate(28);
double spo2 = lungs.GetState().SpO2_pct;
```

### MetabolicModel
```cpp
MetabolicModel meta;
meta.SimulateSepsis(0.7);
double lactate = meta.GetLactate();  // > 2.0
double ph = meta.GetPH();
```

### ECGSynthesizer
```cpp
ECGSynthesizer ecg;
ecg.SetRhythm(ECGRhythm::STEMI_Anterior);
auto samples = ecg.Generate("V2", 3.0, 250.0);  // 750 muestras
```

---

## ENARM.Pharmacology

### DrugDatabase (singleton)
```cpp
auto& db = DrugDatabase::Instance();
auto drug = db.FindByName("norepinephrine");  // optional<DrugParameters>
size_t n = db.Count();                          // 8 categorías
auto names = db.ListAllNames();
```

### DrugAdministration
```cpp
DrugAdministration drugs;
drugs.GiveBolus("norepinephrine", 0.1);
drugs.StartInfusion("propofol", 50.0);
drugs.StopInfusion("propofol");
```

---

## ENARM.Simulation

### PatientSimulator
```cpp
PatientSimulator patient(70.0);  // 70 kg
patient.ApplyScenario(ScenarioType::SepticShock);
patient.Simulate(60.0);          // 60 segundos
auto snap = patient.GetSnapshot();
// snap.MAP, snap.SpO2, snap.heartRate...
```

### EmergencyCode
```cpp
EmergencyCode code;
code.Start(EmergencyCodeType::CodeSTEMI);
code.Update(dt);
auto r = code.PerformAction("aspirina");  // CodeAction
// r.correct, r.points, r.feedback
```

### SurgicalProcedure
```cpp
SurgicalProcedure proc;
proc.Start(ProcedureType::Appendectomy);
proc.PerformAction("asepsia");
proc.NextStep();
```

### SimulationRoom (multi-usuario)
```cpp
SimulationRoom room;
room.AddMember("doc1", "Dr. Perez", RoomRole::LeadStudent);
room.StartScenario(ScenarioType::SepticShock);
room.MemberGiveBolus("doc1", "norepinephrine", 0.1);
```

---

## ENARM.Data

### DatabaseManager
```cpp
ConnectionConfig cfg;
cfg.database = "enarm_db"; cfg.user = "postgres"; cfg.password = "manager";
auto db = DatabaseManager::Create(cfg, 3).Value();  // pool de 3
```

### Repositorios
```cpp
CaseRepository cases(db);
auto all = cases.FindAll(false, 100);

MCQRepository mcqs(db);
auto q = mcqs.FindRandom(10);

GuidelineRepository gpc(db);
auto guides = gpc.FindBySpecialty("Cardiologia");
```

### MCQEngine
```cpp
MCQEngine engine(db);
auto session = engine.StartPractice("Cardiologia", "intermediate", 20);
auto result = engine.Answer(session, 'A', 12.5);
engine.Next(session);
```

---

## ENARM.AI

### OllamaClient
```cpp
OllamaConfig cfg;
cfg.endpoint = "http://localhost:11434";
OllamaClient ollama(cfg);
bool ok = ollama.IsAvailable();
```

### LLMRouter
```cpp
auto router = std::make_shared<LLMRouter>(std::make_shared<OllamaClient>(cfg));
auto res = router->Execute(TaskType::PatientResponse, messages);
```

### PatientAIController
```cpp
PatientPersonaData persona;
persona.fullName = "Maria"; persona.age = 45;
PatientAIController patient(router, persona);
auto res = patient.Ask("Como se siente?");
auto intent = patient.GetLastIntent();  // QuestionIntent
```

### PubMedClient
```cpp
PubMedClient pubmed;
auto articles = pubmed.SearchArticles("STEMI guidelines", 5).Value();
```

### RAGService
```cpp
RAGService rag(router, db);
auto hits = rag.SearchGuidelines("dolor toracico disnea", 3);
```

---

## ENARM.Assessment

### AssessmentEngine
```cpp
AssessmentEngine engine;
engine.loadRubricForCase("case_001");  // IAM
engine.onConversationTurn("Me duele el pecho", "", 10.0);
engine.onTreatmentOrdered("aspirina", "300mg", "", 12.0);
float pct = engine.getCurrentScorePercent();
auto report = engine.generateReport("s1", "st1", 300.0);
```

### GuidelineAdherenceScorer
```cpp
auto results = GuidelineAdherenceScorer::EvaluateSession(
    guidelines, consultedIds, treatments, studies, diagnosis);
double pct = GuidelineAdherenceScorer::TotalPercent(results);
```

---

## ENARM.Learning

### SpacedRepetition
```cpp
auto item = SpacedRepetition::NewItem("case_001", "Cardiologia");
SpacedRepetition sr;
sr.Review(item, 4);  // calidad 0-5
bool due = sr.IsDue(item);
```

### WeaknessDetector
```cpp
WeaknessDetector wd;
wd.RecordAttempt("Cardiologia", "Interrogatorio", false);
auto weak = wd.WeakAreas();
```

---

## ENARM.MCQ

### MCQSession
```cpp
MCQSession s;
s.mode = "practice";
s.questions = ...;
s.Answer('A');
s.Percent();
```

---

## UE5 Bridge (ENARMCoreBridge)

### UPatientAIComponent (Blueprint)
- `StartSimulation(CaseId)`
- `TickSimulation(DeltaSeconds)`
- `GetMAP()`, `GetHeartRate()`, `GetSpO2()`, `GetSystolicBP()`, `GetDiastolicBP()`
- `GiveBolus(DrugName, DoseMg)`, `StartInfusion(DrugName, Rate)`, `StopInfusion(DrugName)`
- `ApplyScenario(ScenarioName)` — "sepsis", "stemi", "normal"
- `AskPatient(Question, Callback)`

### AClinicalSceneGameMode (Blueprint)
- `InitializeScenario(ScenarioName)`
- `FindPatientAI()`
- `IsSceneReady()`
