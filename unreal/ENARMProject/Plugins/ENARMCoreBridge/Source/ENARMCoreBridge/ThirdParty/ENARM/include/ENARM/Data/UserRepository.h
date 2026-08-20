// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - UserRepository
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Common/Result.h"
#include "ENARM/Data/DatabaseManager.h"

#include <memory>
#include <string>
#include <optional>
#include <chrono>

namespace ENARM::Data {

    struct User {
        std::string id;   // UUID como string
        std::string email;
        std::string fullName;
        std::string role;
        bool        emailVerified{false};
        bool        active{true};
    };

    class UserRepository {
    public:
        explicit UserRepository(std::shared_ptr<DatabaseManager> db) : m_db(db) {}

        // Crear nuevo usuario (password se hashea con bcrypt via pgcrypto)
        Common::Result<User> Create(const std::string& email,
                                     const std::string& password,
                                     const std::string& fullName,
                                     const std::string& role = "student");

        // Buscar por email
        std::optional<User> FindByEmail(const std::string& email);
        std::optional<User> FindById(const std::string& id);

        // Verificar password (login)
        Common::Result<User> VerifyPassword(const std::string& email,
                                             const std::string& password);

        // Actualizar last_login
        void UpdateLastLogin(const std::string& userId);

        // Total registrados
        [[nodiscard]] size_t Count();

    private:
        std::shared_ptr<DatabaseManager> m_db;
    };

}