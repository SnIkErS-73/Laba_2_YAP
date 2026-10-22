#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "solver.h"

typedef struct {
    char puzzle[100];
    char name[20];
} TestCase;

TestCase tests[] = {
    {"SEND + MORE = MONEY", "T1"},
    {"FORTY + TEN + TEN = SIXTY", "T2"},
    {"ELEVEN + NINE + FIVE + FIVE = THIRTY", "T3"},
    {"BE + BE = MOO", "T4"},
};

int main() {
    printf("=== Cryptarithmetic Solver - V0 ===\n");
    printf("========================================\n\n");
    
    for (int i = 0; i < 4; i++) {
        printf("Test %s: %s\n", tests[i].name, tests[i].puzzle);
        
        char result[1000] = {0};
        
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        int found = solve_rebus(tests[i].puzzle, result);
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1e9;
        
        if (found) {
            printf("Solution: %s\n", result);
        } else {
            printf("No solution found\n");
        }
        printf("Time: %.6f sec\n\n", elapsed);
    }
    
    return 0;
}
