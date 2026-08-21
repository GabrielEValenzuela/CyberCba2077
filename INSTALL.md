# Instalación en Ubuntu

Esta guía prepara una estación de desarrollo Ubuntu para CyberCBA 2077. El
proyecto usa C++17, CMake, Ninja y vcpkg en modo *manifest*. Las bibliotecas
del juego y de pruebas se resuelven automáticamente desde `vcpkg.json`.

## 1. Instalar paquetes del sistema

En Ubuntu 22.04 o posterior, instalá las herramientas de compilación, las
bibliotecas necesarias para crear una ventana raylib y las herramientas
opcionales de calidad/documentación:

```bash
sudo apt update
sudo apt install build-essential clang clang-tidy cmake ninja-build git curl zip unzip tar pkg-config \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
    libgl1-mesa-dev libglu1-mesa-dev libasound2-dev libwayland-dev \
    libxkbcommon-dev doxygen graphviz
```

`clang` y `clang-tidy` se usan en los presets `asan` y `ci`; `doxygen` y
`graphviz` solo son necesarios para generar la referencia de API y sus
diagramas.

## 2. Instalar vcpkg

Cloná vcpkg fuera del repositorio y fijalo al baseline que usa el proyecto:

```bash
git clone https://github.com/microsoft/vcpkg.git "$HOME/vcpkg"
cd "$HOME/vcpkg"
git checkout d034cf165b78579b6d526cf440d2781940e9074b
./bootstrap-vcpkg.sh
```

Podés ubicarlo en otra carpeta; en ese caso usá esa ruta al definir
`VCPKG_ROOT` en la guía de compilación. No instales `raylib` ni GoogleTest a
mano: CMake los instala de forma reproducible al configurar el proyecto.

## 3. Verificar herramientas

```bash
cmake --version
ninja --version
"$HOME/vcpkg/vcpkg" version
doxygen --version
```

Continuá con [COMPILE.md](COMPILE.md) para configurar, compilar y ejecutar el
proyecto.
