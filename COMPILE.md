# Compilación

Seguí primero [INSTALL.md](INSTALL.md). Todos los comandos de esta guía se
ejecutan desde la raíz del repositorio.

## Configuración de desarrollo

Indicá dónde instalaste vcpkg y configurá el preset de desarrollo:

```bash
export VCPKG_ROOT="$HOME/vcpkg"
cmake --preset dev
```

La primera configuración descarga y compila las dependencias declaradas en
`vcpkg.json`. Las configuraciones siguientes reutilizan la caché de vcpkg.

## Compilar, probar y ejecutar

```bash
cmake --build --preset dev
ctest --test-dir build/dev --output-on-failure
./build/dev/src/app/cybercba_app
```

Ejecutá el binario desde la raíz del repositorio: así puede encontrar la
carpeta `credits/` que utiliza la pantalla de créditos.

## Presets disponibles

| Preset | Propósito |
| --- | --- |
| `dev` | Desarrollo con símbolos de depuración y pruebas. |
| `debug` | Depuración sin la configuración específica de pruebas. |
| `asan` | Diagnóstico con AddressSanitizer y UndefinedBehaviorSanitizer; requiere Clang. |
| `release` | Binario optimizado sin pruebas. |
| `ci` | Compilación estricta con advertencias como errores y clang-tidy; requiere Clang y clang-tidy. |

Para cualquiera de ellos, reemplazá `dev` en los comandos por el nombre del
preset. Por ejemplo:

```bash
cmake --preset asan
cmake --build --preset asan
ctest --preset asan
```

## Generar la referencia de API

Con Doxygen instalado:

```bash
cmake --build build/dev --target docs
```

La documentación HTML se escribe en `build/docs/html/index.html`. También se
puede generarla sin configurar CMake:

```bash
doxygen Doxyfile
```

## Problemas frecuentes

- **`VCPKG_ROOT` no está definido:** exportalo en la misma terminal antes de
  ejecutar `cmake --preset ...`.
- **No encuentra X11, OpenGL o ALSA:** confirmá que instalaste los paquetes de
  desarrollo listados en [INSTALL.md](INSTALL.md).
- **Se cambió el compilador o el preset:** eliminá únicamente el directorio de
  build del preset afectado (por ejemplo, `build/asan`) y configurá de nuevo.
