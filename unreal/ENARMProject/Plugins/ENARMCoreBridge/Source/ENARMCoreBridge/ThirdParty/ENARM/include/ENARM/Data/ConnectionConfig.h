// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - Configuracion de conexion
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <sstream>

namespace ENARM::Data {

    struct ConnectionConfig {
        std::string host          {"localhost"};
        int         port          {5432};
        std::string database      {"enarm_db"};
        std::string user          {"postgres"};
        std::string password      {"manager"};
        int         connectTimeout{10};   // segundos
        std::string sslMode       {"prefer"};  // "disable", "prefer", "require"
        std::string appName       {"ENARM Simulator"};

        // Genera connection string estilo libpq
        [[nodiscard]] std::string ToConnectionString() const {
            std::ostringstream oss;
            oss << "host=" << host
                << " port=" << port
                << " dbname=" << database
                << " user=" << user
                << " password=" << password
                << " connect_timeout=" << connectTimeout
                << " sslmode=" << sslMode
                << " application_name='" << appName << "'";
            return oss.str();
        }

        // Formato URI
        [[nodiscard]] std::string ToURI() const {
            std::ostringstream oss;
            oss << "postgresql://" << user << ":" << password
                << "@" << host << ":" << port << "/" << database;
            return oss.str();
        }
    };

}