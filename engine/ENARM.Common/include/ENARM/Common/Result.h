// ═══════════════════════════════════════════════════════════════
//   ENARM.Common - Result<T, E> Pattern
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <variant>
#include <utility>
#include <type_traits>
#include <optional>

namespace ENARM::Common {

    struct Error {
        int code{0};
        std::string message;
        std::string category;

        Error() = default;
        Error(int c, std::string msg, std::string cat = "general")
            : code(c), message(std::move(msg)), category(std::move(cat)) {}

        [[nodiscard]] std::string ToString() const {
            return "[" + category + ":" + std::to_string(code) + "] " + message;
        }
    };

    template <typename T, typename E = Error>
    class [[nodiscard]] Result {
    public:
        Result(const T& value) : m_data(value) {}
        Result(T&& value) : m_data(std::move(value)) {}
        Result(const E& error) : m_data(error) {}
        Result(E&& error) : m_data(std::move(error)) {}

        static Result Ok(T value) { return Result(std::move(value)); }
        static Result Err(E error) { return Result(std::move(error)); }

        [[nodiscard]] bool IsOk() const noexcept { return std::holds_alternative<T>(m_data); }
        [[nodiscard]] bool IsErr() const noexcept { return std::holds_alternative<E>(m_data); }
        explicit operator bool() const noexcept { return IsOk(); }

        [[nodiscard]] const T& Value() const& { return std::get<T>(m_data); }
        [[nodiscard]] T& Value() & { return std::get<T>(m_data); }
        [[nodiscard]] T&& Value() && { return std::get<T>(std::move(m_data)); }

        [[nodiscard]] const E& Error() const& { return std::get<E>(m_data); }
        [[nodiscard]] E& Error() & { return std::get<E>(m_data); }

        [[nodiscard]] T ValueOr(T defaultValue) const {
            return IsOk() ? Value() : std::move(defaultValue);
        }

    private:
        std::variant<T, E> m_data;
    };

    template <typename E>
    class [[nodiscard]] Result<void, E> {
    public:
        Result() : m_error(std::nullopt) {}
        Result(const E& error) : m_error(error) {}
        Result(E&& error) : m_error(std::move(error)) {}

        static Result Ok() { return Result{}; }
        static Result Err(E error) { return Result(std::move(error)); }

        [[nodiscard]] bool IsOk() const noexcept { return !m_error.has_value(); }
        [[nodiscard]] bool IsErr() const noexcept { return m_error.has_value(); }
        explicit operator bool() const noexcept { return IsOk(); }

        [[nodiscard]] const E& Error() const& { return *m_error; }
        [[nodiscard]] E& Error() & { return *m_error; }

    private:
        std::optional<E> m_error;
    };

    template <typename T>
    Result<T> Ok(T value) { return Result<T>::Ok(std::move(value)); }

    inline Result<void> Ok() { return Result<void>::Ok(); }

    template <typename T = void>
    Result<T> Err(int code, std::string message, std::string category = "general") {
        return Result<T>::Err(Error{code, std::move(message), std::move(category)});
    }

}