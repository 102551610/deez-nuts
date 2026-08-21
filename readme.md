# Deez Nuts

Deez Nuts is a Windows console application for working with Texas Hold'em
poker ranges. It can generate or assign boards, calculate range combinations,
print range data, save and load results, and inspect improvement probabilities.

## Requirements

- Windows
- GCC from MSYS2 UCRT64, available as `gcc` in PowerShell or a similar shell

The source uses the Windows console API through `<windows.h>`.

## Build

Run these commands from the project directory:

```powershell
gcc -std=c11 main.c improve.c struct_range.c windows_gui.c -o deez-nuts.exe
```

Do not build only the active file. `main.c` contains `main()`, while the other
source files provide the range, probability, and console display functions.

## Run

```powershell
.\deez-nuts.exe
```

The application opens an interactive console menu:

| Key | Action |
| --- | --- |
| `r` | Randomise cards |
| `a` | Assign cards manually |
| `t` | Run tests |
| `l` | Load cards from a file when no range is loaded |
| `p` | Print the current range |
| `w` | Print raw range data |
| `x` | Expand raw range data |
| `s` | Save the current range |
| `d` | Delete the current range |
| `b` | Rebuild boxed cards |
| `m` | Modify boxed cards |
| `q` | Quit |

Some options appear only after a range has been created or loaded. Follow the
prompts in the console for board size, boxed cards, and test parameters.

## Source Overview

- `main.c` - application entry point, menu loop, card input, randomisation,
	save/load flow, and test commands.
- `struct_range.h` / `struct_range.c` - `Range` nodes and range construction,
	sorting, hand evaluation, and cleanup.
- `struct_frac.h` - `Frac` values used for improvement calculations.
- `improve.h` / `improve.c` - greatest-common-divisor, least-common-multiple,
	and range improvement calculations.
- `windows_gui.h` / `windows_gui.c` - Windows console colours and formatted
	card, range, raw-data, and histogram output.
- `history.txt` - saved card data used by the application's load/save features.

## VS Code

The generated **Build Active File** task compiles only the file currently open.
Use the command in the Build section, or update the task so it includes
`main.c`, `improve.c`, `struct_range.c`, and `windows_gui.c`.
