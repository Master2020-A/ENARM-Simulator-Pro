// =====================================================================
//   ENARM.Assessment - GuidelineAdherenceScorer (Sprint 7B Epica 3)
//   Evalua adherencia del estudiante a las GPCs consultadas en sesion.
// =====================================================================
#pragma once

#include "ENARM/Data/Repositories/GuidelineRepository.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <cctype>

namespace ENARM::Assessment {

// ============================================================
// Struct: TreatmentAction (input del scorer)
// Independiente de GUI, solo texto plano.
// ============================================================
struct TreatmentAction {
    std::string medication;
    std::string dose;
    std::string rationale;
};

// ============================================================
// Struct: GuidelineAdherenceResult (output por cada GPC)
// ============================================================
struct GuidelineAdherenceResult {
    std::string guidelineId;
    std::string guidelineTitle;
    int         totalRecommendations{0};
    int         matchedRecommendations{0};
    double      adherencePercent{0.0};      // 0-100
    int         pointsEarned{0};
    int         pointsPossible{0};
    bool        wasConsulted{false};
    int         consultationBonus{0};       // +5 por consultar
    std::vector<std::string> matchedItems;  // Detalles de aciertos
    std::vector<std::string> missedItems;   // Detalles de omisiones
};

// ============================================================
// KeywordDictionary: sinonimos de conceptos medicos
// ============================================================
class KeywordDictionary {
public:
    // Devuelve TRUE si el texto contiene cualquier sinonimo del concepto.
    // El match es case-insensitive y usa substring simple.
    static bool ContainsAnyOf(const std::string& text,
                              const std::vector<std::string>& keywords) {
        std::string lower = ToLower(text);
        for (const auto& kw : keywords) {
            std::string kwLower = ToLower(kw);
            if (lower.find(kwLower) != std::string::npos) {
                return true;
            }
        }
        return false;
    }
    
    // Concepto -> sinonimos
    static const std::unordered_map<std::string, std::vector<std::string>>& Concepts() {
        static const std::unordered_map<std::string, std::vector<std::string>> m = {
            // Antiagregantes
            {"AAS",           {"aas", "aspirina", "acido acetilsalicilico", "asa"}},
            {"P2Y12",         {"clopidogrel", "ticagrelor", "prasugrel", "plavix", "brilinta", "p2y12"}},
            // Anticoagulantes
            {"heparina",      {"heparina", "hnf", "enoxaparina", "clexane", "bivalirudina", "fondaparinux"}},
            {"warfarina",     {"warfarina", "acenocumarol", "sintrom"}},
            {"NOAC",          {"apixaban", "rivaroxaban", "dabigatran", "edoxaban", "eliquis", "xarelto"}},
            // Cardiovascular
            {"estatina",      {"estatina", "atorvastatina", "rosuvastatina", "simvastatina", "pravastatina", "lipitor"}},
            {"betabloqueador",{"betabloqueador", "metoprolol", "bisoprolol", "carvedilol", "atenolol", "propranolol"}},
            {"IECA",          {"ieca", "captopril", "enalapril", "lisinopril", "ramipril", "perindopril"}},
            {"ARA-II",        {"ara-ii", "ara ii", "losartan", "valsartan", "telmisartan", "irbesartan", "candesartan"}},
            {"calcio-ant",    {"amlodipino", "nifedipino", "diltiazem", "verapamilo", "felodipino"}},
            {"diuretico",     {"diuretico", "furosemida", "hidroclorotiazida", "espironolactona", "clortalidona"}},
            {"nitrato",       {"nitroglicerina", "isosorbide", "nitrato"}},
            // Reperfusion
            {"ICP",           {"icp", "intervencionismo coronario", "angioplastia", "cateterismo", "stent"}},
            {"fibrinolisis",  {"fibrinolisis", "trombolisis", "alteplase", "tenecteplase", "estreptoquinasa", "tpa"}},
            // Antibioticos comunes
            {"betalactamico", {"amoxicilina", "ampicilina", "penicilina", "cefalosporina", "ceftriaxona", "cefuroxima"}},
            {"macrolido",     {"azitromicina", "claritromicina", "eritromicina"}},
            {"quinolona",     {"levofloxacino", "ciprofloxacino", "moxifloxacino"}},
            // Insulina/diabetes
            {"insulina",      {"insulina", "regular", "nph", "glargina", "lispro", "aspart"}},
            // Corticoides
            {"corticoide",    {"corticoide", "prednisona", "metilprednisolona", "dexametasona", "hidrocortisona", "budesonida"}},
            // Analgesicos
            {"AINE",          {"aine", "ibuprofeno", "naproxeno", "diclofenaco", "ketorolaco", "indometacina"}},
            {"opioide",       {"morfina", "tramadol", "fentanilo", "oxicodona", "codeina"}},
            {"paracetamol",   {"paracetamol", "acetaminofen", "tempra"}},
            // Fluidos
            {"cristaloide",   {"salino", "sf 0.9", "ringer", "lactato", "hartmann", "cristaloide"}},
            // GI
            {"IBP",           {"ibp", "omeprazol", "pantoprazol", "esomeprazol", "lansoprazol"}}
        };
        return m;
    }
    
