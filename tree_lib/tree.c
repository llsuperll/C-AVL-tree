#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tree.h"
#include "../include/input.h"

int get_height(Node* node) {
    if (node) {
        return node -> height;
    }
    return 0;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int get_balance(Node* node) {
    if (node) {
        return get_height(node -> left) - get_height(node -> right);
    }
    return 0;
}

Node* right_rotate(Node* y) {
    int h, h1, h2;
    Node* x = y -> left;
    Node* R = x -> right;
    
    x -> right = y;
    y -> left = R;
    
    h1 = get_height(y -> right);
    h2 = get_height(y -> left);
    h = max(h1, h2);
    y -> height = h + 1;
    
    h1 = get_height(x -> right);
    h2 = get_height(x -> left);
    h = max(h1, h2);
    x -> height = h + 1;
    return x;
}

Node* left_rotate(Node* x) {
    int h, h1, h2;
    Node* y = x -> right;
    Node* L = y -> left;

    y -> left = x;
    x -> right = L;

    h1 = get_height(x -> right);
    h2 = get_height(x -> left);
    h = max(h1, h2);
    x -> height = h + 1;
    
    h1 = get_height(y -> right);
    h2 = get_height(y -> left);
    h = max(h1, h2);
    y -> height = h + 1;
    return y;
}

ErrorCode insert(Node** root, const char* key, const char* info) {
    if (!*root) {
        Node* new = malloc(sizeof(Node));
        if (!new) {
            return ERR_MEMORY;
        }
        new -> key = strdup(key);
        new -> info = strdup(info);
        new -> left = NULL;
        new -> right = NULL;
        new -> height = 1;
        *root = new;
        return OK;
    }

    if (strcmp(key, (*root)->key) >= 0) {
        insert(&(*root) -> right, key, info);
    } else {
        insert(&(*root) -> left, key, info);
    }

    int left_h = get_height((*root) -> left);
    int right_h = get_height((*root) -> right);
    (*root) -> height = max(left_h, right_h) + 1;

    int balance = get_balance(*root);
    int balance_right = get_balance((*root) -> right);
    int balance_left = get_balance((*root) -> left);

    if (balance > 1) {
        if (balance_left < 0) {
            (*root) -> left = left_rotate((*root) -> left);
            *root = right_rotate(*root);
        } else {
            *root = right_rotate(*root);
        }
    } else if (balance < -1) {
        if (balance_right > 0) {
            (*root) -> right = right_rotate((*root) -> right);
            *root = left_rotate(*root);
        } else {
            *root = left_rotate(*root);
        }
    }
    return OK;
}

ErrorCode delete(Node** root, const char* key) {
    if (!key) {
        return ERR_INFO;
    } else if (!*root) {
        return ERR_ELEMENT_NOT_FOUND;
    }
    if (strcmp(key, (*root) -> key) > 0) {
        delete(&(*root) -> right, key);
    } else if (strcmp(key, (*root) -> key) < 0) {
        delete(&(*root) -> left, key);
    } else {
        if ((*root) -> right && (*root) -> left) {
            Node* min = (*root) -> right;
            while (min -> left) {
                min = min -> left;
            }
            free((*root) -> key);
            (*root) -> key = NULL;
            free((*root) -> info);
            (*root) -> info = NULL;
            (*root) -> key = strdup(min -> key);
            (*root) -> info = strdup(min -> info);
            delete(&(*root) -> right, min -> key);
        } else if (!(*root) -> right && !(*root) -> left) {
            free((*root) -> info);
            (*root) -> info = NULL;
            free((*root) -> key);
            (*root) -> key = NULL;
            free((*root));
            *root = NULL;
        } else {
            Node* child = NULL;
            if ((*root) -> right) {
                child = (*root) -> right;
            } else {
                child = (*root) -> left;
            }
            Node* to_free = *root;
            *root = child;
            free(to_free -> key);
            to_free -> key = NULL;
            free(to_free -> info);
            to_free -> info = NULL;
            free(to_free);
            to_free = NULL;
        }
    }
    if (!*root) {
        return OK;
    }

    int left_h = get_height((*root) -> left);
    int right_h = get_height((*root) -> right);
    (*root) -> height = max(left_h, right_h) + 1;

    int balance = get_balance(*root);
    int balance_right = get_balance((*root) -> right);
    int balance_left = get_balance((*root) -> left);

    if (balance > 1) {
        if (balance_left < 0) {
            (*root) -> left = left_rotate((*root) -> left);
            *root = right_rotate(*root);
        } else {
            *root = right_rotate(*root);
        }
    } else if (balance < -1) {
        if (balance_right > 0) {
            (*root) -> right = right_rotate((*root) -> right);
            *root = left_rotate(*root);
        } else {
            *root = left_rotate(*root);
        }
    }
    return OK;
}

void bypass_backwards(Node* root, const char* start, const char* end) {
    if (!root) {
        return;
    }
    bypass_backwards(root -> left, start, end);
    bypass_backwards(root -> right, start, end);
    if (strcmp(root -> key, start) < 0 || strcmp(root -> key, end) > 0) {
        printf("%s [%s]\n", root -> key, root -> info);
    }
}

void free_tree(Node** root) {
    if (*root) {
        free((*root) -> key);
        (*root) -> key = NULL;
        free((*root) -> info);
        (*root) -> info = NULL;
        free_tree(&(*root) -> left);
        free_tree(&(*root) -> right);
        free(*root);
        *root = NULL;
    }
    return;
}

void print_tree(Node* node, const char* prefix, int isLeft) {
    if (!node) {
        return;
    }
    printf("%s", prefix);
    if (isLeft) {
        printf("├── ");
    } else {
        printf("└── ");
    }
    printf("%s [%s]\n", node -> key, node -> info);
    char new_pref[256] = {0};
    strncpy(new_pref, prefix, 255);
    if (isLeft) {
        strcat(new_pref, "│   ");
    } else {
        strcat(new_pref, "    ");
    }
    print_tree(node -> left, new_pref, 1);
    print_tree(node -> right, new_pref, 0);
}

ErrorCode find_key(Node*** arr, int* arr_size, Node* root, const char* key) {
    if (!key) {
        return ERR_INFO;
    }
    *arr = NULL;
    *arr_size = 0;
    Node* cur = root;
    Node** res = NULL;
    int cap = 0;

    *arr = res;
    *arr_size = cap;
    while (cur) {
        int cmp = strcmp(key, cur -> key);
        if (cmp == 0) {
            cap += 1;
            res = realloc(res, sizeof(Node*) * cap);
            if (!res) {
                return ERR_MEMORY;
            }
            res[cap - 1] = cur;
            cur = cur -> right;
        } else if (cmp > 0) {
            cur = cur -> right;
        } else {
            cur = cur -> left;
        }
    }
    *arr = res;
    *arr_size = cap;
    return OK;
}

int compare_strings(const char* str1, const char* str2) {
    int sum1 = 0, sum2 = 0;
    for (int i = 0; str1[i] != '\0'; i++) {
        sum1 += (int)str1[i];
    }
    for (int i = 0; str2[i] != '\0'; i++) {
        sum2 += (int)str2[i];
    }
    return sum1 - sum2;
}

ErrorCode find_closest_key(Node*** arr, int* arr_size, Node* root, const char* key) {
    if (!key) {
        return ERR_INFO;
    }
    *arr = NULL;
    *arr_size = 0;
    Node* cur = root;
    Node* found = root;
    Node** res = NULL;
    int cap = 0, min_diff = 10e8, diff = 0;

    while (cur) {
        int diff = compare_strings(key, cur -> key);
        if (0 < diff && diff < min_diff) {
            cap = 1;
            min_diff = diff;
            free(res);
            res = malloc(sizeof(Node*));
            res[0] = cur;
        } else if (diff == min_diff) {
            cap += 1;
            res = realloc(res, sizeof(Node*) * cap);
            res[cap - 1] = cur;
        }
        
        if (diff > 0) {
            cur = cur -> right;
        } else {
            cur = cur -> left;
        }
    }
    *arr = res;
    *arr_size = cap;
    return OK;
}

ErrorCode import_from_file(Node** root, const char* filename) {
    if (!filename) {
        return ERR_FILE;
    }
    FILE* file = fopen(filename, "r");
    if (!file) {
        return ERR_FILE;
    }
    free_tree(root);
    Node* new_root = NULL;
    char* line = NULL;
    char* key = NULL;
    char* info = NULL;
    int c = 0;
    while ((line = my_getline("", file)) != NULL) {
        if (c % 2 == 0) {
            key = strdup(line);
        } else {
            info = strdup(line);
            if (insert(&new_root, key, info) != OK) {
                free(line);
                free(info);
                free(key);
                return ERR_MEMORY;
            }
            if (key) {
                free(key);
            }
            if (info) {
                free(info);
            }
        }
        c += 1;
        free(line);
    }
    if (c % 2 != 0) {
        return ERR_FILE_READ;
    }
    *root = new_root;
    fclose(file);
    return OK;
}

void fill_dotfile(FILE* file, Node* node) {
    if (!node) {
        return;
    }
    fprintf(file, "    \"%p\" [label=\"%s\\nvalue: %s\"];\n", (void*)node, node -> key, node -> info);
    if (node -> left) {
        fprintf(file, "    \"%p\" -> \"%p\";\n", (void*)node, (void*)node -> left);
        fill_dotfile(file, node -> left);
    }
    if (node -> right) {
        fprintf(file, "    \"%p\" -> \"%p\";\n", (void*)node, (void*)node -> right);
        fill_dotfile(file, node -> right);
    }
}

ErrorCode graphviz_tree(Node* root, const char* filename) {
    if (!root) {
        return ERR_NULL_TREE;
    }
    if (!filename) {
        return ERR_FILE;
    }
    FILE* file = fopen(filename, "w");
    if (!file) {
        return ERR_FILE;
    }
    fprintf(file, "digraph BST {\n");
    fprintf(file, "    node [shape=circle, style=filled, fillcolor=\"#22e55d\"];\n");
    fill_dotfile(file, root);
    fprintf(file, "}\n");
    fclose(file);

    char* cmd = calloc(2 * strlen(filename) + 27, sizeof(char));
    if (!cmd) {
        return ERR_MEMORY;
    }
    sprintf(cmd, "dot -Tpng %s -o %s.png", filename, filename);
    pclose(popen(cmd, "r"));
    free(cmd);
    return OK;
}
