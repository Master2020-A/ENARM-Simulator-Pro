// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - DatabaseManager Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Data/DatabaseManager.h"
#include "ENARM/Common/Logger.h"

#include <condition_variable>

namespace ENARM::Data {

    DatabaseManager::DatabaseManager(ConnectionConfig config, size_t poolSize)
        : m_config(std::move(config)), m_poolSize(poolSize) {}

    DatabaseManager::~DatabaseManager() {
        std::lock_guard lock(m_mutex);
        while (!m_available.empty()) m_available.pop();
    }

    Common::Result<std::shared_ptr<DatabaseManager>> DatabaseManager::Create(
        const ConnectionConfig& config, size_t poolSize)
    {
        std::shared_ptr<DatabaseManager> mgr(new DatabaseManager(config, poolSize));
        auto init = mgr->Initialize();
        if (init.IsErr()) {
            return Common::Result<std::shared_ptr<DatabaseManager>>::Err(init.Error());
        }
        return mgr;
    }

    Common::Result<void> DatabaseManager::Initialize() {
        try {
            const auto connStr = m_config.ToConnectionString();
            for (size_t i = 0; i < m_poolSize; ++i) {
                auto conn = std::make_shared<pqxx::connection>(connStr);
                if (!conn->is_open()) {
                    return Common::Err<void>(500, "Conexion fallo", "database");
                }
                m_available.push(conn);
            }
            return Common::Ok();
        } catch (const std::exception& e) {
            return Common::Err<void>(500,
                std::string("Error conectando a PostgreSQL: ") + e.what(),
                "database");
        }
    }

    PooledConnection DatabaseManager::Acquire() {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [this]{ return !m_available.empty(); });

        auto conn = m_available.front();
        m_available.pop();

        return PooledConnection(conn,
            [this](std::shared_ptr<pqxx::connection> c) {
                this->ReturnConnection(c);
            });
    }

    void DatabaseManager::ReturnConnection(std::shared_ptr<pqxx::connection> conn) {
        std::lock_guard lock(m_mutex);
        m_available.push(conn);
        m_cv.notify_one();
    }

    std::string DatabaseManager::GetServerVersion() {
        try {
            auto conn = Acquire();
            pqxx::read_transaction tx(conn.Get());
            auto row = tx.exec("SELECT version()").one_row();
            return row[0].as<std::string>();
        } catch (const std::exception& e) {
            return std::string("Error: ") + e.what();
        }
    }

}