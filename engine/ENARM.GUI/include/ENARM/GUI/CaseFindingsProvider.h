// =====================================================================
//   ENARM.GUI - CaseFindingsProvider
//   Provee findings de exploracion fisica y resultados de estudios
//   por caso clinico (case_001 a case_010).
//   FIX Sprint 7A.2.5: elimina hardcodes de IAM en TODOS los casos.
// =====================================================================
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace ENARM::GUI {

    class CaseFindingsProvider {
    public:
        // Areas estandar de exploracion fisica (siempre las mismas para UX consistente)
        static std::vector<std::string> GetStandardExamAreas();

        // Estudios estandar disponibles en ENARM (mismos para todos los casos)
        // El alumno debe saber CUALES pedir para cada caso.
        static std::vector<std::string> GetStandardStudies();

        // ---- Datos POR CASO ----

        // Findings de exploracion fisica segun caseId + area
        // caseId puede ser "case_004_neumonia", "case_006_apendicitis", etc.
        static std::string GetExamFinding(const std::string& caseId,
                                            const std::string& examArea);

        // Resultado de un estudio segun caseId + nombre del estudio
        static std::string GetStudyResult(const std::string& caseId,
                                            const std::string& studyName);

    private:
        // Detectar case_id a partir de string (busca "001", "iam", "neumonia", etc.)
        static std::string NormalizeCaseId(const std::string& rawId);
    };

}