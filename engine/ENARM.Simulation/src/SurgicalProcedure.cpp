// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - SurgicalProcedure Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Simulation/SurgicalProcedure.h"

#include <algorithm>
#include <cctype>

namespace ENARM::Simulation {

    namespace {
        std::string ToLower(std::string s) {
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return s;
        }

        bool ContainsAny(const std::string& haystack,
                         const std::vector<std::string>& needles) {
            const std::string h = ToLower(haystack);
            for (const auto& n : needles) {
                if (h.find(ToLower(n)) != std::string::npos) return true;
            }
            return false;
        }
    }

    SurgicalProcedure::SurgicalProcedure() = default;

    void SurgicalProcedure::Start(ProcedureType type) {
        m_status = ProcedureStatus{};
        m_status.type = type;
        m_running = true;
        BuildSteps(type);
    }

    void SurgicalProcedure::BuildSteps(ProcedureType type) {
        switch (type) {
            case ProcedureType::Appendectomy:
                m_status.procedureName = "Apendicectomia";
                m_status.timeLimitSeconds = 1200.0;
                m_status.steps = {
                    {"Preparacion", "Paciente en decubito supino, asepsia y antisepsia",
                     "Bisturi", {"asepsia", "antisepsia", "yodopovidona", "povidona"},
                     {"abrir abdomen sin preparar"}, 5, true},
                    {"Incision", "Incision de McBurney (cuadrante inferior derecho)",
                     "Bisturi", {"mcburney", "incision"},
                     {"incision en cuadrante superior"}, 10, false},
                    {"Aislamiento apendice", "Localizar y exteriorizar el apendice cecal",
                     "Pinzas", {"localizar apendice", "exteriorizar", "ceco"},
                     {"lesionar ciego"}, 10, true},
                    {"Ligadura", "Ligar base apendicular y mesoapendice",
                     "Ligaduras", {"ligadura", "ligar", "mesoapendice"},
                     {"dejar base sin ligar"}, 15, true},
                    {"Cierre", "Cierre por planos de la pared abdominal",
                     "Sutura", {"cierre", "suturar", "piel"},
                     {"dejar piel abierta"}, 5, false}
                };
                break;
            case ProcedureType::LapCholecystectomy:
                m_status.procedureName = "Colecistectomia laparoscopica";
                m_status.timeLimitSeconds = 1500.0;
                m_status.steps = {
                    {"Neumoperitoneo", "Crear neumoperitoneo con aguja de Veress",
                     "Veress", {"neumoperitoneo", "veres", "co2", "veress"},
                     {"abrir sin neumoperitoneo"}, 10, true},
                    {"Trocar de trabajo", "Colocar trocares de trabajo",
                     "Trocart", {"trocar", "trocarez", "puerto"}, {}, 5, false},
                    {"Diseccion triangulo de Calot", "Identificar conducto cistico y arteria cistica",
                     "Disector", {"calot", "cistico", "disector"},
                     {"lesion de via biliar"}, 20, true},
                    {"Ligadura", "Ligamos y cortamos cistico y arteria",
                     "Clip", {"clip", "ligadura", "cistico"},
                     {"no cerrar clips"}, 15, true},
                    {"Extraccion", "Extraer la vesicula por puerto umbilical",
                     "Pinza", {"extraccion", "vesicula"}, {}, 5, false}
                };
                break;
            case ProcedureType::CesareanSection:
                m_status.procedureName = "Cesarea";
                m_status.timeLimitSeconds = 1800.0;
                m_status.steps = {
                    {"Incision piel", "Incision de Pfannenstiel",
                     "Bisturi", {"pfann", "incision"}, {}, 5, false},
                    {"Incision aponeurosis", "Seccionar aponeurosis del oblicuo",
                     "Bisturi", {"aponeurosis"}, {}, 5, false},
                    {"Apertura peritoneo", "Abrir peritoneo parietal",
                     "Tijera", {"peritoneo"}, {}, 5, false},
                    {"Histerotomia", "Incision segmentaria del utero",
                     "Bisturi", {"histerotomia", "utero", "segmentaria"}, {}, 10, true},
                    {"Extraccion neonatal", "Extraer el neonato y clamp del cordon",
                     "Pinza", {"neonato", "cordon", "extraccion"},
                     {"extraccion brusca"}, 15, true},
                    {"Placenta", "Extraccion manual de placenta",
                     "Pinza", {"placenta"}, {}, 10, false},
                    {"Cierre", "Cierre en planos del utero y abdomen",
                     "Sutura", {"cierre", "utero"}, {}, 5, false}
                };
                break;
            case ProcedureType::Thoracostomy:
                m_status.procedureName = "Toracostomia (drenaje pleural)";
                m_status.timeLimitSeconds = 300.0;
                m_status.steps = {
                    {"Preparacion", "Paciente en decubito, zona de insercion",
                     "Bisturi", {"preparacion", "asepsia"}, {}, 5, false},
                    {"Incision", "Incision en 5to espacio intercostal, linea axilar media",
                     "Bisturi", {"5to", "axilar", "incision"}, {}, 10, true},
                    {"Diseccion digital", "Diseccion con dedo sobre el borde superior de la costilla",
                     "Digital", {"dedo", "digital", "borde"}, {"lesionar paquete vascular"}, 10, true},
                    {"Insercion dren", "Insertar tubo toracostomia",
                     "Tubo", {"tubo", "drenaje"}, {}, 10, true},
                    {"Fijacion", "Fijar tubo y conectar a sello de agua",
                     "Sutura", {"sello", "fijacion"}, {}, 5, false}
                };
                break;
            case ProcedureType::Tracheostomy:
                m_status.procedureName = "Traqueostomia";
                m_status.timeLimitSeconds = 600.0;
                m_status.steps = {
                    {"Preparacion", "Paciente con oxigeno y monitorizado",
                     "Monitor", {"oxigeno", "monitor"}, {}, 5, false},
                    {"Incisioncion", "Incision cervical media sobre cartilago cricoides",
                     "Bisturi", {"incision", "cervical"}, {}, 10, true},
                    {"Diseccion", "Separar musculos y glandula tiroides",
                     "Tijera", {"musculos", "tiroides"}, {}, 10, false},
                    {"Traqueostomia", "Incision en la traquea entre anillos 2-3",
                     "Bisturi", {"traquea", "anillo"}, {"lesionar"}, 15, true},
                    {"Cannula", "Insertar la cannula de traqueostomia",
                     "Cannula", {"cannula", "tubo"}, {}, 10, true},
                    {"Fijar", "Fijar cannula con correa",
                     "Sutura", {"fijar", "correa"}, {}, 5, false}
                };
                break;
            case ProcedureType::WoundSuture:
                m_status.procedureName = "Sutura de heridas";
                m_status.timeLimitSeconds = 300.0;
                m_status.steps = {
                    {"Limpieza", "Lavado con suero y antisepsia",
                     "Suero", {"suero", "limpieza", "antisepsia"}, {}, 5, false},
                    {"Anestesia", "Infiltracion de anestesico local",
                     "Lidocaina", {"anestes", "lidocaina"}, {}, 5, false},
                    {"Sutura", "Puntos simples o continos con monofilamento",
                     "Sutura", {"sutura", "punto", "monofilamento"}, {}, 10, false},
                    {"Cierre", "Cierre con Dittricapicula",
                     "Sutura", {"cierre"}, {}, 5, false}
                };
                break;
            case ProcedureType::AbscessDrainage:
                m_status.procedureName = "Drenaje de abscesos";
                m_status.timeLimitSeconds = 300.0;
                m_status.steps = {
                    {"Preparacion", "Antisepsia y anestesia local",
                     "Lidoca", {"antisepsia", "anestes"}, {}, 5, false},
                    {"Incision", "Incision con bisturi #11",
                     "Bisturi", {"incision", "bisturi"}, {}, 5, false},
                    {"Drenaje", "Evacuar contenido purulento",
                     "Suero", {"drenaje", "evacuar"}, {}, 10, false},
                    {"Cultivo", "Toma de cultivo para bacterias",
                     "Tubo", {"cultivo", "muestra"}, {}, 10, true},
                    {"Cierre", "Dejar drenaje y cubrir",
                     "Gasas", {"drenaje", "gasas"}, {}, 5, false}
                };
                break;
            case ProcedureType::AdvancedCPR:
                m_status.procedureName = "RCP Avanzado";
                m_status.timeLimitSeconds = 300.0;
                m_status.steps = {
                    {"Valoracion", "Comprobar inconsciencia y respiracion",
                     "Monitor", {"inconsciencia", "respiracion"}, {}, 5, true},
                    {"Compresiones", "Compresiones 30:2",
                     "Manos", {"compresiones", "30:2"}, {}, 10, true},
                    {"Via aerea", "Abrir via aerea y ventilacion",
                     "Ambu", {"ambu", "via aerea", "ventilacion"}, {}, 5, true},
                    {"Desfibrilacion", "Analisis y descarga del DAE",
                     "Dae", {"dae", "desfibrilacion", "descarga"}, {}, 15, true},
                    {"Adrenalina", "Epinefrina 1mg IV cada 3-5 min",
                     "Jeringa", {"epinefrina", "adrenalina", "1mg"}, {}, 10, true}
                };
                break;
            default:
                m_status.procedureName = "Procedimiento";
                break;
        }

        // Calcular maxPoints
        m_status.maxPoints = 0;
        for (const auto& s : m_status.steps) m_status.maxPoints += s.points;
    }

