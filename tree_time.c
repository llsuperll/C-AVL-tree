#include <stdio.h>
#include <stdlib.h>
#include "include/tree.h"
#include <time.h>
#include <string.h>

#define N 5

int main() {
    srand(time(NULL));
    FILE* file = fopen("out.csv", "w");
    FILE* input = fopen("random_strings.txt", "r");
    if (!file) {
        return ERR_FILE;
    }
    Node* root = NULL;
    Node** search = NULL;
    int s = 0;
    ErrorCode err;
    clock_t start, end;
    double insert_t = 0, find_t = 0, spec_t = 0, bypass_t = 0, del_t = 0;
    char* key;
    char buffer[256];
    fprintf(file, "N,Insert,Search,SpecialSearch,Bypass,Delete\n");
    for (int i = 0; i <= 1000000; i++) {
        fgets(buffer, sizeof(buffer), input);
        key = strdup(buffer);
        int info = i;
        if (i % 10000 == 0) {
            start = clock();
            for (int i = 0; i < N; i++) {
                err = insert(&root, key, info);
            }
            end = clock();
            insert_t = (double)(end - start) / CLOCKS_PER_SEC;

            char* s_key = generate_key();
            start = clock();

            for (int i = 0; i < N; i++) {
                err = find_key(&search, &s, root, s_key);
            }
            end = clock();
            find_t = (double)(end - start) / CLOCKS_PER_SEC;
            free(search);
            start = clock();
            for (int i = 0; i < N; i++) {
                err = find_closest_key(&search, &s, root, s_key);
            }
            end = clock();
            spec_t = (double)(end - start) / CLOCKS_PER_SEC;
            free(search);

            start = clock();
            for (int i = 0; i < N; i++) {
                bypass_backwards(root, "", "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ");
            }
            end = clock();
            bypass_t = (double)(end - start) / CLOCKS_PER_SEC;

            start = clock();
            for (int i = 0; i < N; i++) {
                err = delete(&root, key);
            }
            end = clock();
            del_t = (double)(end - start) / CLOCKS_PER_SEC;

            fprintf(file, "%d,%.1f,%.1f,%.1f,%.1f,%.1f\n", i, insert_t, find_t, spec_t, bypass_t, del_t);
            free(s_key);
        } else {
            err = insert(&root, key, info);
        }
        free(key);
    }
    free_tree(&root);
    fclose(file);
}
