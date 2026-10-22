#ifndef SOLVER_H
#define SOLVER_H

#include <stdint.h>

typedef struct {
    char letters[10];
    int n_letters;
    long long coeff[10];
    int is_leading[10];
    char* addends[10];
    int n_addends;
    char* result;
} Puzzle;

int solve_rebus(const char* puzzle_str, char* result);

void collect_letters(const char* puzzle_str, Puzzle* puzzle);
void parse_puzzle(const char* puzzle_str, Puzzle* puzzle);
void compute_coeffs(Puzzle* puzzle);
void mark_leading(Puzzle* puzzle);
int verify_solution(Puzzle* puzzle, int digit_map[10]);

#endif
