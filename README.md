# Morpion (Tic-Tac-Toe) - SDL 1.2 C/C++ Game

[![Build & Package](https://github.com/FromTheRags/Morpion/actions/workflows/build.yml/badge.svg)](https://github.com/FromTheRags/Morpion/actions/workflows/build.yml)
[![License: GPL-3.0-or-later](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)
[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)](https://isocpp.org/)
[![Library: SDL 1.2](https://img.shields.io/badge/Library-SDL%201.2-green.svg)](https://www.libsdl.org/)
[![CMake](https://img.shields.io/badge/Build-CMake-064F8C.svg?logo=cmake&logoColor=white)](https://cmake.org/)

A nostalgic French **Tic-Tac-Toe (Morpion)** game built in **2013** as a junior personal programming project using C/C++, Code::Blocks, SDL 1.2, and SDL_ttf.
Of course as I was yet learning at that time the code is very simple and annotated in french.
I voluntary keep the code as it, without upgrading it, except for the need upgrade for builchain.

This repository is preserved and open-sourced under the **GNU General Public License v3.0 (GPLv3)** for historical, educational, and nostalgic purposes, updated with modern cross-platform CMake builds, Inno Setup packaging, CI/CD automation, and original aesthetic assets.

---

## 🎮 Features

- **Classic 3x3 Gameplay**: Play as Circles (O) against the AI Crosses (X) on an antique Japanese cedar wood board.
- **4 AI Difficulty Levels**:
  1. **Vraiment Facile** (*Very Easy*): Plays in the edge positions and makes mistakes.
  2. **Normal**: Plays random valid moves across the board.
  3. **Difficile** (*Hard*): Plays the center, blocks winning player moves, and completes winning lines.
  4. **Impossible**: Employs strategic diagonal and edge counters — virtually unbeatable!
- **End-Game Banners**: Colorful celebratory text for **VICTOIRE**, **DÉFAITE**, and **MATCH NUL** with dynamic spacing and automatic horizontal centering.
- **Dynamic Fullscreen & Resizing**: Seamless fullscreen toggle with **F11**, **Alt+Enter**, or **F**, with aspect-ratio preserved scaling.
- **Japanese Studio Art Aesthetic**:
  - **Menu Screen (`assets/nice.bmp`)**: Atmospheric Studio Ghibli-inspired Japanese countryside landscape at twilight with a cozy wooden teahouse, warm glowing lanterns, and distant Mount Fuji.
  - **Game Board (`assets/grille.bmp`)**: Warm antique Japanese cedar tabletop with carved groove lines.
  - **Tokens (`assets/croix.bmp`, `assets/rond.bmp`)**: Authentic Japanese *Sumi-e* ink brush cross (Crimson red) and Zen *Enso* circle (Deep indigo blue).
  - **Typography**: [Patrick Hand](https://fonts.google.com/specimen/Patrick+Hand) (Handwriting) & [Graduate](https://fonts.google.com/specimen/Graduate) (Display), both licensed under SIL OFL 1.1.

---

## 🖼️ Game Assets Overview

All game images have been custom-crafted in a cohesive Japanese studio aesthetic and released under the GPLv3 license:
- `assets/grille.bmp`: 760x757 carved cedar game board.
- `assets/nice.bmp`: 760x757 twilight countryside menu screen.
- `assets/croix.bmp`: 212x212 Sumi-e ink brush cross (X).
- `assets/rond.bmp`: 212x212 Zen Enso ink circle (O).
- `assets/sdl_icone.bmp` & `assets/morpion.ico`: Textless application window and executable icons.
- `assets/PatrickHand-Regular.ttf` & `assets/Graduate-Regular.ttf`: Open-source SIL OFL fonts.

---

## 🐧 Building on Linux

### 1. Install Dependencies

Install CMake, a C++ compiler, and the SDL 1.2 development libraries:

- **Ubuntu / Debian / Linux Mint**:
  ```bash
  sudo apt-get update
  sudo apt-get install -y build-essential cmake libsdl1.2-dev libsdl-ttf2.0-dev
  ```
- **Fedora / RHEL**:
  ```bash
  sudo dnf install -y gcc-c++ cmake SDL-devel SDL_ttf-devel
  ```
- **Arch Linux / Manjaro**:
  ```bash
  sudo pacman -S base-devel cmake sdl12-compat sdl_ttf
  ```

### Option A: Building from Terminal

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run!
./build/morpion
```

### Option B: Building in CLion (JetBrains IDE)

1. Open CLion &rarr; **File &rarr; Open** &rarr; Select the `Morpion` folder.
2. CLion detects `CMakeLists.txt` automatically.
3. Click the green **Run ▶ (`morpion`)** button to build and launch the game.

---

## 🍏 Building on macOS

### 1. Install Dependencies

Install CMake and SDL 1.2 development libraries using [Homebrew](https://brew.sh/):

```bash
brew install cmake sdl sdl_ttf
```

### Option A: Building from Terminal

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run!
./build/morpion
```

### Option B: Building in CLion (JetBrains IDE)

1. Open CLion &rarr; **File &rarr; Open** &rarr; Select the `Morpion` folder.
2. CLion detects CMake and the Homebrew toolchain automatically.
3. Click the green **Run ▶ (`morpion`)** button to build and launch the game.

---

## 🪟 Building on Windows

### 🛠️ Windows Prerequisites (CMake & C++ Compilers)

Before building from the **Terminal** or with **CLion**, ensure that you have **CMake** and a **C++ Compiler Toolchain** (Visual Studio MSVC or MinGW-w64) installed.

> [!NOTE]
> Official Windows development headers and libraries for SDL 1.2 and SDL_ttf are bundled in `cmake/sdl12/`. **No external SDL package manager or manual SDL library download is required on Windows!**

#### 1. Install CMake

CMake configures and generates native build files for your compiler of choice (Visual Studio, Ninja, MinGW Makefiles, etc.).

- **Direct Download (Recommended)**:
  1. Download the latest Windows x64 Installer (`.msi` or `.exe`) from [cmake.org/download](https://cmake.org/download/).
  2. Run the installer and select **"Add CMake to the system PATH for all users"** (or for the current user).
- **Via Windows Package Manager (Winget)**:
  ```powershell
  winget install --id Kitware.CMake
  ```
- **Verify installation**:
  ```powershell
  cmake --version
  ```

#### 2. Install a C++ Compiler (Visual Studio or MinGW-w64)

Choose one of the two main C++ toolchains for Windows:

##### Option A: Visual Studio 2022 / MSVC (*Recommended*)
1. Download **Visual Studio 2022 Community** (Free) or **Build Tools for Visual Studio 2022** from [visualstudio.microsoft.com/downloads](https://visualstudio.microsoft.com/downloads/).
   - Or install via `winget`:
     ```powershell
     winget install Microsoft.VisualStudio.2022.Community --override "--add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended"
     ```
2. In the Visual Studio Installer, select the workload **Desktop development with C++** (*Développement Desktop en C++*).
3. This installs Microsoft's C++ compiler (`cl.exe`), Windows SDK, and build utilities.

##### Option B: MinGW-w64 (GCC for Windows)
- **Method 1: WinLibs (Standalone Portable Archive - Quickest)**:
  1. Download the latest UCRT / POSIX x86_64 release from [winlibs.com](https://winlibs.com/).
  2. Extract the archive to `C:\mingw64`.
  3. Add `C:\mingw64\bin` to your Windows `PATH` environment variable.
- **Method 2: MSYS2 (Package Manager)**:
  1. Download and install [MSYS2](https://www.msys2.org/) (or run `winget install MSYS2.MSYS2`).
  2. Open the **MSYS2 MINGW64** shell and install the toolchain:
     ```bash
     pacman -Syu
     pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-make
     ```
  3. Add `C:\msys64\mingw64\bin` to your Windows system `PATH`.
- **Verify compiler**:
  ```powershell
  gcc --version
  # or
  g++ --version
  ```

---

### 🔨 1. Building from Terminal

#### Method A: Using Visual Studio / MSVC (Default Windows Generator)

```powershell
# Configure (x64) - automatically detects bundled SDL 1.2 & SDL_ttf
cmake -B build -S . -A x64

# Compile Release
cmake --build build --config Release

# Run!
.\build\Release\morpion.exe
```

#### Method B: Using MinGW-w64 / GCC

```powershell
# Configure with Ninja (or "MinGW Makefiles")
cmake -B build -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Release
# Or: cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# Compile Release
cmake --build build

# Run!
.\build\morpion.exe
```

---

### 🚀 2. Building in CLion (JetBrains IDE)

[CLion](https://www.jetbrains.com/clion/) integrates CMake natively (and comes with bundled CMake).

#### Option A: CLion with Visual Studio Toolchain

1. Open CLion &rarr; **File &rarr; Open** &rarr; Select the `Morpion` folder.
2. Open **Settings (Ctrl+Alt+S) &rarr; Build, Execution, Deployment &rarr; Toolchains**:
   - Select or add **Visual Studio** (CLion automatically detects your VS 2022 installation).
3. In **Settings &rarr; Build, Execution, Deployment &rarr; CMake**:
   - Set Architecture to **x64** (or default).
4. Click **Reload CMake Project** if prompted.
5. Click the green **Run ▶ (`morpion`)** button to build and launch the game!

#### Option B: CLion with MinGW Toolchain

1. Open CLion &rarr; **File &rarr; Open** &rarr; Select the `Morpion` folder.
2. In **Settings (Ctrl+Alt+S) &rarr; Build, Execution, Deployment &rarr; Toolchains**:
   - Add a **MinGW** toolchain and set the Environment path to `C:\mingw64` (or `C:\msys64\mingw64`).
3. Click the green **Run ▶ (`morpion`)** button!

#### Option C: CLion with WSL (Linux Subsystem on Windows)

1. In your WSL terminal (Ubuntu / Debian):
   ```bash
   sudo apt-get update
   sudo apt-get install -y build-essential cmake libsdl1.2-dev libsdl-ttf2.0-dev
   ```
2. In CLion: **Settings &rarr; Build, Execution, Deployment &rarr; Toolchains** &rarr; Add **WSL** (set as default).
3. Click **Run ▶ (`morpion`)**.

---

### 📦 3. Building the Windows Installer (Inno Setup)

An Inno Setup script [`innosetup/morpion script.iss`](innosetup/morpion%20script.iss) is included to generate a standalone Windows installer.

To compile the installer locally with [Inno Setup 6](https://jrsoftware.org/isinfo.php):
```cmd
# Copy morpion.exe next to the script
copy build\Release\morpion.exe .\

# Compile the installer
iscc "innosetup/morpion script.iss"
```
The resulting installer executable `morpion-setup-v1.5.exe` will be generated in the `dist/` directory.

---

## 🚀 GitHub Actions CI/CD

Continuous Integration is set up via [GitHub Actions](.github/workflows/build.yml):
- **Linux (`ubuntu-latest`)**: Compiles with GCC/Ninja, packages binary and assets into a `.tar.gz` archive, and uploads build artifacts.
- **Windows (`windows-latest`)**: Compiles with MSYS2 MinGW-w64, generates both a portable `.zip` bundle and a full Windows installer (`.exe`) via Inno Setup, and uploads them as build artifacts.

---

## 📜 Copyright & Licensing

- **Code & Artwork**: Copyright (C) 2013 FromTheRags. Released under the [GNU General Public License v3.0 (GPLv3)](LICENSE).
- **Fonts**:
  - `assets/PatrickHand-Regular.ttf` ([Patrick Hand](https://fonts.google.com/specimen/Patrick+Hand)) by Patrick Wagesreiter — Licensed under [SIL Open Font License 1.1](OFL.txt).
  - `assets/Graduate-Regular.ttf` ([Graduate](https://fonts.google.com/specimen/Graduate)) by Eduardo Tunni — Licensed under [SIL Open Font License 1.1](OFL.txt).

---

## 💡 Historical Note

> *« Ce programme a été créé par : FromTheRags »*
>
> Originally written in April–November 2013 using Code::Blocks IDE and MinGW on Windows. This open-source release preserves the original logic and spirit while making it accessible, portable, and clean for modern systems.
