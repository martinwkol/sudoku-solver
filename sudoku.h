#ifndef __SUDOKU_H__
#define __SUDOKU_H__

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

typedef uint8_t SudokuEntry;
typedef uint16_t SudokuOptions;

#define SUDOKU_NUM_ROWS 9
#define SUDOKU_NUM_COLUMNS 9
#define SUDOKU_CELL_SIZE 3
#define SUDOKU_NUM_SQUARES (SUDOKU_NUM_ROWS * SUDOKU_NUM_COLUMNS)

#define SUDOKU_NO_ENTRY ((SudokuEntry) 0)

typedef struct {
    SudokuEntry entries[SUDOKU_NUM_SQUARES];
    SudokuOptions options[SUDOKU_NUM_SQUARES];
} Sudoku;

void sudokuInitTables();
void sudokuInitEmpty(Sudoku* sudoku);
void sudokuReadFromFile(FILE* file, Sudoku* sudoku);
void sudokuSimplePrint(FILE* file, Sudoku* sudoku);
void sudokuPrettyPrint(FILE* file, Sudoku* sudoku);

inline SudokuEntry sudokuGet(Sudoku* sudoku, int row, int column) {
    assert(0 <= row && row < SUDOKU_NUM_ROWS);
    assert(0 <= column && column < SUDOKU_NUM_COLUMNS);
    return sudoku->entries[row * SUDOKU_NUM_COLUMNS + column];
}
void sudokuSet(Sudoku* sudoku, int row, int column, SudokuEntry entry);
int sudokuSolve(Sudoku* sudoku);

#endif // __SUDOKU_H__
