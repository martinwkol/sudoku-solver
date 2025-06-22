#include "sudoku.h"

#define SUDOKU_OPT_NUM_BITS 9
#define SUDOKU_BIT_TABLES_SIZE (1 << SUDOKU_OPT_NUM_BITS)

#define SUDOKU_ALL_OPTIONS ((SudokuOptions)((1 << 9) - 1))

typedef enum {
    NOT_SOLVABLE = 0,
    SOLVED = 1,
    NOT_SOLVED_YET = 2
} SolveStatus;

static int8_t popcountTable[SUDOKU_BIT_TABLES_SIZE];
static int8_t lsbTable[SUDOKU_BIT_TABLES_SIZE];
static int tablesInitialized = 0;




// Init tables

static void initPopcountTable() {
    for (SudokuOptions i = 0; i < SUDOKU_BIT_TABLES_SIZE; i++) {
        int8_t popcount = 0;
        for (SudokuOptions options = i; options; options &= options - 1) popcount++;
        popcountTable[i] = popcount;
    }
}

static void initLsbTable() {
    lsbTable[0] = -1;
    for (SudokuOptions i = 1; i < SUDOKU_BIT_TABLES_SIZE; i++) {
        int8_t lsb = 0;
        for (SudokuOptions options = i; (options & 1) == 0; options >>= 1) lsb++;
        lsbTable[i] = lsb;
    }
}

void sudokuInitTables() {
    initPopcountTable();
    initLsbTable();
    tablesInitialized = 1;
}




// Helper functions

static int8_t popcount(SudokuOptions options) {
    assert(tablesInitialized);
    assert(options < SUDOKU_BIT_TABLES_SIZE);
    return popcountTable[options];
}

static int8_t lsb(SudokuOptions options) {
    assert(tablesInitialized);
    assert(options < SUDOKU_BIT_TABLES_SIZE);
    return lsbTable[options];
}

static int8_t popLsb(SudokuOptions* options) {
    int8_t ret = lsb(*options);
    *options &= *options - 1;
    return ret;
}

static SudokuEntry popOption(SudokuOptions* options) {
    return (SudokuEntry) (popLsb(options) + 1);
}

static SudokuOptions optionsAllow(SudokuOptions options, SudokuEntry entry) {
    return options & (1 << (entry - 1));
}

static void optionsRemove(SudokuOptions* options, SudokuEntry entry) {
    *options &= ~(1 << (entry - 1));
}

static int squareIdx(int row, int column) {
    assert(0 <= row && row < SUDOKU_NUM_ROWS);
    assert(0 <= column && column < SUDOKU_NUM_COLUMNS);
    return row * SUDOKU_NUM_COLUMNS + column;
}




// Init sudoku

void sudokuInitEmpty(Sudoku* sudoku) {
    for (int idx = 0; idx < SUDOKU_NUM_SQUARES; idx++) {
        sudoku->entries[idx] = SUDOKU_NO_ENTRY;
        sudoku->options[idx] = SUDOKU_ALL_OPTIONS;
    }
}

void sudokuReadFromFile(Sudoku* sudoku, FILE* file) {
    for (int idx = 0; idx < SUDOKU_NUM_SQUARES; idx++) {
        sudoku->entries[idx] = SUDOKU_NO_ENTRY;
        sudoku->options[idx] = SUDOKU_ALL_OPTIONS;
    }
    for (int idx = 0; idx < SUDOKU_NUM_SQUARES; idx++) {
        int input = getc(file);
        while (!('0' <= input && input <= '9')) input = getc(file);
        if (input != '0') { 
            sudokuSet(sudoku, idx / SUDOKU_NUM_COLUMNS, idx % SUDOKU_NUM_COLUMNS, input - '0');
        }
    }
}




// Print sudoku

void sudokuSimplePrint(Sudoku* sudoku, FILE* file) {
    for (int idx = 0; idx < SUDOKU_NUM_SQUARES; idx++) {
        SudokuEntry entry = sudoku->entries[idx];
        if (entry == SUDOKU_NO_ENTRY)   fprintf(file, "0");
        else                            fprintf(file, "%d", entry);
    }
}

static void printHorizontalLine(FILE* file) {
    for (int column = 0; column < SUDOKU_NUM_COLUMNS; column++) {
        if (column % SUDOKU_CELL_SIZE == 0) fprintf(file, "++---");
        else fprintf(file, "+---");
    }
    fprintf(file, "++\n");
}

