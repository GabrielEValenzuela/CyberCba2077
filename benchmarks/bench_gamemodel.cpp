#include <benchmark/benchmark.h>
#include "model/GameModel.hpp"

static void BM_AddCredits(benchmark::State& state) {
  for (auto _ : state) {
    cyberpunk::GameModel model;
    for (int i = 0; i < state.range(0); ++i) {
      model.addCredits(1);
    }
    benchmark::DoNotOptimize(model.credits());
  }
}

BENCHMARK(BM_AddCredits)->Arg(10)->Arg(100)->Arg(1000);

BENCHMARK_MAIN();
