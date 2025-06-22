#include "sudoku.h"
#include <stdio.h>

int main() {
    sudokuInitTables();

    Sudoku sudoku;
    sudokuReadFromFile(&sudoku, stdin);
    printf("Sudoku to solve\n");
    sudokuPrettyPrint(&sudoku, stdout);

    int solved = sudokuSolve(&sudoku);
    if (solved) {
        printf("\nSudoku solved\n");
    } else {
        printf("\nSudoku not solved\n");
    }
    sudokuPrettyPrint(&sudoku, stdout);
}