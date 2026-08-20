ENARM Simulator Pro - API Real Verificada

Snapshot de APIs confirmadas por 86 tests unitarios pasando.
HemodynamicModel v6

Namespace: ENARM::Physiology

C++

#include "ENARM/Physiology/HemodynamicModel.h"

HemodynamicModel model;
for (int i = 0; i < 500; ++i) model.Update(DeltaT{0.01});
model.SimulateShock(0.8);
model.ApplyVasoconstrictionBaseline(2.0);
double map = model.GetMAP();  // 60-70 mmHg

Metodos: Update, SimulateShock, SimulateHypertension, ResetToNormal,
ApplyVasoconstrictionBaseline, SetHeartRate, GetMAP, GetSystolicBP,
GetDiastolicBP, GetPulsePressure, GetCardiacOutput, GetParameters.
RespiratoryModel (Otis 1950)

Namespace: ENARM::Physiology

C++

#include "ENARM/Physiology/RespiratoryModel.h"

RespiratoryModel model;
for (int i = 0; i < 500; ++i) model.Update(DeltaT{0.01});
model.SimulateARDS(0.7);
double spo2 = model.GetSpO2();  // <95%

Metodos: Update, SimulateARDS, SimulateCOPD, SimulateAsthma,
SimulatePneumonia, ResetToNormal, SetRespiratoryRate, SetTidalVolume,
SetFiO2, SetPEEP, GetSpO2, GetPaO2, GetPaCO2, GetEtCO2,
GetMinuteVent.
OxygenationModel (Kelman + Bohr)

Namespace: ENARM::Physiology

C++

#include "ENARM/Physiology/OxygenationModel.h"

double sao2 = OxygenationModel::CalculateSaO2(100.0);   // ~98%
double pao2 = OxygenationModel::CalculatePaO2FromSaO2(90.0);
double cao2 = OxygenationModel::CalculateCaO2(15.0, 98.0, 100.0);

DrugDatabase (Singleton)

Namespace: ENARM::Pharmacology

C++

#include "ENARM/Pharmacology/DrugDatabase.h"

auto& db = DrugDatabase::Instance();
auto drug = db.FindByName("norepinephrine");
size_t total = db.Count();
auto vasos = db.FindByCategory(DrugCategory::Vasopressor);

AssessmentEngine

Namespace: enarm::assessment (minusculas)

C++

#include "ENARM/Assessment/AssessmentEngine.h"

AssessmentEngine engine;
engine.loadRubric(AssessmentEngine::createIAMRubric());
engine.onDiagnosisSubmitted("IAM CEST", "ECG ST elevado", 120.0f);
float score = engine.getCurrentScorePercent();

10 rubricas: createIAMRubric, createSepsisRubric, createACVRubric,
createNeumoniaRubric, createPreeclampsiaRubric, createApendicitisRubric,
createTCERubric, createCADRubric, createLRARubric, createBronquiolitisRubric.
CompetencyEvaluator

Namespace: enarm::assessment

C++

CompetencyEvaluator ev;
bool match = ev.containsSemanticMatch("El paciente tiene IAMCEST", "IAMCEST");

ECGSynthesizer (NUEVO S2)

Namespace: ENARM::Physiology

C++

#include "ENARM/Physiology/ECGSynthesizer.h"

ECGSynthesizer synth;
auto ecg = synth.Generate(ECGRhythm::STEMI_Anterior, 95.0);
bool stemi = synth.HasSTEMICriteria(ecg);

19 ritmos: Normal, STEMI_Anterior/Inferior/Lateral, NSTEMI,
AtrialFibrillation, VentricularTachycardia, VentricularFibrillation,
AVBlock_First/Second_Mobitz1/Second_Mobitz2/Third, LBBB, RBBB,
Hyperkalemia, Hypokalemia, LongQT, SinusTachycardia, SinusBradycardia.
DiseaseProgressionEngine (NUEVO S2)

Namespace: ENARM::Physiology

C++

#include "ENARM/Physiology/DiseaseProgressionEngine.h"

DiseaseProgressionEngine engine("SepticShock");
engine.Advance(20.0);
engine.ApplyTreatment("antibiotics");
double risk = engine.GetMortalityRisk();  // 0-1

Enfermedades soportadas: SepticShock, Sepsis, IAMCEST, STEMI,
Stroke, EVC.