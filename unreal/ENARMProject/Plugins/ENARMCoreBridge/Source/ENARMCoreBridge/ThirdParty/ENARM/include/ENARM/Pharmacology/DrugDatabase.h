// ═══════════════════════════════════════════════════════════════
//   ENARM.Pharmacology - Base de datos de farmacos ICU
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Pharmacology/DrugParameters.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

namespace ENARM::Pharmacology {

    class DrugDatabase {
    public:
        // Singleton
        static DrugDatabase& Instance();

        // Busqueda
        [[nodiscard]] std::optional<DrugParameters> FindByName(const std::string& name) const;
        [[nodiscard]] std::vector<DrugParameters> FindByCategory(DrugCategory cat) const;
        [[nodiscard]] std::vector<std::string> ListAllNames() const;
        [[nodiscard]] size_t Count() const noexcept { return m_drugs.size(); }

    private:
        DrugDatabase();
        void RegisterAllDrugs();

        // Helpers para crear farmacos comunes
        void RegisterVasopressors();
        void RegisterInotropes();
        void RegisterSedatives();
        void RegisterAnalgesics();
        void RegisterAnesthetics();
        void RegisterNMBAs();
        void RegisterAntiarrhythmics();
        void RegisterOthers();

        std::unordered_map<std::string, DrugParameters> m_drugs;
    };

}