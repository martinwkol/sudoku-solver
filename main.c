#include "sudoku.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    sudokuInitTables();
    Sudoku sudoku;

    
    void (*printSudoku)(Sudoku*, FILE*) = sudokuPrettyPrint;
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
    

    sudokuReadFromFile(&sudoku, stdin);

    if (printInput) {
        printSudoku(&sudoku, stdout);
        printf("\n");
    }

    int solved = sudokuSolve(&sudoku);
    if (!solved) {
        fprintf(stderr, "Sudoku not solvable\n");
        return 2;
    }
    printSudoku(&sudoku, stdout);

    return 0;
}