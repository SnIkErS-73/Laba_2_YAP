#ifndef SOLVER_H
#define SOLVER_H

#include <stdint.h>

typedef struct {
    char letters[10];           // Уникальные буквы
    int n_letters;              // Количество букв
    int64_t coeff[10];          // Коэффициенты для каждой буквы
    int is_leading[10];         // Флаг: ведущая буква (не может быть 0)
    char* addends[10];          // Слагаемые левой части
    int n_addends;              // Количество слагаемых
    char* result;               // Результат (правая часть)
} Puzzle;

// Основная функция решения ребуса
int solve_rebus(const char* puzzle_str, char* result);

// Внутренние функции
void parse_puzzle(const char* puzzle_str, Puzzle* puzzle);
void collect_letters(const char* puzzle_str, Puzzle* puzzle);
void compute_coeffs(Puzzle* puzzle);
void mark_leading(Puzzle* puzzle);
int dfs_v0(Puzzle* puzzle, int k, int used_digits, int64_t partial_sum, 
           int digit_map[10], char* result);

#endif
