# Testing

`unit_tests` cubre GameModel, Queue heredada, save faltante/corrupto/round-trip, exclusividad de capacidades, hacking/cooldown, combate/daño/invulnerabilidad, checkpoints y finalización de misión. Ejecutar `ctest --test-dir build/dev --output-on-failure`. El benchmark `benchmarks_run` preserva la medición de GameModel.
