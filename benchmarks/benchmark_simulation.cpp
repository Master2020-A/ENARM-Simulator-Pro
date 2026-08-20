// ============================================================
//  benchmark_simulation.cpp - Benchmark de simulacion paralela
//  Mide cuantos pacientes puede simular en paralelo (60fps)
//  Uso: ENARM.Benchmarks.exe [numPacientes] [segundos]
// ============================================================
#include "ENARM/Simulation/PatientSimulator.h"
#include "ENARM/Simulation/ClinicalScenarios.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>

using namespace ENARM::Simulation;

int main(int argc, char* argv[]) {
    const int numPatients = (argc > 1) ? std::atoi(argv[1]) : 100;
    const double simSeconds = (argc > 2) ? std::atof(argv[2]) : 60.0;
    const double dt = 1.0 / 60.0;  // 60 fps

    printf("============================================\n");
    printf("  ENARM Benchmark - Simulacion paralela\n");
    printf("  Pacientes: %d | Simulacion: %.0f s | dt: %.4f s\n",
           numPatients, simSeconds, dt);
    printf("============================================\n\n");

    // Crear pacientes con escenarios variados
    std::vector<PatientSimulator> patients;
    patients.reserve(numPatients);
    for (int i = 0; i < numPatients; ++i) {
        PatientSimulator p(70.0);
        switch (i % 5) {
            case 0: p.ApplyScenario(ScenarioType::SepticShock); break;
            case 1: p.ApplyScenario(ScenarioType::AcuteMI_STEMI); break;
            case 2: p.ApplyScenario(ScenarioType::SevereAsthma); break;
            case 3: p.ApplyScenario(ScenarioType::HypovolemicShock); break;
            default: p.ApplyScenario(ScenarioType::HealthyBaseline); break;
        }
        patients.push_back(std::move(p));
    }

    // Medir tiempo de simulacion
    const int steps = static_cast<int>(simSeconds / dt);
    auto start = std::chrono::high_resolution_clock::now();

    for (int step = 0; step < steps; ++step) {
        for (auto& p : patients) {
            p.Step(dt);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();

    // Resultados
    double simPerSec = (numPatients * steps) / elapsed;
    double fps = steps / elapsed;
    double msPerStep = (elapsed / steps) * 1000.0;

    printf("Resultados:\n");
    printf("  Tiempo total:        %.2f s\n", elapsed);
    printf("  Pasos simulados:     %d\n", steps);
    printf("  FPS de simulacion:   %.1f\n", fps);
    printf("  ms por paso:         %.3f ms\n", msPerStep);
    printf("  Pacientes/segundo:   %.0f\n", simPerSec);
    printf("  Pacientes a 60fps:   %.0f\n", 1000.0 / (msPerStep * 60.0));
    printf("\n");

    // Verificar que todos los pacientes siguen vivos
    int alive = 0;
    for (auto& p : patients) {
        auto snap = p.GetSnapshot();
        if (snap.MAP > 0.0) ++alive;
    }
    printf("Pacientes con MAP > 0: %d/%d\n", alive, numPatients);

    return 0;
}
