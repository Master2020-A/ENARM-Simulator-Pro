// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - UserRepository (libpqxx v7.10+)
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Data/Repositories/UserRepository.h"

namespace ENARM::Data {

    namespace {
        template <typename Row>
        User RowToUser(const Row& row) {
            User u;
            u.id             = row["id"].template as<std::string>();
            u.email          = row["email"].template as<std::string>();
            u.fullName       = row["full_name"].template as<std::string>();
            u.role           = row["role"].template as<std::string>();
            u.emailVerified  = row["email_verified"].template as<bool>();
            u.active         = row["active"].template as<bool>();
            return u;
        }
    }

    Common::Result<User> UserRepository::Create(
        const std::string& email, const std::string& password,
        const std::string& fullName, const std::string& role)
    {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());

            auto row = tx.exec(
                "INSERT INTO users (email, password_hash, full_name, role, email_verified) "
                "VALUES ($1, crypt($2, gen_salt('bf')), $3, $4, false) "
                "RETURNING id, email, full_name, role, email_verified, active",
                pqxx::params{email, password, fullName, role}
            ).one_row();

            tx.commit();
            return Common::Result<User>::Ok(RowToUser(row));
        } catch (const std::exception& e) {
            return Common::Err<User>(500, e.what(), "user_repo");
        }
    }

    std::optional<User> UserRepository::FindByEmail(const std::string& email) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT id, email, full_name, role, email_verified, active "
                "FROM users WHERE email = $1",
                pqxx::params{email});
            if (res.empty()) return std::nullopt;
            return RowToUser(res[0]);
        } catch (...) {
            return std::nullopt;
        }
    }

    std::optional<User> UserRepository::FindById(const std::string& id) {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT id, email, full_name, role, email_verified, active "
                "FROM users WHERE id = $1::uuid",
                pqxx::params{id});
            if (res.empty()) return std::nullopt;
            return RowToUser(res[0]);
        } catch (...) {
            return std::nullopt;
        }
    }

    Common::Result<User> UserRepository::VerifyPassword(
        const std::string& email, const std::string& password)
    {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto res = tx.exec(
                "SELECT id, email, full_name, role, email_verified, active "
                "FROM users WHERE email = $1 AND password_hash = crypt($2, password_hash)",
                pqxx::params{email, password});
            if (res.empty()) {
                return Common::Err<User>(401, "Credenciales invalidas", "auth");
            }
            return Common::Result<User>::Ok(RowToUser(res[0]));
        } catch (const std::exception& e) {
            return Common::Err<User>(500, e.what(), "auth");
        }
    }

    void UserRepository::UpdateLastLogin(const std::string& userId) {
        try {
            auto conn = m_db->Acquire();
            pqxx::work tx(conn.Get());
            tx.exec("UPDATE users SET last_login = NOW() WHERE id = $1::uuid",
                    pqxx::params{userId});
            tx.commit();
        } catch (...) {}
    }

    size_t UserRepository::Count() {
        try {
            auto conn = m_db->Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto row = tx.exec("SELECT COUNT(*) FROM users").one_row();
            return row[0].as<size_t>();
        } catch (...) { return 0; }
    }

}