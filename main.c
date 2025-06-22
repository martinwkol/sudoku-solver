#include "sudoku.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    sudokuInitTables();
    Sudoku sudoku;

    void (*printSudoku)(FILE*, Sudoku*) = sudokuPrettyPrint;
    int printInput = 0;
    int opt;
    while ((opt = getopt(argc, argv, "si")) != -1) {
        switch (opt)
        {
        case 's': printSudoku = sudokuSimplePrint; break;
        case 'i': printInput = 1; break;
        default:
            fprintf(stderr, "Usage: %s [-si]\n", argv[0]);
            return 1;
        }
    }

    sudokuReadFromFile(stdin, &sudoku);
    if (printInput) {
        printSudoku(stdout, &sudoku);
        printf("\n");
    }

    int solved = sudokuSolve(&sudoku);
    if (!solved) {
        fprintf(stderr, "Sudoku not solvable\n");
        return 2;
    }
    printSudoku(stdout, &sudoku);

    return 0;
}