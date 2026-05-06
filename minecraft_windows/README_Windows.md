# CraftCPP — Minecraft clone in C++ (Windows Edition)

Un motor de vóxeles inspirado en Minecraft construido con **C++17, OpenGL 3.3, GLFW, GLEW y GLM**.

---

## Requisitos del sistema

| Requisito | Mínimo |
|---|---|
| OS | Windows 10 / 11 (64-bit) |
| GPU | Compatible con OpenGL 3.3 |
| RAM | 4 GB |
| Visual Studio | 2019 o 2022 (con carga de trabajo **"Desarrollo de escritorio con C++"**) |
| CMake | 3.18+ |
| Git | cualquier versión reciente |

---

## ⚡ Instalación rápida (recomendado)

### Paso 1 – Instalar Visual Studio

Descarga [Visual Studio Community 2022](https://visualstudio.microsoft.com/es/vs/community/) y durante la instalación selecciona:

> ✅ **Desarrollo de escritorio con C++**

### Paso 2 – Instalar CMake y Git

- [CMake](https://cmake.org/download/) → marca *"Add CMake to the system PATH"*
- [Git](https://git-scm.com/download/win)

### Paso 3 – Ejecutar el instalador de dependencias

Abre **PowerShell como administrador** en la carpeta del proyecto y ejecuta:

```powershell
powershell -ExecutionPolicy Bypass -File setup_windows.ps1
```

Esto descarga **vcpkg** y compila automáticamente GLFW, GLEW y GLM.  
*(Puede tardar 5–10 minutos la primera vez)*

### Paso 4 – Compilar y ejecutar

```bat
build.bat
```

Al finalizar te preguntará si quieres lanzar el juego directamente.

---

## 🔧 Compilación manual (avanzado)

Si prefieres hacer todo a mano sin los scripts:

```bat
:: 1. Instala vcpkg (solo la primera vez)
git clone https://github.com/microsoft/vcpkg.git --depth=1
cd vcpkg
bootstrap-vcpkg.bat -disableMetrics
vcpkg install glfw3:x64-windows glew:x64-windows glm:x64-windows
vcpkg integrate install
cd ..

:: 2. Configura y compila
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_TOOLCHAIN_FILE=vcpkg\scripts\buildsystems\vcpkg.cmake ^
    -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build . --config Release --parallel
```

El ejecutable quedará en `build\Release\CraftCPP.exe`.

---

## 🎮 Controles

| Tecla / Botón | Acción |
|---|---|
| W A S D | Mover |
| Ratón | Mirar |
| Space | Saltar (modo andando) / Subir (modo vuelo) |
| Left Shift | Bajar (modo vuelo) |
| Left Ctrl | Sprint |
| F | Activar/desactivar modo vuelo |
| 1 – 8 | Seleccionar tipo de bloque |
| Rueda del ratón | Ciclar tipo de bloque |
| Clic izquierdo | Romper bloque |
| Clic derecho | Colocar bloque |
| Clic central | Seleccionar bloque apuntado |
| Esc | Liberar / capturar cursor |
| Q | Salir |

## Hotbar de bloques (teclas 1–8)
`1` Hierba · `2` Tierra · `3` Piedra · `4` Arena · `5` Madera · `6` Hojas · `7` Agua · `8` Roca base

---

## Estructura del proyecto

```
CraftCPP/
├── src/
│   ├── main.cpp        Punto de entrada
│   ├── Game.h/cpp      Bucle principal, input, física, renderizado
│   ├── World.h         Gestor de chunks, raycast DDA, acceso a bloques
│   ├── WorldGen.h      Generación procedural (terreno, cuevas, árboles)
│   ├── Chunk.h/cpp     Datos del chunk, constructor de malla, upload OpenGL
│   ├── Block.h         Tipos de bloque y mapeo de UV en el atlas de texturas
│   ├── Camera.h        Cámara FPS (vista/proyección)
│   ├── Shader.h        Wrapper de shader + fuentes GLSL embebidas
│   └── PerlinNoise.h   Implementación de Perlin noise
├── CMakeLists.txt      Sistema de compilación
├── setup_windows.ps1   Instalador automático de dependencias
└── build.bat           Script de compilación con un clic
```

---

## ❓ Solución de problemas

### "cmake no se reconoce como comando"
→ Reinstala CMake marcando *"Add to PATH"* y reinicia la terminal.

### "No se encontró GLEW / GLFW"
→ Ejecuta `setup_windows.ps1` de nuevo. Asegúrate de que vcpkg terminó sin errores.

### Error de linkado `LNK2019` o `LNK1120`
→ Asegúrate de estar compilando en x64. Revisa que `VCPKG_TARGET_TRIPLET=x64-windows`.

### La ventana se abre y se cierra inmediatamente
→ Ejecuta desde la terminal para ver el error:
```bat
build\Release\CraftCPP.exe
```

### FPS bajo
→ Reduce `renderDist` en `Game.cpp` (busca `world->renderDist`, por defecto es 8, prueba con 4 o 5).

---

## Dependencias

| Librería | Versión | Uso |
|---|---|---|
| GLFW | 3.x | Ventana e input |
| GLEW | 2.x | Cargador de extensiones OpenGL |
| GLM | 0.9.9+ | Matemáticas (vectores, matrices) |
| OpenGL | 3.3 core | Renderizado |
