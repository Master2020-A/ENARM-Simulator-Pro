// ═══════════════════════════════════════════════════════════════
//   ENARM.AI - Tipos comunes
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <cstdint>

namespace ENARM::AI {

    // Rol de un mensaje en la conversacion
    enum class MessageRole : uint8_t {
        System,     // Instrucciones al modelo (persona, contexto)
        User,       // Mensaje del usuario/estudiante
        Assistant   // Respuesta del modelo/paciente
    };

    // Mensaje individual en una conversacion
    struct Message {
        MessageRole role;
        std::string content;

        static Message System(std::string c) { return {MessageRole::System, std::move(c)}; }
        static Message User(std::string c) { return {MessageRole::User, std::move(c)}; }
        static Message Assistant(std::string c) { return {MessageRole::Assistant, std::move(c)}; }
    };

    // Opciones para la generacion
    struct GenerationOptions {
        double temperature{0.7};        // 0=deterministico, 2=creativo (paciente: 0.7-0.9)
        double topP{0.9};               // nucleus sampling
        int topK{40};                   // top-k sampling
        int maxTokens{500};             // Max tokens en respuesta
        int seed{-1};                   // -1 = aleatorio
        double repeatPenalty{1.1};      // Penaliza repeticiones
        std::vector<std::string> stop;  // Tokens de parada
        int timeoutSeconds{120};        // Timeout HTTP
        int numCtx{2048};               // Context window (limitar para evitar OOM y lentitud)

        // Presets convenientes
        static GenerationOptions Patient() {
            GenerationOptions o;
            o.temperature = 0.8;    // paciente natural, con variabilidad
            o.maxTokens = 150;      // respuestas cortas (paciente no da discursos)
            o.repeatPenalty = 1.15;
            o.numCtx = 2048;        // contexto limitado para velocidad
            return o;
        }

        static GenerationOptions ClinicalReasoning() {
            GenerationOptions o;
            o.temperature = 0.3;    // mas deterministico para diagnostico
            o.maxTokens = 800;
            return o;
        }

        static GenerationOptions Fast() {
            GenerationOptions o;
            o.temperature = 0.7;
            o.maxTokens = 150;
            return o;
        }
    };

    // Resultado de una generacion
    struct GenerationResult {
        std::string content;                    // Texto generado
        std::string model;                      // Modelo que respondio
        int promptTokens{0};                    // Tokens del prompt
        int completionTokens{0};                // Tokens generados
        int totalTokens{0};                     // Total
        std::chrono::milliseconds duration{0};  // Tiempo total
        double tokensPerSecond{0.0};            // Velocidad
        bool success{false};
        std::string errorMessage;
    };

    // Tipo de tarea (para el Router)
    enum class TaskType : uint8_t {
        PatientResponse,        // Respuesta de paciente virtual
        PatientResponseFast,    // Respuesta rapida (frases cortas)
        ClinicalReasoning,      // Razonamiento diagnostico
        DosageCalculation,      // Calculo de dosis, formulas
        CaseGeneration,         // Generar caso clinico JSON
        EmbeddingGeneration,    // Vectorizar texto
        Custom                  // Usar modelo especifico
    };

}