# GBA Template Application

[TOC]

## Intro

This is a small repository that can help get started programming for the game boy advance. 

## Requirements

1. VS code (technically optional but highly recommended)
2. [devkitPro](https://devkitpro.org/wiki/Getting_Started)
3. cmake (install in the msys2 that comes with devkitPro, open a terminal and use pacman to install)

## Usage

Press `ctrl + shift + B` to bring up the build menu. First run `Run cmake` to generate your build files. This only needs to be done once unless the CMakeLists.txt is modified. Then run `Compile GBA Project` to produce a `.gba` file in the `bin/` directory. Run `Clean GBA Project` to do a `make clean` on the project and clean up any stale objects.

Note: if you alter the CMakeLists.txt, you will have to delete the `build/` directory to regenerate them correctly. An unsolved issue results in a broken .gba binary if cmake is run twice. 