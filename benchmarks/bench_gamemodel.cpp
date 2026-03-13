#include "model/gameModel.hpp"
#include <benchmark/benchmark.h>

// =============================================================================
// BM_SpendCredits
// Mide el costo de N llamadas a spendCredits(1).
// Nota: el modelo inicia con 250 créditos — state.range(0) no puede superar
// ese valor. Los rangos están acotados en consecuencia.
// =============================================================================
static void BM_SpendCredits(benchmark::State& state)
{
    for (auto _ : state)
    {
        CyberpunkCba::GameModel model {"Ghost_47"};
        for (int i {0}; i < state.range(0); ++i)
        {
            model.spendCredits(1);
        }
        benchmark::DoNotOptimize(model.credits());
    }
}

BENCHMARK(BM_SpendCredits)->Arg(10)->Arg(100)->Arg(200);

// =============================================================================
// BM_IncrementAlert
// Mide el costo de llevar AlertLevel de None a Maximum (5 pasos).
// =============================================================================
static void BM_IncrementAlert(benchmark::State& state)
{
    for (auto _ : state)
    {
        CyberpunkCba::GameModel model {"Ghost_47"};
        for (int i {0}; i < state.range(0); ++i)
        {
            model.incrementAlert("bench");
        }
        benchmark::DoNotOptimize(model.alertLevel());
    }
}

BENCHMARK(BM_IncrementAlert)->Arg(1)->Arg(5)->Arg(10);

// =============================================================================
// BM_LogAction
// Mide el costo de insertar N entradas en el log de actividad.
// =============================================================================
static void BM_LogAction(benchmark::State& state)
{
    for (auto _ : state)
    {
        CyberpunkCba::GameModel model {"Ghost_47"};
        for (int i {0}; i < state.range(0); ++i)
        {
            model.logAction("benchmark entry");
        }
        benchmark::DoNotOptimize(model.actionLog().size());
    }
}

BENCHMARK(BM_LogAction)->Arg(10)->Arg(100)->Arg(1000);

// =============================================================================
// BM_GameModelConstruction
// Mide el costo de construir y destruir GameModel.
// =============================================================================
static void BM_GameModelConstruction(benchmark::State& state)
{
    for (auto _ : state)
    {
        CyberpunkCba::GameModel model {"Ghost_47"};
        benchmark::DoNotOptimize(model.isRunning());
    }
}

BENCHMARK(BM_GameModelConstruction);

BENCHMARK_MAIN();
