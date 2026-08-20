#include <enarm/assessment/CompetencyEvaluator.h>
#include <algorithm>
#include <sstream>
#include <cctype>

namespace enarm::assessment {

// ----------------------------------------------------------------
// Tabla de sinonimos medicos
// ----------------------------------------------------------------
const std::unordered_map<std::string, std::vector<std::string>>&
CompetencyEvaluator::getSynonymTable() {
    static const std::unordered_map<std::string, std::vector<std::string>> table = {
        // Semiologia
        {"dolor toracico",   {"dolor de pecho","dolor precordial","opresion toracica","malestar toracico"}},
        {"irradiacion",      {"se extiende","corre hacia","va hacia","irradia","propaga"}},
        {"inicio",           {"cuando empezo","desde cuando","hace cuanto","cuanto tiempo"}},
        {"intensidad",       {"que tan fuerte","escala de dolor","del 1 al 10","cuanto duele"}},
        {"duracion",         {"cuanto dura","tiempo que lleva","desde hace"}},
        {"factores agravantes",{"que lo empeora","que aumenta","esfuerzo","al caminar"}},
        {"factores atenuantes",{"que lo alivia","que lo mejora","con el reposo","nitroglicerina"}},
        // Sintomas
        {"disnea",           {"falta de aire","dificultad para respirar","ahogo","falta de aliento","me falta el aire"}},
        {"fiebre",           {"temperatura","calentura","hipertermia","febricula","calor"}},
        {"cefalea",          {"dolor de cabeza","jaqueca","migrania"}},
        {"nausea",           {"asco","ganas de vomitar","nauseas"}},
        {"edema",            {"hinchazon","inflamacion","retencion de liquidos","piernas hinchadas"}},
        {"diaforesis",       {"sudoracion","sudor frio","sudando","transpiracion"}},
        {"palpitaciones",    {"corazon acelerado","latidos rapidos","me late rapido"}},
        {"sincope",          {"desmayo","perdida de conocimiento","me desvaneci","me caí"}},
        // Antecedentes
        {"hipertension",     {"presion alta","tension alta","hta","hipertenso"}},
        {"diabetes",         {"azucar alta","diabetico","glucosa elevada","dm2","dm1"}},
        {"tabaquismo",       {"fuma","fumador","cigarro","cigarrillo","tabaco"}},
        {"dislipidemia",     {"colesterol","trigliceridos","lipidos altos"}},
        {"antecedentes familiares",{"familia","papa","mama","hermano","herencia","familiar"}},
        {"medicamentos",     {"farmacos","medicina","pastillas","tratamiento actual","toma algo"}},
        {"alergias",         {"reacciones alergicas","alergia a medicamentos","intolerancia"}},
        // Exploracion
        {"presion arterial", {"pa","tension arterial","presion","sistolica","diastolica"}},
        {"frecuencia cardiaca",{"fc","pulso","latidos por minuto","ritmo cardiaco"}},
        {"frecuencia respiratoria",{"fr","respiraciones por minuto","ritmo respiratorio"}},
        {"saturacion de oxigeno",{"spo2","saturacion","oximetria","oxigeno en sangre"}},
        {"auscultacion cardiaca",{"corazon","ruidos cardiacos","soplo","s1","s2","auscultando el corazon"}},
        {"auscultacion pulmonar",{"pulmones","murmullo vesicular","estertores","sibilancias","auscultando pulmones"}},
        {"ingurgitacion yugular",{"yugulares","venas del cuello","distension yugular"}},
        {"pulsos perifericos", {"pulso radial","pulsos","circulacion periferica"}},
        {"llenado capilar",   {"relleno capilar","perfusion periferica"}},
        // Estudios
        {"electrocardiograma",{"ecg","ekg","trazo","12 derivaciones","electro"}},
        {"troponina",         {"troponinas","troponina i","troponina t","biomarcadores cardiacos"}},
        {"biometria hematica",{"bh","hemograma","conteo sanguineo","biometria"}},
        {"quimica sanguinea", {"qs","glucosa","urea","creatinina","quimica"}},
        {"radiografia de torax",{"rx torax","placa","radiografia","rx de torax"}},
        {"gasometria",        {"gases arteriales","aga","gas arterial","gases en sangre"}},
        {"hemocultivos",      {"cultivos de sangre","hemocultivo","cultivo"}},
        {"lactato",           {"acido lactico","lactato serico"}},
        // Diagnosticos
        {"infarto agudo de miocardio",{"iam","iamcest","iamssest","infarto","sindrome coronario agudo","sca"}},
        {"shock septico",     {"sepsis","choque septico","septicemia","choque infectante"}},
        {"acv isquemico",     {"ictus","evento vascular cerebral","evc","derrame cerebral","infarto cerebral","stroke"}},
        // Tratamientos
        {"aspirina",          {"acido acetilsalicilico","asa","aspirina 300"}},
        {"clopidogrel",       {"plavix","antiagregante"}},
        {"heparina",          {"anticoagulacion","hbpm","enoxaparina"}},
        {"reperfusion",       {"cateterismo","angioplastia","pci","fibrinolitico","trombolisis","tenecteplase"}},
        {"oxigeno",           {"suplemento de oxigeno","mascarilla","puntas nasales","o2"}},
        {"antibiotico",       {"antibioticos","antimicrobiano","penicilina","ceftriaxona","pip-tazo","vancomicina"}},
        {"betabloqueador",    {"metoprolol","atenolol","carvedilol"}},
        {"estatina",          {"atorvastatina","rosuvastatina","simvastatina"}},
        {"alteplase",         {"rtpa","tpa","trombolitico cerebral","activador tisular"}},
    };
    return table;
}

// ----------------------------------------------------------------
// normalize: minusculas + sin tildes basico
// ----------------------------------------------------------------
std::string CompetencyEvaluator::normalize(const std::string& text) const {
    std::string out;
    out.reserve(text.size());
    for (unsigned char c : text) {
        // Minusculas
        char lc = static_cast<char>(std::tolower(c));
        // Tildes comunes -> sin tilde
        switch (c) {
            case 0xE1: case 0xC1: lc = 'a'; break; // á Á
            case 0xE9: case 0xC9: lc = 'e'; break; // é É
            case 0xED: case 0xCD: lc = 'i'; break; // í Í
            case 0xF3: case 0xD3: lc = 'o'; break; // ó Ó
            case 0xFA: case 0xDA: lc = 'u'; break; // ú Ú
            case 0xFC: case 0xDC: lc = 'u'; break; // ü Ü
            case 0xF1: case 0xD1: lc = 'n'; break; // ñ Ñ
            default: break;
        }
        out += lc;
    }
    return out;
}

// ----------------------------------------------------------------
// containsSemanticMatch
// ----------------------------------------------------------------
bool CompetencyEvaluator::containsSemanticMatch(
    const std::string& text,
    const std::string& conceptName,
    const std::vector<std::string>& synonyms) const
{
    std::string normText    = normalize(text);
    std::string normConcept = normalize(conceptName);

    // Match directo
    if (normText.find(normConcept) != std::string::npos) return true;

    // Sinonimos pasados explicitamente
    for (const auto& syn : synonyms) {
        if (normText.find(normalize(syn)) != std::string::npos) return true;
    }

    // Tabla global de sinonimos
    const auto& table = getSynonymTable();
    for (const auto& [key, syns] : table) {
        if (normConcept.find(normalize(key)) != std::string::npos ||
            normalize(key).find(normConcept) != std::string::npos) {
            for (const auto& syn : syns) {
                if (normText.find(normalize(syn)) != std::string::npos) return true;
            }
        }
    }

    return false;
}

// ----------------------------------------------------------------
// extractTextsForDimension: textos relevantes segun dimension
// ----------------------------------------------------------------
std::vector<std::string> CompetencyEvaluator::extractTextsForDimension(
    const std::vector<StudentAction>& actions,
    CompetencyDimension dimension) const
{
    std::vector<std::string> texts;
    for (const auto& a : actions) {
        bool include = false;
        switch (dimension) {
            case CompetencyDimension::ClinicalInterview:
                include = (a.type == StudentAction::Type::ConversationTurn);
                break;
            case CompetencyDimension::PhysicalExamination:
                include = (a.type == StudentAction::Type::PhysicalExam);
                break;
            case CompetencyDimension::DiagnosticStudies:
                include = (a.type == StudentAction::Type::StudyOrdered);
                break;
            case CompetencyDimension::ClinicalDiagnosis:
                include = (a.type == StudentAction::Type::DiagnosisSubmitted);
                break;
            case CompetencyDimension::TreatmentPlan:
                include = (a.type == StudentAction::Type::TreatmentOrdered);
                break;
            case CompetencyDimension::FollowUpMonitoring:
                include = (a.type == StudentAction::Type::FollowUpAction);
                break;
        }
        if (include) {
            texts.push_back(a.primary);
            if (!a.secondary.empty()) texts.push_back(a.secondary);
        }
    }
    return texts;
}

// ----------------------------------------------------------------
// getPerformanceLevel
// ----------------------------------------------------------------
std::string CompetencyEvaluator::getPerformanceLevel(float percentage) {
    if (percentage >= 90.0f) return "Excelente";
    if (percentage >= 70.0f) return "Bueno";
    if (percentage >= 50.0f) return "Adecuado";
    return "Insuficiente";
}

// ----------------------------------------------------------------
// generateCriterionJustification
// ----------------------------------------------------------------
std::string CompetencyEvaluator::generateCriterionJustification(
    const RubricCriterion&          criterion,
    float                           coverage,
    const std::vector<std::string>& found,
    const std::vector<std::string>& missed) const
{
    std::ostringstream ss;
    ss << criterion.description << ": ";
    if (coverage >= 0.9f)       ss << criterion.excellentDescription;
    else if (coverage >= 0.7f)  ss << criterion.goodDescription;
    else if (coverage >= 0.5f)  ss << criterion.adequateDescription;
    else                        ss << criterion.insufficientDescription;

    if (!missed.empty()) {
        ss << " (Faltaron: ";
        for (size_t i = 0; i < missed.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << missed[i];
        }
        ss << ")";
    }
    return ss.str();
}

// ----------------------------------------------------------------
// evaluateCriterion
// ----------------------------------------------------------------
CriterionScore CompetencyEvaluator::evaluateCriterion(
    const RubricCriterion&           criterion,
    const std::vector<StudentAction>& actions) const
{
    CriterionScore cs;
    cs.criterionId = criterion.id;
    cs.maxPoints   = criterion.maxPoints;

    // Concatenar todo el texto de las acciones
    std::string allText;
    for (const auto& a : actions) {
        allText += " " + a.primary + " " + a.secondary;
    }

    int elementsFound = 0;
    const size_t n = criterion.keyElements.size();

    for (size_t i = 0; i < n; ++i) {
        const auto& elem = criterion.keyElements[i];
        std::vector<std::string> syns;
        if (i < criterion.synonymGroups.size())
            syns = criterion.synonymGroups[i];

        if (containsSemanticMatch(allText, elem, syns)) {
            cs.elementsIdentified.push_back(elem);
            ++elementsFound;
        } else {
            cs.elementsMissed.push_back(elem);
        }
    }

    float coverage = (n > 0)
        ? static_cast<float>(elementsFound) / static_cast<float>(n)
        : 0.0f;

    // Aplicar peso del criterio
    float weightedCoverage = std::min(1.0f, coverage * criterion.weight);
    cs.pointsEarned = static_cast<int>(weightedCoverage * criterion.maxPoints);
    cs.percentage   = weightedCoverage * 100.0f;
    cs.justification = generateCriterionJustification(
        criterion, coverage, cs.elementsIdentified, cs.elementsMissed);

    return cs;
}

// ----------------------------------------------------------------
// evaluateDimension
// ----------------------------------------------------------------
DimensionScore CompetencyEvaluator::evaluateDimension(
    const DimensionRubric&           rubric,
    const std::vector<StudentAction>& actions,
    float                            timeSpentSeconds) const
{
    DimensionScore score;
    score.dimension        = rubric.dimension;
    score.maxPoints        = rubric.maxPoints;
    score.timeSpentMinutes = timeSpentSeconds / 60.0f;

    for (const auto& criterion : rubric.criteria) {
        // Solo pasar acciones relevantes para esta dimension
        std::vector<StudentAction> relevant = actions;  // Evaluator usa allText interno
        CriterionScore cs = evaluateCriterion(criterion, actions);
        score.pointsEarned += cs.pointsEarned;
        score.criterionScores.push_back(std::move(cs));
    }

    // Clamp al maximo de la dimension
    score.pointsEarned = std::min(score.pointsEarned, score.maxPoints);
    score.percentage   = (score.maxPoints > 0)
        ? (static_cast<float>(score.pointsEarned) / score.maxPoints) * 100.0f
        : 0.0f;
    score.performanceLevel = getPerformanceLevel(score.percentage);

    return score;
}

} // namespace enarm::assessment
