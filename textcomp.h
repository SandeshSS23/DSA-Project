#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
typedef struct node{
    char* sequence;
    int code;
}node;
#define MAX_TABLE_SIZE 650000
node* createnode(const char* sequence, int code);
void freenode(node* p);
int findnode(node** table, int size, const char* sequence);
void insertnode(node** table, int size, const char* sequence, int code);

void encode(const char* s1, int* output_code);
void decode(const int* op, int op_size);
