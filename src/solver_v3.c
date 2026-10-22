#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int find_letter_idx(Puzzle* p, char ch) {
    for (int i = 0; i < p->n_letters; i++) {
        if (p->letters[i] == ch) return i;
    }
    if (p->n_letters < 10) {
        p->letters[p->n_letters] = ch;
        return p->n_letters++;
    }
    return -1;
}

void collect_letters(const char* puzzle_str, Puzzle* puzzle) {
    puzzle->n_letters = 0;
    
    for (int i = 0; puzzle_str[i]; i++) {
        if (puzzle_str[i] >= 'A' && puzzle_str[i] <= 'Z') {
            find_letter_idx(puzzle, puzzle_str[i]);
        }
    }
}

void parse_puzzle(const char* puzzle_str, Puzzle* puzzle) {
    char buf[500];
    strncpy(buf, puzzle_str, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    
    char* eq = strchr(buf, '=');
    if (!eq) return;
    *eq = '\0';
    
    char* left = buf;
    char* right = eq + 1;
    
    while (*right == ' ') right++;
    puzzle->result = malloc(strlen(right) + 1);
    strcpy(puzzle->result, right);
    
    puzzle->n_addends = 0;
    char* p = left;
    
    while (*p) {
        while (*p == ' ') p++;
        char* start = p;
        while (*p && *p != '+') p++;
        
        int len = p - start;
        while (len > 0 && start[len-1] == ' ') len--;
        
        if (len > 0) {
            puzzle->addends[puzzle->n_addends] = malloc(len + 1);
            strncpy(puzzle->addends[puzzle->n_addends], start, len);
            puzzle->addends[puzzle->n_addends][len] = '\0';
            puzzle->n_addends++;
        }
        
        if (*p == '+') p++;
    }
}

void mark_leading(Puzzle* puzzle) {
    for (int i = 0; i < puzzle->n_letters; i++) {
        puzzle->is_leading[i] = 0;
    }
    
    for (int w = 0; w < puzzle->n_addends; w++) {
        int id = find_letter_idx(puzzle, puzzle->addends[w][0]);
        if (id >= 0) puzzle->is_leading[id] = 1;
    }
    
    int id = find_letter_idx(puzzle, puzzle->result[0]);
    if (id >= 0) puzzle->is_leading[id] = 1;
}

void compute_coeffs(Puzzle* puzzle) {
    for (int i = 0; i < puzzle->n_letters; i++) {
        puzzle->coeff[i] = 0;
    }
    
    for (int w = 0; w < puzzle->n_addends; w++) {
        int len = strlen(puzzle->addends[w]);
        int p10 = 1;
        
        for (int i = len - 1; i >= 0; i--) {
            int id = find_letter_idx(puzzle, puzzle->addends[w][i]);
            puzzle->coeff[id] += p10;
            p10 *= 10;
        }
    }
    
    int len = strlen(puzzle->result);
    int p10 = 1;
    
    for (int i = len - 1; i >= 0; i--) {
        int id = find_letter_idx(puzzle, puzzle->result[i]);
        puzzle->coeff[id] -= p10;
        p10 *= 10;
    }
}

int verify_solution(Puzzle* puzzle, int digit_map[10]) {
    long long sum = 0;
    
    for (int w = 0; w < puzzle->n_addends; w++) {
        long long v = 0;
        for (char* p = puzzle->addends[w]; *p; p++) {
            int id = find_letter_idx(puzzle, *p);
            v = v * 10 + digit_map[id];
        }
        sum += v;
    }
    
    long long res_val = 0;
    for (char* p = puzzle->result; *p; p++) {
        int id = find_letter_idx(puzzle, *p);
        res_val = res_val * 10 + digit_map[id];
    }
    
    return (sum == res_val);
}

void format_result(Puzzle* puzzle, int digit_map[10], char* result_str) {
    char temp[1000] = {0};
    
    for (int w = 0; w < puzzle->n_addends; w++) {
        if (w > 0) strcat(temp, " + ");
        
        long long v = 0;
        for (char* p = puzzle->addends[w]; *p; p++) {
            int id = find_letter_idx(puzzle, *p);
            v = v * 10 + digit_map[id];
        }
        
        char buf[50];
        sprintf(buf, "%lld", v);
        strcat(temp, buf);
    }
    
    strcat(temp, " = ");
    
    long long res_val = 0;
    for (char* p = puzzle->result; *p; p++) {
        int id = find_letter_idx(puzzle, *p);
        res_val = res_val * 10 + digit_map[id];
    }
    
    char buf[50];
    sprintf(buf, "%lld", res_val);
    strcat(temp, buf);
    
    strcpy(result_str, temp);
}

static long long suffix_max(Puzzle* p, int k) {
    long long s = 0;
    for (int j = k; j < p->n_letters; j++) {
        long long c = p->coeff[j];
        if (c < 0) c = -c;
        s += 9LL * c;
    }
    return s;
}

static int check_low_columns(Puzzle* p, int digit_map[10]) {
    int rlen = strlen(p->result);
    int maxlen = rlen;
    
    for (int w = 0; w < p->n_addends; w++) {
        int l = strlen(p->addends[w]);
        if (l > maxlen) maxlen = l;
    }
    
    int carry = 0;
    
    for (int t = 0; t < maxlen; t++) {
        int complete = 1;
        int sum = carry;
        
        for (int w = 0; w < p->n_addends; w++) {
            int l = strlen(p->addends[w]);
            if (t < l) {
                int i = find_letter_idx(p, p->addends[w][l - 1 - t]);
                if (digit_map[i] == -1) {
                    complete = 0;
                    break;
                }
                sum += digit_map[i];
            }
        }
        
        if (!complete) break;
        
        int res_digit = -1;
        if (t < rlen) {
            int ri = find_letter_idx(p, p->result[rlen - 1 - t]);
            if (digit_map[ri] == -1) {
                complete = 0;
            } else {
                res_digit = digit_map[ri];
            }
        }
        
        if (!complete) break;
        
        if (res_digit != -1) {
            if (sum % 10 != res_digit) return 0;
        }
        
        carry = sum / 10;
    }
    
    return 1;
}

static int dfs_v3(Puzzle* p, int k, int used_mask, long long partial, 
                  int digit_map[10], char* result) {
    
    if (k == p->n_letters) {
        if (partial == 0 && verify_solution(p, digit_map)) {
            format_result(p, digit_map, result);
            return 1;
        }
        return 0;
    }
    
    long long pabs = partial < 0 ? -partial : partial;
    if (pabs > suffix_max(p, k)) return 0;
    
    int i = k;
    
    for (int d = 0; d <= 9; d++) {
        if (used_mask & (1 << d)) continue;
        if (d == 0 && p->is_leading[i]) continue;
        
        digit_map[i] = d;
        
        if (check_low_columns(p, digit_map) &&
            dfs_v3(p, k + 1, used_mask | (1 << d), 
                   partial + (long long)p->coeff[i] * d, digit_map, result)) {
            return 1;
        }
    }
    
    digit_map[i] = -1;
    return 0;
}

int solve_rebus(const char* puzzle_str, char* result) {
    Puzzle puzzle = {0};
    
    collect_letters(puzzle_str, &puzzle);
    parse_puzzle(puzzle_str, &puzzle);
    compute_coeffs(&puzzle);
    mark_leading(&puzzle);
    
    int digit_map[10];
    for (int i = 0; i < 10; i++) {
        digit_map[i] = -1;
    }
    
    int found = dfs_v3(&puzzle, 0, 0, 0, digit_map, result);
    
    for (int i = 0; i < puzzle.n_addends; i++) {
        if (puzzle.addends[i]) free(puzzle.addends[i]);
    }
    if (puzzle.result) free(puzzle.result);
    
    return found;
}
