#include "TextCompression.h"

node* createnode(const char* sequence, int code) {
    node*nn = malloc(sizeof(node));
    nn->sequence = malloc(strlen(sequence) + 1);
    strcpy(nn->sequence, sequence);
    nn->code = code;
    return nn;
}

void freenode(node* p) {
    free(p->sequence);
    free(p);
}

int findnode(node** table, int size, const char* sequence) {
    for (int i = 0; i < size; i++) {
        if (strcmp(table[i]->sequence, sequence) == 0) {
            return table[i]->code;
        }
    }
    return -1;
}

void insertnode(node** table, int size, const char* sequence, int code) {
    table[size] = createnode(sequence, code);
}



