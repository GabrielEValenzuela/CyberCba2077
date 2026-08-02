#!/usr/bin/env bash
# Build and optionally run Cyber CBA using its vcpkg manifest dependencies.
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
project_dir="$(cd "${script_dir}/.." && pwd)"
build_dir="${project_dir}/build"
runtime_config="${project_dir}/config/game.cfg"
run_game=false

usage()
{
    echo "Usage: $0 [--dev] [--run]"
    echo "  --dev  Use config/development.cfg when running."
    echo "  --run  Run cyber-cba after a successful build."
}

while (($# > 0)); do
    case "$1" in
        --dev)
            runtime_config="${project_dir}/config/development.cfg"
            ;;
        --run)
            run_game=true
            ;;
        --help|-h)
            usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
    shift
done

vcpkg_root="${VCPKG_ROOT:-}"
if [[ -z "${vcpkg_root}" ]]; then
    vcpkg_command="$(command -v vcpkg || true)"
    if [[ -z "${vcpkg_command}" ]]; then
        echo "Set VCPKG_ROOT or add vcpkg to PATH before building." >&2
        exit 1
    fi
    vcpkg_root="$(cd "$(dirname "${vcpkg_command}")/.." && pwd)"
fi

toolchain_file="${vcpkg_root}/scripts/buildsystems/vcpkg.cmake"
if [[ ! -f "${toolchain_file}" ]]; then
    echo "vcpkg toolchain not found: ${toolchain_file}" >&2
    exit 1
fi

cmake -S "${project_dir}" -B "${build_dir}" -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_TOOLCHAIN_FILE="${toolchain_file}"
cmake --build "${build_dir}" --target cyber_cba

if [[ "${run_game}" == true ]]; then
    exec "${build_dir}/cyber-cba" --config "${runtime_config}"
fi