    void SurgicalProcedure::Update(double dt_s) {
        if (!m_running) return;
        m_status.elapsedSeconds += dt_s;
        if (m_status.elapsedSeconds >= m_status.timeLimitSeconds) {
            m_running = false;
            m_status.completed = true;
            m_status.complications.push_back("Tiempo excedido: procedimiento incompleto");
        }
    }

    bool SurgicalProcedure::IsCorrectForStep(const std::string& action) const {
        if (m_status.IsFinished() || m_status.steps.empty()) return false;
        const auto& step = m_status.steps[m_status.currentStep];
        if (ContainsAny(action, step.correctActions)) return true;
        return false;
    }

    bool SurgicalProcedure::PerformAction(const std::string& actionName) {
        if (!m_running || m_status.IsFinished()) return false;
        const auto& step = m_status.steps[m_status.currentStep];

        // Error critico?
        if (!step.wrongActions.empty() && ContainsAny(actionName, step.wrongActions)) {
            m_status.complications.push_back(
                "Error en paso '" + step.name + "': " + actionName);
            m_status.patientStable = false;
            m_running = false;
            m_status.completed = true;
            return false;
        }

        // Accion correcta para este paso?
        if (IsCorrectForStep(actionName)) {
            m_status.steps[m_status.currentStep].completed = true;
            m_status.totalPoints += step.points;
            return true;
        }
        return false;
    }

