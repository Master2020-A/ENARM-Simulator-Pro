// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - Prompt templates para pacientes virtuales
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace ENARM::AI::PromptTemplates {

    // Datos para construir persona del paciente
    struct PatientPersonaData {
        std::string fullName;
        int age{0};
        std::string sex;                    // "Male"/"Female"
        std::string chiefComplaint;         // Motivo de consulta
        std::string historyPresentIllness;  // HEA narrativa
        nlohmann::json symptoms;            // Sintomas del caso
        nlohmann::json medicalHistory;      // Antecedentes
        nlohmann::json vitalSigns;          // Signos vitales actuales
        nlohmann::json familyHistory;
        nlohmann::json lifestyle;
        std::string educationLevel{"basico"};   // Nivel para adaptar lenguaje
        std::string emotionalState{"ansioso"};  // Estado emocional
    };

    class PatientPrompts {
    public:
        // Genera el prompt SISTEMA (persona del paciente)
        static std::string BuildSystemPrompt(const PatientPersonaData& data);

        // Prompt para saludo inicial (opcional)
        static std::string BuildInitialGreetingPrompt(const PatientPersonaData& data);

        // Guardrails: instrucciones que evitan que el paciente "rompa personaje"
        static std::string GetGuardrails();
    };

}