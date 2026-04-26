# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Dungeon Explorer game in C — university project (Université de Toulouse, spring 2025-2026). Four incremental levels: inventory/player/room/dungeon (lvl 0), game loop + exploration (lvl 1), enemy combat (lvl 2), binary save + DFS treasure search (lvl 3).

## Build commands

All source files live at the repo root. The Makefile must define these targets:

```bash
make                  # builds main executable: donjon
make test_inventaire  # builds inventory unit test
make test_donjon      # builds dungeon + UI display test
```

Required `CFLAGS`: `-Wall -DNIV01` for levels 0–1. Drop `-DNIV01` when implementing level 2 (enemy support in ui.c is gated behind `#ifndef NIV01`).

Memory debugging:
```bash
valgrind ./donjon donjon32.txt          # Linux
leaks --atExit -- ./donjon donjon32.txt # macOS
gcc -fsanitize=address ...              # address sanitizer
```

Run the game:
```bash
./donjon donjon32.txt   # or donjon16.txt / donjon64.txt
```

## Architecture

### Module encapsulation pattern

Every module exposes an **opaque pointer type** (`typedef struct sXxx *tXxx`). The struct definition lives only in the `.c` file. The `.h` contains only the typedef and function declarations. **Never put struct fields in `.h` files.**

### Module dependency order

```
inventaire  ←  joueur
                 ↑
salle  ←  donjon  ←  jeu  ←  main
                 ↑
              ennemi (level 2)
```

`ui.h` depends on `donjon.h` and `joueur.h`, and also on `ennemi.h` / `salle.h` at level 2 (guarded by `#ifndef NIV01`).

### Coordinate system

The dungeon is stored as a 1D array of size `w*h`. Cell at `(x, y)` is at index `y*w + x`. `(0,0)` is top-left; x is the column, y is the row. North = y-1, South = y+1, West = x-1, East = x+1.

### Key invariants

- `InventaireVider` leaves the `sInventaire` struct alive (only frees items); `InventaireLiberer` frees the struct itself and sets the pointer to NULL — same pattern for all `Liberer` functions.
- `InventaireVersChaine` allocates and returns a string the **caller must `free`**. `UI_LireLigne` does the same.
- A "mur" salle has `existe=0`; its `desc` and `objets` fields are NULL — `SalleLiberer` must handle both cases.
- `SalleCreerVide` default description: `"Une salle sans particularite."`
- Player default stats: `pv=20, pvMax=20, atk=5, def=1`.
- Combat damage formula: `degats = max(1, attaquant_atk - defenseur_def)`. PV floor is 0.

### Dungeon file format

```
<w> <h>
<grid lines using '#' '.' '@'>
DESC <x> <y> <description>
ITEM <x> <y> <nom> <quantite>
ENEMY <x> <y> <nom> <pv> <atk> <def>   ← level 2 only
```

`@` marks the player start position (room must be marked visited). Unknown directive lines are silently skipped.

### Provided files (do not modify)

- `ui.h`, `ui.c` — stdio UI implementation
- `ui_nc.c` — ncurses UI (link with `-lncurses`)
- `donjon16.txt`, `donjon32.txt`, `donjon64.txt` — example dungeons

### Submission

Zip only `.h`, `.c`, `Makefile`, and `niveau-n.txt` (where n is the highest completed level). No binaries, no `.o`, no extra files.
