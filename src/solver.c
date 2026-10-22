#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int find_letter_index(Puzzle* puzzle, char letter) {
    for (int i = 0; i < puzzle->n_letters; i++) {
        if (puzzle->letters[i] == letter) {
            return i;
        }
    }
    return -1;
}

void collect_letters(const char* puzzle_str, Puzzle* puzzle) {
    puzzle->n_letters = 0;
    memset(puzzle->letters, 0, 10);
    
    for (int i = 0; puzzle_str[i] && puzzle->n_letters < 10; i++) {
        char c = puzzle_str[i];
        if (isalpha(c) && isupper(c)) {

            int found = 0;
            for (int j = 0; j < puzzle->n_letters; j++) {
                if (puzzle->letters[j] == c) {
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                puzzle->letters[puzzle->n_letters++] = c;
            }
        }
    }
}

void parse_puzzle(const char* puzzle_str, Puzzle* puzzle) {
    const char* eq_pos = strchr(puzzle_str, '=');
    if (!eq_pos) return;
    
    int left_len = eq_pos - puzzle_str;
    char* left_part = (char*)malloc(left_len + 1);
    strncpy(left_part, puzzle_str, left_len);
    left_part[left_len] = '\0';
    
    const char* right_part = eq_pos + 1;
    while (*right_part == ' ') right_part++;
    
    puzzle->result = (char*)malloc(strlen(right_part) + 1);
    strcpy(puzzle->result, right_part);
    
    int j = 0;
    for (int i = 0; puzzle->result[i]; i++) {
        if (puzzle->result[i] != ' ') {
            puzzle->result[j++] = puzzle->result[i];
        }
    }
    puzzle->result[j] = '\0';
    
    puzzle->n_addends = 0;
    char* copy = (char*)malloc(strlen(left_part) + 1);
    strcpy(copy, left_part);
    
    char* token = strtok(copy, "+");
    while (token && puzzle->n_addends < 10) {
        while (*token == ' ') token++;
        
        int len = strlen(token);
        while (len > 0 && token[len-1] == ' ') {
            token[len-1] = '\0';
            len--;
        }
        
        if (len > 0) {
            puzzle->addends[puzzle->n_addends] = (char*)malloc(len + 1);
            strcpy(puzzle->addends[puzzle->n_addends], token);
            puzzle->n_addends++;
        }
        
        token = strtok(NULL, "+");
    }
    
    free(copy);
    free(left_part);
}

void mark_leading(Puzzle* puzzle) {
    for (int i = 0; i < puzzle->n_letters; i++) {
        puzzle->is_leading[i] = 0;
    }
    
    for (int a = 0; a < puzzle->n_addends; a++) {
        char first = puzzle->addends[a][0];
        int idx = find_letter_index(puzzle, first);
        if (idx >= 0) {
            puzzle->is_leading[idx] = 1;
        }
    }
    
    char first = puzzle->result[0];
    int idx = find_letter_index(puzzle, first);
    if (idx >= 0) {
        puzzle->is_leading[idx] = 1;
    }
}

void compute_coeffs(Puzzle* puzzle) {
    for (int i = 0; i < puzzle->n_letters; i++) {
        puzzle->coeff[i] = 0;
    }
    
    for (int a = 0; a < puzzle->n_addends; a++) {
        char* word = puzzle->addends[a];
        int len = strlen(word);
        
        for (int pos = 0; pos < len; pos++) {
            char letter = word[pos];
            int idx = find_letter_index(puzzle, letter);
            if (idx < 0) continue;
            
            int64_t weight = 1;
            for (int j = 0; j < len - pos - 1; j++) {
                weight *= 10;
            }
            
            puzzle->coeff[idx] += weight;
        }
    }
    
    char* word = puzzle->result;
    int len = strlen(word);
    
    for (int pos = 0; pos < len; pos++) {
        char letter = word[pos];
        int idx = find_letter_index(puzzle, letter);
        if (idx < 0) continue;
        
        int64_t weight = 1;
        for (int j = 0; j < len - pos - 1; j++) {
            weight *= 10;
        }
        
        puzzle->coeff[idx] -= weight;
    }
}

int64_t compute_number(const char* word, Puzzle* puzzle, int digit_map[10]) {
    int64_t num = 0;
    
    for (int i = 0; word[i]; i++) {
        char letter = word[i];
        int letter_idx = find_letter_index(puzzle, letter);
        if (letter_idx < 0) continue;
        
        int digit = digit_map[letter_idx];
        num = num * 10 + digit;
    }
    
    return num;
}

// Проверить корректность
int verify_solution(Puzzle* puzzle, int digit_map[10]) {
    int64_t sum = 0;
    
    for (int a = 0; a < puzzle->n_addends; a++) {
        sum += compute_number(puzzle->addends[a], puzzle, digit_map);
    }
    
    int64_t result = compute_number(puzzle->result, puzzle, digit_map);
    
    return (sum == result);
}

void format_result(Puzzle* puzzle, int digit_map[10], char* result_str) {
    char temp[1000] = {0};
    
    for (int a = 0; a < puzzle->n_addends; a++) {
        if (a > 0) strcat(temp, " + ");
        
        int64_t num = compute_number(puzzle->addends[a], puzzle, digit_map);
        char buf[50];
        sprintf(buf, "%lld", num);
        strcat(temp, buf);
    }
    
    strcat(temp, " = ");
    
    int64_t res = compute_number(puzzle->result, puzzle, digit_map);
    char buf[50];
    sprintf(buf, "%lld", res);
    strcat(temp, buf);
    
    strcpy(result_str, temp);
}

int dfs_v0(Puzzle* puzzle, int k, int used_digits, int64_t partial_sum,
           int digit_map[10], char* result) {
    
    if (k == puzzle->n_letters) {
        if (partial_sum == 0) {
            if (verify_solution(puzzle, digit_map)) {
                format_result(puzzle, digit_map, result);
                return 1;
            }
        }
        return 0;
    }
    
    int letter_idx = k;
    char letter = puzzle->letters[letter_idx];
    
    for (int digit = 0; digit <= 9; digit++) {
        if (used_digits & (1 << digit)) {
            continue;
        }
        
        if (digit == 0 && puzzle->is_leading[letter_idx]) {
            continue;
        }
        
        digit_map[letter_idx] = digit;
        
        int64_t new_partial = partial_sum + puzzle->coeff[letter_idx] * digit;
        
        if (dfs_v0(puzzle, k + 1, used_digits | (1 << digit), new_partial, 
                   digit_map, result)) {
            return 1;
        }
    }
    
    return 0;
}

int solve_rebus(const char* puzzle_str, char* result) {
    Puzzle puzzle = {0};
    
    puzzle.n_letters = 0;
    puzzle.n_addends = 0;
    puzzle.result = NULL;
    
    collect_letters(puzzle_str, &puzzle);
    
    parse_puzzle(puzzle_str, &puzzle);
    
    compute_coeffs(&puzzle);
    
    mark_leading(&puzzle);
    
    int digit_map[10];
    for (int i = 0; i < 10; i++) {
        digit_map[i] = -1;
    }
    
    int found = dfs_v0(&puzzle, 0, 0, 0, digit_map, result);
    
    for (int i = 0; i < puzzle.n_addends; i++) {
        if (puzzle.addends[i]) {
            free(puzzle.addends[i]);
        }
    }
    if (puzzle.result) {
        free(puzzle.result);
    }
    
    return found;
}