    void SurgicalProcedure::NextStep() {
        if (m_status.currentStep < m_status.steps.size()) {
            ++m_status.currentStep;
        }
        if (m_status.IsFinished()) {
            m_running = false;
            m_status.completed = true;
        }
    }

    std::string SurgicalProcedure::GetProcedureName() const {
        return m_status.procedureName;
    }

    std::vector<std::string> SurgicalProcedure::AvailableInstruments(ProcedureType type) {
        switch (type) {
            case ProcedureType::Appendectomy:
                return {"Bisturi", "Tijera", "Pinzas", "Ligaduras", "Sutura"};
            case ProcedureType::LapCholecystectomy:
                return {"Veress", "Trocar", "Disector", "Clips", "Pinza"};
            case ProcedureType::CesareanSection:
                return {"Bisturi", "Tijera", "Pinza", "Sutura", "Luna"};
            case ProcedureType::Thoracostomy:
                return {"Bisturi", "Tubo", "Sutura", "Suero"};
            case ProcedureType::Tracheostomy:
                return {"Bisturi", "Cannula", "Sutura", "Suero"};
            case ProcedureType::WoundSuture:
                return {"Sutura", "Lidocaína", "Suero", "Gasas"};
            case ProcedureType::AbscessDrainage:
                return {"Bisturi", "Suero", "Gasas", "Tubo cultivo"};
            case ProcedureType::AdvancedCPR:
                return {"Manos", "Ambu", "DAE", "Jeringa"};
        }
        return {};
    }

}