// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - DatabaseManager con pool basico
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Common/Result.h"
#include "ENARM/Data/ConnectionConfig.h"

#include <pqxx/pqxx>
#include <memory>
#include <mutex>
#include <queue>
#include <functional>
#include <string>

namespace ENARM::Data {

    // RAII wrapper que devuelve la conexion al pool al destruirse
    class PooledConnection {
    public:
        PooledConnection(std::shared_ptr<pqxx::connection> conn,
                         std::function<void(std::shared_ptr<pqxx::connection>)> returnFn)
            : m_conn(conn), m_return(returnFn) {}

        ~PooledConnection() {
            if (m_conn && m_return) m_return(m_conn);
        }

        PooledConnection(const PooledConnection&) = delete;
        PooledConnection& operator=(const PooledConnection&) = delete;
        PooledConnection(PooledConnection&&) noexcept = default;

        [[nodiscard]] pqxx::connection& Get() { return *m_conn; }
        [[nodiscard]] pqxx::connection* operator->() { return m_conn.get(); }
        [[nodiscard]] pqxx::connection& operator*() { return *m_conn; }

    private:
        std::shared_ptr<pqxx::connection> m_conn;
        std::function<void(std::shared_ptr<pqxx::connection>)> m_return;
    };

    class DatabaseManager {
    public:
        // Factory: crea el manager y prueba conexion
        static Common::Result<std::shared_ptr<DatabaseManager>> Create(
            const ConnectionConfig& config,
            size_t poolSize = 5);

        ~DatabaseManager();

        DatabaseManager(const DatabaseManager&) = delete;
        DatabaseManager& operator=(const DatabaseManager&) = delete;

        // Obtiene una conexion del pool (bloqueante si esta lleno)
        [[nodiscard]] PooledConnection Acquire();

        // Ejecutar una transaccion sencilla
        template <typename Fn>
        auto Execute(Fn&& fn) {
            auto conn = Acquire();
            pqxx::work tx(conn.Get());
            auto result = fn(tx);
            tx.commit();
            return result;
        }

        // Solo lectura
        template <typename Fn>
        auto ExecuteRead(Fn&& fn) {
            auto conn = Acquire();
            pqxx::read_transaction tx(conn.Get());
            return fn(tx);
        }

        // Info
        [[nodiscard]] const ConnectionConfig& GetConfig() const noexcept { return m_config; }
        [[nodiscard]] size_t GetPoolSize() const noexcept { return m_poolSize; }
        [[nodiscard]] std::string GetServerVersion();

    private:
        DatabaseManager(ConnectionConfig config, size_t poolSize);
        Common::Result<void> Initialize();
        void ReturnConnection(std::shared_ptr<pqxx::connection> conn);

        ConnectionConfig m_config;
        size_t m_poolSize;

        std::mutex m_mutex;
        std::condition_variable m_cv;
        std::queue<std::shared_ptr<pqxx::connection>> m_available;
    };

}