void sudokuPrettyPrint(Sudoku* sudoku, FILE* file) {
    for (int row = 0; row < SUDOKU_NUM_ROWS; row++) {
        printHorizontalLine(file);
        if (row % SUDOKU_CELL_SIZE == 0) printHorizontalLine(file);
        for (int column = 0; column < SUDOKU_NUM_COLUMNS; column++) {
            fprintf(file, "|");
            if (column % SUDOKU_CELL_SIZE == 0) fprintf(file, "|");
            SudokuEntry entry = sudokuGet(sudoku, row, column);
            if (entry == SUDOKU_NO_ENTRY)  fprintf(file, "   ");
            else                    fprintf(file, " %d ", entry);
        }
        fprintf(file, "||\n");
    }
    printHorizontalLine(file);
    printHorizontalLine(file);
}




// getter & setter for for squares

extern inline SudokuEntry sudokuGet(Sudoku* sudoku, int row, int column);

void sudokuSet(Sudoku* sudoku, int row, int column, SudokuEntry entry) {
    assert(sudoku->entries[squareIdx(row, column)] == SUDOKU_NO_ENTRY);
    sudoku->entries[squareIdx(row, column)] = entry;
    for (int c = 0; c < SUDOKU_NUM_COLUMNS; c++) {
        optionsRemove(&sudoku->options[squareIdx(row, c)], entry);
    }
    for (int r = 0; r < SUDOKU_NUM_ROWS; r++) {
        optionsRemove(&sudoku->options[squareIdx(r, column)], entry);
    }
    int cellRStart = (row / SUDOKU_CELL_SIZE) * SUDOKU_CELL_SIZE;
    int cellCStart = (column / SUDOKU_CELL_SIZE) * SUDOKU_CELL_SIZE;
    for (int cellR = cellRStart; cellR < cellRStart + SUDOKU_CELL_SIZE; cellR++) {
        for (int cellC = cellCStart; cellC < cellCStart + SUDOKU_CELL_SIZE; cellC++) {
            optionsRemove(&sudoku->options[squareIdx(cellR, cellC)], entry);
        }
    }
}




// Solve sudoku

static SolveStatus solveSingleOptions(Sudoku* sudoku) {
    int continueSearch = 1;
    while (continueSearch) {
        int solved = 1;
        continueSearch = 0;
        for (int row = 0; row < SUDOKU_NUM_ROWS; row++) {
            for (int column = 0; column < SUDOKU_NUM_COLUMNS; column++) {
                int idx = squareIdx(row, column);
                if (sudoku->entries[idx] != SUDOKU_NO_ENTRY) continue;
                solved = 0;

                int optionsCount = popcount(sudoku->options[idx]);
                if (optionsCount == 0) return NOT_SOLVABLE;
                if (optionsCount >= 2) continue;
                sudokuSet(sudoku, row, column, popOption(&sudoku->options[idx]));
                continueSearch = 1;
            }
        }
        if (solved) return SOLVED;
    }
    return NOT_SOLVED_YET;
}

static int solveMultipleOptions(Sudoku* sudoku) {
    int minNumOptions = SUDOKU_OPT_NUM_BITS + 1;
    int bestRow = 0;
    int bestCol = 0;
    for (int row = 0; row < SUDOKU_NUM_ROWS; row++) {
        for (int column = 0; column < SUDOKU_NUM_COLUMNS; column++) {
            if (sudokuGet(sudoku, row, column) != SUDOKU_NO_ENTRY) continue;
            int numOptions = popcount(sudoku->options[squareIdx(row, column)]);
            if (numOptions < minNumOptions) {
                minNumOptions = numOptions;
                bestRow = row;
                bestCol = column;
            }
        }
    }

    if (minNumOptions == 0) return 0;
    SudokuOptions* bestOption = &sudoku->options[squareIdx(bestRow, bestCol)];
    while (*bestOption) {
        SudokuEntry option = popOption(bestOption);
        Sudoku copy = *sudoku;
        sudokuSet(&copy, bestRow, bestCol, option);
        if (sudokuSolve(&copy)) {
            *sudoku = copy;
            return 1;
        }
    }
    return 0;
}

int sudokuSolve(Sudoku* sudoku) {
    SolveStatus solveStatus = solveSingleOptions(sudoku);
    if (solveStatus == NOT_SOLVABLE) return 0;
    if (solveStatus == SOLVED) return 1;
    return solveMultipleOptions(sudoku);
}
