#include <stdio.h>
#include <time.h>
#include "solver.h"

// Тестовые случаи
typedef struct {
    char puzzle[100];
    char expected[100];
} TestCase;

TestCase tests[] = {
    {"SEND + MORE = MONEY", "9567 + 1085 = 10652"},
    {"FORTY + TEN + TEN = SIXTY", ""},
    {"ELEVEN + NINE + FIVE + FIVE = THIRTY", ""},
};

int main() {
    printf("Cryptarithmetic Solver--------------------\n\n");
    
    for (int i = 0; i < 3; i++) {
        char result[100] = {0};
        
        printf("Test %d: %s\n", i+1, tests[i].puzzle);
        
        double start = clock();
        int found = solve_rebus(tests[i].puzzle, result);
        double elapsed = (clock() - start) / CLOCKS_PER_SEC;
        
        if (found) {
            printf("Solution: %s\n", result);
            printf("Time: %.6f sec\n\n", elapsed);
        } else {
            printf("No solution found\n\n");
        }
    }
    
    return 0;
}
