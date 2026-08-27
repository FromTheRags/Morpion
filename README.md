# Morpion (Tic-Tac-Toe) - SDL 1.2 C++ Game

[![Build & Package](https://github.com/FromTheRags/morpion/actions/workflows/build.yml/badge.svg)](https://github.com/FromTheRags/morpion/actions/workflows/build.yml)
[![License: GPL-3.0-or-later](https://img.shields.io/badge/License-GPL%203.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![SDL](https://img.shields.io/badge/Library-SDL%201.2-green.svg)](https://www.libsdl.org/)

A nostalgic French **Tic-Tac-Toe (Morpion)** game built in **2013** by **FromTheRags** as a junior personal programming project using C++, Code::Blocks, SDL 1.2, and SDL_ttf.
Of course as the writer was yet learning at that time the code is very simple and annotated in french.
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

## 🚀 Building in CLion (JetBrains IDE)

[CLion](https://www.jetbrains.com/clion/) supports CMake natively. Because official Windows development libraries are bundled in `cmake/sdl12/`, **no external package manager is required on Windows!**

### Option A: CLion with Visual Studio Toolchain (Windows)

1. Open CLion &rarr; **File &rarr; Open** &rarr; Select the `Morpion` folder.
2. In CLion, open **Settings (Ctrl+Alt+S) &rarr; Build, Execution, Deployment &rarr; Toolchains**:
   - Add or select the **Visual Studio** toolchain (pointing to your Visual Studio 2022 installation).
3. In **Settings &rarr; Build, Execution, Deployment &rarr; CMake**:
   - Ensure the architecture is set to **x64** (or default).
4. Click the **Reload CMake Project** button if needed.
5. Click the green **Run ▶ (`morpion`)** button to build and launch the game!

---

### Option B: CLion with WSL (Linux Subsystem on Windows)

1. In your WSL terminal (Ubuntu / Debian):
   ```bash
   sudo apt-get update
   sudo apt-get install -y build-essential cmake libsdl1.2-dev libsdl-ttf2.0-dev
   ```
2. In CLion: **Settings &rarr; Build, Execution, Deployment &rarr; Toolchains** &rarr; Add **WSL** (set as default).
3. Click **Run ▶ (`morpion`)**.

---

### Option C: CLion on Linux or macOS

1. Install dependencies:
   - **Ubuntu/Debian**: `sudo apt install -y build-essential cmake libsdl1.2-dev libsdl-ttf2.0-dev`
   - **macOS**: `brew install cmake sdl sdl_ttf`
2. Open the `Morpion` folder in CLion and click **Run ▶**.

---

## 🔨 Building from Terminal

### Windows (Visual Studio / CMake - Zero Config)

```powershell
# Configure (x64) - automatically detects bundled SDL 1.2 & SDL_ttf
cmake -B build -S . -A x64

# Compile Release
cmake --build build --config Release

# Run!
.\build\Release\morpion.exe
```

---

### Linux (Ubuntu / Debian / Fedora)

```bash
# Ubuntu / Debian
sudo apt-get update
sudo apt-get install -y build-essential cmake libsdl1.2-dev libsdl-ttf2.0-dev

# Fedora
sudo dnf install -y gcc-c++ cmake SDL-devel SDL_ttf-devel

# Build & Run
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/morpion
```

---

### macOS (Homebrew)

```bash
brew install cmake sdl sdl_ttf
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/morpion
```

---

## 📜 About MinGW (Minimalist GNU for Windows)

### 💡 Background & History

**MinGW** (*Minimalist GNU for Windows*) is an open-source suite that ports the GNU Compiler Collection (GCC) and GNU Binutils natively to Microsoft Windows.

- **Direct Native Runtime**: Unlike Cygwin (which requires a POSIX emulation DLL), MinGW links directly against the native Microsoft C Runtime (`msvcrt.dll` or modern `UCRT`). Executables run as native, lightweight Windows applications with zero emulation overhead.
- **The 2013 Era**: When this project was built in 2013, the **Code::Blocks + MinGW bundle** was the quintessential toolkit for students and beginners learning C/C++ in France and worldwide (popularized by *Le Site du Zéro / OpenClassrooms*).
- **Evolution to MinGW-w64 & MSYS2**: Today, the project has evolved into **MinGW-w64** and **MSYS2**, providing 64-bit support, modern C++17/20 standards, and package management.

---

## 📦 Building the Windows Installer (Inno Setup)

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