    // Dado un texto de recomendacion, extrae que conceptos aparecen
    static std::vector<std::string> ExtractConcepts(const std::string& recommendationText) {
        std::vector<std::string> found;
        std::string lower = ToLower(recommendationText);
        for (const auto& [conceptName, synonyms] : Concepts()) {
            for (const auto& syn : synonyms) {
                if (lower.find(ToLower(syn)) != std::string::npos) {
                    found.push_back(conceptName);
                    break;
                }
            }
        }
        return found;
    }
    
private:
    static std::string ToLower(const std::string& s) {
        std::string out = s;
        std::transform(out.begin(), out.end(), out.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return out;
    }
};

// ============================================================
// GuidelineAdherenceScorer: motor principal
// ============================================================
class GuidelineAdherenceScorer {
public:
    // Puntos por recomendacion segun nivel de evidencia
    static int PointsForLevel(const std::string& level) {
        if (level == "A") return 5;
        if (level == "B") return 3;
        if (level == "C") return 2;
        return 1;
    }
    
    // Bonus por consultar la GPC
    static constexpr int CONSULTATION_BONUS = 5;
    
    // Evalua una GPC individual contra las acciones del estudiante
    static GuidelineAdherenceResult EvaluateGuideline(
        const Data::Guideline& g,
        bool wasConsulted,
        const std::vector<TreatmentAction>& treatments,
        const std::vector<std::string>& studies,
        const std::string& diagnosis
    ) {
        GuidelineAdherenceResult r;
        r.guidelineId = g.id;
        r.guidelineTitle = g.title;
        r.wasConsulted = wasConsulted;
        r.consultationBonus = wasConsulted ? CONSULTATION_BONUS : 0;
        r.totalRecommendations = static_cast<int>(g.recommendations.size());
        
        // Construir un "texto de acciones" unificado para hacer matching
        std::string actionsText;
        for (const auto& t : treatments) {
            actionsText += t.medication + " " + t.dose + " " + t.rationale + " ";
        }
        for (const auto& s : studies) {
            actionsText += s + " ";
        }
        actionsText += diagnosis;
        
        // Evaluar cada recomendacion
        for (const auto& rec : g.recommendations) {
            int recPoints = PointsForLevel(rec.level);
            r.pointsPossible += recPoints;
            
            // Extraer conceptos de la recomendacion
            auto concepts = KeywordDictionary::ExtractConcepts(rec.text);
            
            if (concepts.empty()) {
                // Recomendacion sin conceptos matcheable (e.g., "vigilar")
                // No penalizamos ni premiamos; queda como "no evaluable"
                continue;
            }
            
            // La recomendacion se cumple si TODOS los conceptos aparecen en las acciones
            bool allMatched = true;
            for (const auto& conceptName : concepts) {
                const auto& synonyms = KeywordDictionary::Concepts().at(conceptName);
                if (!KeywordDictionary::ContainsAnyOf(actionsText, synonyms)) {
                    allMatched = false;
                    break;
                }
            }
            
            if (allMatched) {
                r.matchedRecommendations++;
                r.pointsEarned += recPoints;
                r.matchedItems.push_back("[" + rec.level + "] " + rec.text);
            } else {
                r.missedItems.push_back("[" + rec.level + "] " + rec.text);
            }
        }
        
        r.pointsEarned += r.consultationBonus;
        r.pointsPossible += CONSULTATION_BONUS;
        
        if (r.pointsPossible > 0) {
            r.adherencePercent = 100.0 * r.pointsEarned / r.pointsPossible;
        }
        
        return r;
    }
    
    // Evalua todas las GPCs de la sesion
    static std::vector<GuidelineAdherenceResult> EvaluateSession(
        const std::vector<Data::Guideline>& sessionGuidelines,
        const std::unordered_set<std::string>& consultedIds,
        const std::vector<TreatmentAction>& treatments,
        const std::vector<std::string>& studies,
        const std::string& diagnosis
    ) {
        std::vector<GuidelineAdherenceResult> results;
        results.reserve(sessionGuidelines.size());
        for (const auto& g : sessionGuidelines) {
            bool consulted = consultedIds.count(g.id) > 0;
            results.push_back(EvaluateGuideline(g, consulted, treatments, studies, diagnosis));
        }
        return results;
    }
    
    // Agregados
    static int TotalPoints(const std::vector<GuidelineAdherenceResult>& results) {
        int sum = 0;
        for (const auto& r : results) sum += r.pointsEarned;
        return sum;
    }
    
    static int MaxPoints(const std::vector<GuidelineAdherenceResult>& results) {
        int sum = 0;
        for (const auto& r : results) sum += r.pointsPossible;
        return sum;
    }
    
    static double TotalPercent(const std::vector<GuidelineAdherenceResult>& results) {
        int max = MaxPoints(results);
        return max > 0 ? (100.0 * TotalPoints(results) / max) : 0.0;
    }
};

} // namespace ENARM::Assessment