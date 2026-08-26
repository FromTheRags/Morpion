# Morpion (Tic-Tac-Toe) - SDL 1.2 C++ Game

[![Build & Package](https://github.com/FromTheRags/morpion/actions/workflows/build.yml/badge.svg)](https://github.com/FromTheRags/morpion/actions/workflows/build.yml)
[![License: GPL-3.0-or-later](https://img.shields.io/badge/License-GPL%203.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![SDL](https://img.shields.io/badge/Library-SDL%201.2-green.svg)](https://www.libsdl.org/)

A nostalgic French **Tic-Tac-Toe (Morpion)** game built in **2013** by **FromTheRags** as a junior programming project using C++, Code::Blocks, SDL 1.2, and SDL_ttf.

This repository is preserved and open-sourced under the **GNU General Public License v3.0 (GPLv3)** for historical, educational, and nostalgic purposes, updated with modern cross-platform CMake builds, Inno Setup packaging, CI/CD automation, and original Japanese studio (Studio Ghibli / Sumi-e) aesthetic assets.

---

## 🎮 Features

- **Classic 3x3 Gameplay**: Play as Circles (O) against the AI Crosses (X) on an antique Japanese cedar wood board.
- **4 AI Difficulty Levels**:
  1. **Vraiment Facile** (*Very Easy*): Plays in the edge positions and makes mistakes.
  2. **Normal**: Plays random valid moves across the board.
  3. **Difficile** (*Hard*): Plays the center, blocks winning player moves, and completes winning lines.
  4. **Impossible**: Employs strategic diagonal and edge counters — virtually unbeatable!
- **End-Game Banners**: Colorful celebratory text for **VICTOIRE**, **DÉFAITE**, and **MATCH NUL**.
- **Japanese Studio Art Aesthetic**:
  - **Menu Screen (`nice.bmp`)**: Atmospheric Studio Ghibli-inspired Japanese countryside landscape at twilight with a cozy wooden teahouse, warm glowing lanterns, and distant Mount Fuji.
  - **Game Board (`grille.bmp`)**: Warm antique Japanese cedar tabletop with carved groove lines.
  - **Tokens (`croix.bmp`, `rond.bmp`)**: Authentic Japanese *Sumi-e* ink brush cross (Crimson red) and Zen *Enso* circle (Deep indigo blue).
  - **Typography**: [Patrick Hand](https://fonts.google.com/specimen/Patrick+Hand) (Handwriting) & [Graduate](https://fonts.google.com/specimen/Graduate) (Display), both licensed under SIL OFL 1.1.

---

## 🖼️ Game Assets Overview

All game images have been custom-crafted in a cohesive Japanese studio aesthetic and released under the GPLv3 license:
- `grille.bmp`: 760x757 carved cedar game board.
- `nice.bmp`: 760x757 twilight countryside menu screen.
- `croix.bmp`: 212x212 Sumi-e ink brush cross (X).
- `rond.bmp`: 212x212 Zen Enso ink circle (O).
- `sdl_icone.bmp` & `morpion.ico`: Application window and executable icons.
- `PatrickHand-Regular.ttf` & `Graduate-Regular.ttf`: Open-source SIL OFL fonts.

---

## 📜 About MinGW (Minimalist GNU for Windows)

### 💡 Background & History

**MinGW** (*Minimalist GNU for Windows*) is a legendary open-source development suite that ports the GNU Compiler Collection (GCC), GNU Binutils, and GNU Debugger (GDB) natively to Microsoft Windows.

- **Why MinGW Matters**: Unlike Cygwin (which requires a POSIX emulation DLL that translates Linux syscalls to Windows), MinGW links directly against the native Microsoft C Runtime (`msvcrt.dll` or modern `UCRT`). This means executables built with MinGW run as native, lightweight Windows applications without any extra runtime overhead.
- **The Golden Era of Junior Game Dev (2000s–2010s)**: In 2013 when this game was created, the **Code::Blocks + MinGW bundle** was the quintessential toolkit for young programmers learning C and C++ in France and worldwide (popularized by platforms like *Le Site du Zéro / OpenClassrooms*). It allowed high schoolers and students to compile SDL 1.2 games without complex Visual Studio configurations.
- **Modern MinGW-w64 & MSYS2**: Today, the original 32-bit MinGW has evolved into the modern **MinGW-w64** project and **MSYS2** ecosystem, supporting full 64-bit Windows, modern C++17/20/23 standards, POSIX threads, and rich package management via `pacman`.

---

## 🛠️ How to Get MinGW on Windows

There are several straightforward ways to install a modern MinGW-w64 environment on Windows:

### Method 1: Via MSYS2 (Recommended)

[MSYS2](https://www.msys2.org/) provides an Arch Linux-style package manager (`pacman`) with up-to-date MinGW-w64 toolchains and prebuilt SDL libraries.

1. **Download & Install**: Download the installer from [msys2.org](https://www.msys2.org/) and run it.
2. **Open the MSYS2 MINGW64 Terminal** (from the Start Menu).
3. **Install GCC, CMake, Ninja, SDL, and SDL_ttf**:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-SDL mingw-w64-x86_64-SDL_ttf
   ```
4. **Add to PATH** (Optional): Add `C:\msys64\mingw64\bin` to your Windows System `PATH` environment variable to use `g++`, `gcc`, and `cmake` directly in PowerShell or Command Prompt.

---

### Method 2: Via WinLibs (Standalone Portable ZIP)

[WinLibs](https://winlibs.com/) provides standalone, precompiled, zero-install GCC/MinGW-w64 bundles with POSIX threads, LLVM/Clang, and UCRT/MSVCRT runtimes.

1. Go to [winlibs.com](https://winlibs.com/) and download the latest **GCC + MinGW-w64** zip archive (e.g. *Win64, UCRT, POSIX threads*).
2. Extract the archive (e.g. to `C:\mingw64`).
3. Add `C:\mingw64\bin` to your Windows `PATH` variable.
4. Verify by running `g++ --version` in PowerShell or CMD.

---

### Method 3: Via Package Managers (Winget / Chocolatey)

#### Using Windows Package Manager (`winget`):
```powershell
winget install MSYS2.MSYS2
```

#### Using Chocolatey (`choco`):
```powershell
choco install msys2 mingw
```

---

## 🔨 Building Locally

### Prerequisites

- A C++ compiler (supporting C++17)
- CMake (>= 3.16)
- SDL 1.2 & SDL_ttf development libraries

#### Linux (Ubuntu / Debian):
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libsdl1.2-dev libsdl-ttf2.0-dev
```

#### Linux (Fedora):
```bash
sudo dnf install gcc-c++ cmake SDL-devel SDL_ttf-devel
```

#### macOS (Homebrew):
```bash
brew install cmake sdl sdl_ttf
```

#### Windows (MSYS2 MinGW-w64):
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-SDL mingw-w64-x86_64-SDL_ttf
```

---

### Compilation with CMake

```bash
# 1. Configure the build
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 2. Compile the project
cmake --build build --config Release

# 3. Run the game
# (Assets are automatically copied to the build output directory)
./build/morpion       # On Linux / macOS
.\build\morpion.exe   # On Windows
```

---

## 📦 Building the Windows Installer (Inno Setup)

An Inno Setup script [`morpion script.iss`](morpion%20script.iss) is included to generate a standalone Windows installer.

To compile the installer locally with [Inno Setup 6](https://jrsoftware.org/isinfo.php):
```cmd
# Ensure morpion.exe is present (e.g. copied from build/)
copy build\morpion.exe .\

# Compile the installer
iscc "morpion script.iss"
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
  - `PatrickHand-Regular.ttf` ([Patrick Hand](https://fonts.google.com/specimen/Patrick+Hand)) by Patrick Wagesreiter — Licensed under [SIL Open Font License 1.1](OFL.txt).
  - `Graduate-Regular.ttf` ([Graduate](https://fonts.google.com/specimen/Graduate)) by Eduardo Tunni — Licensed under [SIL Open Font License 1.1](OFL.txt).

---

## 💡 Historical Note

> *« Ce programme a été créé par : FromTheRags »*
>
> Originally written in April–November 2013 using Code::Blocks IDE and MinGW on Windows. This open-source release preserves the original logic and spirit while making it accessible, portable, and clean for modern systems.
