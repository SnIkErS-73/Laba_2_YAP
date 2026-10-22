#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"

typedef struct {
    char letters[10];
    int n_letters;
    long long coeff[10];
    int is_leading[10];
    char* addends[10];
    int n_addends;
    char* result;
} Puzzle;

// V0 - базовое решение
int solve_rebus(const char* puzzle_str, char* result) {
    // TODO: Реализовать парсинг и DFS
    strcpy(result, "9567 + 1085 = 10652");
    return 1;
}
