#!/usr/bin/env python3
import json
import sys

THRESHOLD = float(sys.argv[3]) if len(sys.argv) > 3 else 0.15


def load(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def index_bench(data):
    out = {}
    for b in data.get("benchmarks", []):
        name = b.get("name")
        # google benchmark: prefer cpu_time, else real_time
        t = b.get("cpu_time", b.get("real_time"))
        unit = b.get("time_unit", "ns")
        if name and t is not None:
            out[name] = (float(t), unit)
    return out


base = index_bench(load(sys.argv[1]))
pr = index_bench(load(sys.argv[2]))

regressions = []
for name, (t_pr, unit_pr) in pr.items():
    if name in base:
        t_base, unit_base = base[name]
        # assume same unit (true if same runner); if not, we still ratio times
        ratio = (t_pr / t_base) if t_base > 0 else 1.0
        if ratio > 1.0 + THRESHOLD:
            regressions.append((name, t_base, t_pr, ratio))

# print summary (used by bot)
print(
    f"Compared {len(pr)} benchmarks vs baseline ({len(base)}). Threshold={THRESHOLD * 100:.0f}%"
)
if regressions:
    print("REGRESSIONS:")
    for name, t_base, t_pr, ratio in sorted(
        regressions, key=lambda x: x[3], reverse=True
    )[:10]:
        print(
            f"- {name}: {t_base:.2f} -> {t_pr:.2f}  ({(ratio - 1) * 100:.1f}% slower)"
        )
    sys.exit(2)

print("No significant regressions detected.")
sys.exit(0)
