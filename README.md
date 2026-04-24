# YAcrackmes

Yet Another Crackmes — a collection of Windows crackme programs written in C++23.

## Requirements

- Windows (x64)
- [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) with the **MSVC** C++ toolchain
- [CMake](https://cmake.org/) ≥ 3.28
- [Ninja](https://ninja-build.org/)

## Building

Open a **Developer Command Prompt for VS** (so that `cl.exe` and `ninja` are on PATH), then:

```bat
# Debug build
cmake --preset msvc-ninja-debug
cmake --build --preset build-debug

# Release build
cmake --preset msvc-ninja-release
cmake --build --preset build-release
```

Executables are written to `build/debug/` or `build/release/` respectively.

## Project Structure

```
YAcrackmes/
├── CMakeLists.txt           # Root build configuration
├── CMakePresets.json        # MSVC + Ninja build presets
├── crackmes/
│   └── c01_helloworld/      # Placeholder crackme
│       ├── CMakeLists.txt
│       └── main.cpp
└── README.md
```

## Adding a New Crackme

Follow these steps to add a new crackme (e.g., `c02_reverse`):

1. **Create a directory** under `crackmes/`:

   ```
   crackmes/
   └── c02_reverse/
       ├── CMakeLists.txt
       └── main.cpp
   ```

2. **Write the crackme's `CMakeLists.txt`** — use the existing one as a template:

   ```cmake
   add_executable(c02_reverse main.cpp)
   target_compile_features(c02_reverse PRIVATE cxx_std_23)
   set_target_properties(c02_reverse PROPERTIES OUTPUT_NAME "c02_reverse")
   ```

3. **Register it in the root `CMakeLists.txt`** by appending one line inside the
   *Crackme targets* section:

   ```cmake
   add_subdirectory(crackmes/c02_reverse)
   ```

4. Rebuild — the new target will be picked up automatically by CMake.

## Naming Convention

| Prefix | Meaning                  |
|--------|--------------------------|
| `c01_` | First crackme (serial)   |
| `c02_` | Second crackme, and so on|

Use a short, descriptive suffix after the number (e.g., `helloworld`, `serial`, `keygen`).
