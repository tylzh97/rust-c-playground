/*
 * Filename:      main.c
 * Author:        Chenghao <sculizhenghao@foxmail.com>
 * Date Created:  2024/04/29 10:30
 * Last Modified: 2024/04/29 15:10
 * Description:   
 *    Mix-coding programming, for a C language project, implement some 
 * of the core functions using Rust, and call it as an external function.
 */


#include <stdio.h>
#include <stdint.h>

extern int64_t* sum_elements(int[], int, float);
extern void     free_memory(int64_t*);

int main() {
    int array[] = {1, 2, 3, 4};
    float float_number = 3.14;

    int64_t* results = sum_elements(array, sizeof(array)/sizeof(int), float_number);
    for (int i = 0; i < sizeof(array)/sizeof(int); i++) {
        printf("--> idx: %d, %ld \n", i, results[i]);
    }
    free_memory(results);
    // free_memory(results);  // free(): double free detected in tcache 2

    return 0;
}
