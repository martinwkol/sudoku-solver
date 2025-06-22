

# Sudoku Solver

A simple C program that solves standard 9x9 Sudoku puzzles. It reads the puzzle from standard input, solves it, and prints the result to standard output.

## Files

- `main.c`: Program entry point, handles input/output and command-line arguments
- `sudoku.c`: Core Sudoku solving logic
- `sudoku.h`: Header for the Sudoku solver
- `inputs/`: Contains example puzzles (`i1.txt`, `i2.txt`, `i3.txt`)

## Build and Run

Compile the program using `gcc`. Use the `-DNDEBUG` flag to disable assertions (recommended for release builds):

```bash
gcc -DNDEBUG -O3 -o sudoku main.c sudoku.c
````

Then run it with:

```bash
./sudoku [options] < input.txt
```

### Options

* `-s`
  Use **simple output** format (same as input format).

* `-i`
  Also print the **input puzzle** before the solution.

You can combine options, e.g. `./sudoku -i -s < inputs/i1.txt`.

## Input Format

* 9 lines, each with 9 digits (0–9)
* `0` represents an **empty cell**
* Any non-digit characters are ignored

### Example Input

```
361025900
080960010
400000057
008000471
000603000
259000800
740000005
020018060
005470329
```

## Output

By default, the program prints the solved puzzle in a **pretty format** with borders. If `-s` is used, the output will match the input format (just the numbers).

### Example Output
For the example input above, the pretty output be as follows:
```
++---+---+---++---+---+---++---+---+---++
++---+---+---++---+---+---++---+---+---++
|| 3 | 6 | 1 || 7 | 2 | 5 || 9 | 4 | 8 ||
++---+---+---++---+---+---++---+---+---++
|| 5 | 8 | 7 || 9 | 6 | 4 || 2 | 1 | 3 ||
++---+---+---++---+---+---++---+---+---++
|| 4 | 9 | 2 || 8 | 3 | 1 || 6 | 5 | 7 ||
++---+---+---++---+---+---++---+---+---++
++---+---+---++---+---+---++---+---+---++
|| 6 | 3 | 8 || 2 | 5 | 9 || 4 | 7 | 1 ||
++---+---+---++---+---+---++---+---+---++
|| 1 | 7 | 4 || 6 | 8 | 3 || 5 | 9 | 2 ||
++---+---+---++---+---+---++---+---+---++
|| 2 | 5 | 9 || 1 | 4 | 7 || 8 | 3 | 6 ||
++---+---+---++---+---+---++---+---+---++
++---+---+---++---+---+---++---+---+---++
|| 7 | 4 | 6 || 3 | 9 | 2 || 1 | 8 | 5 ||
++---+---+---++---+---+---++---+---+---++
|| 9 | 2 | 3 || 5 | 1 | 8 || 7 | 6 | 4 ||
++---+---+---++---+---+---++---+---+---++
|| 8 | 1 | 5 || 4 | 7 | 6 || 3 | 2 | 9 ||
++---+---+---++---+---+---++---+---+---++
++---+---+---++---+---+---++---+---+---++
```

## Examples

Solve and pretty-print:

```bash
./sudoku < inputs/i1.txt
```

Solve and print in simple format:

```bash
./sudoku -s < inputs/i2.txt
```

Print input and then the solution:

```bash
./sudoku -i < inputs/i3.txt
```
