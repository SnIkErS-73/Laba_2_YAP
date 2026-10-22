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
    {"BE + BE = MOO", "T4"},
};

int main() {
    printf("=== Debug Mode ===\n\n");
    
    char puzzle_str[] = "SEND + MORE = MONEY";
    
    Puzzle puzzle = {0};
    collect_letters(puzzle_str, &puzzle);
    parse_puzzle(puzzle_str, &puzzle);
    compute_coeffs(&puzzle);
    mark_leading(&puzzle);
    
    printf("Puzzle: %s\n\n", puzzle_str);
    
    printf("Letters: ");
    for (int i = 0; i < puzzle.n_letters; i++) {
        printf("%c ", puzzle.letters[i]);
    }
    printf("\n\n");
    
    printf("Addends:\n");
    for (int i = 0; i < puzzle.n_addends; i++) {
        printf("  %s\n", puzzle.addends[i]);
    }
    printf("Result: %s\n\n", puzzle.result);
    
    printf("Coefficients:\n");
    for (int i = 0; i < puzzle.n_letters; i++) {
        printf("  %c: %lld (leading: %d)\n", puzzle.letters[i], puzzle.coeff[i], puzzle.is_leading[i]);
    }
    
    printf("\n\nTesting manual assignment: S=9, E=5, N=6, D=7, M=1, O=0, R=8, Y=2\n");
    int digit_map[10] = {-1};
    digit_map[0] = 9;
    digit_map[1] = 5;
    digit_map[2] = 6;
    digit_map[3] = 7;
    digit_map[4] = 1;
    digit_map[5] = 0;
    digit_map[6] = 8;
    digit_map[7] = 2;
    
    int64_t partial = 0;
    for (int i = 0; i < puzzle.n_letters; i++) {
        partial += puzzle.coeff[i] * digit_map[i];
    }
    printf("Partial sum: %lld\n", partial);
    printf("Verify: %d\n", verify_solution(&puzzle, digit_map));
    
    for (int i = 0; i < puzzle.n_addends; i++) {
        free(puzzle.addends[i]);
    }
    free(puzzle.result);
    
    return 0;
}
