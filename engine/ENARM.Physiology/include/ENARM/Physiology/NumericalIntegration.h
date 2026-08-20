// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - Integracion numerica (RK4)
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <functional>

namespace ENARM::Physiology {

    // Runge-Kutta 4to orden clasico
    // Resuelve: dy/dt = f(t, y)
    // Retorna: y(t + dt)
    template <typename F>
    inline double RK4(double t, double y, double dt, F f) {
        const double k1 = f(t,             y);
        const double k2 = f(t + dt * 0.5,  y + dt * 0.5 * k1);
        const double k3 = f(t + dt * 0.5,  y + dt * 0.5 * k2);
        const double k4 = f(t + dt,        y + dt * k3);
        return y + (dt / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
    }

    // Euler simple (para debugging o cuando no importa precision)
    template <typename F>
    inline double Euler(double t, double y, double dt, F f) {
        return y + dt * f(t, y);
    }

    // Clamp helper
    inline double Clamp(double v, double lo, double hi) noexcept {
        return v < lo ? lo : (v > hi ? hi : v);
    }

}