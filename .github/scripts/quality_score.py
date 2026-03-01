#!/usr/bin/env python3
import os
import re
import sys


def clamp(x):
    return max(0, min(100, x))


format_ok = os.getenv("Q_FORMAT_OK", "false") == "true"
build_ok = os.getenv("Q_BUILD_OK", "false") == "true"
tests_ok = os.getenv("Q_TESTS_OK", "false") == "true"

coverage = float(os.getenv("Q_COVERAGE", "0"))  # 0..100
tidy_warnings = int(os.getenv("Q_TIDY_WARNINGS", "999999"))

score = 0
score += 20 if format_ok else 0
score += 20 if build_ok else 0
score += 20 if tests_ok else 0

# Coverage scoring: 0->0, 50->10, 80->18, 90->20
if coverage >= 90:
    score += 20
elif coverage >= 80:
    score += 18
elif coverage >= 50:
    score += 10
elif coverage >= 20:
    score += 5

# Tidy scoring: 0 warn -> 20, <=10 -> 15, <=30 -> 10, else -> 0
if tidy_warnings == 0:
    score += 20
elif tidy_warnings <= 10:
    score += 15
elif tidy_warnings <= 30:
    score += 10

score = clamp(score)

print(f"QUALITY_SCORE={score}")
if score < 60:
    print("QUALITY_STATUS=needs-work")
else:
    print("QUALITY_STATUS=ok")
