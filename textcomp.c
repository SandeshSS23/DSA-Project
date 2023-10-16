#include "Textcomp.h"

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


void encode(const char* s1, int* output_code) {
    printf("Encoding\n");
    node** table = malloc(sizeof(node*) * MAX_TABLE_SIZE);
    for (int i = 0; i < 256; i++) {
        char ch[2] = { (char)i, '\0' };
        table[i] = createnode(ch, i);
    }
    char p[MAX_TABLE_SIZE] = "";
    char c[2] = { s1[0], '\0' };
    strcpy(p, c);

    int code = 256;
    int output_index = 0;

    printf("String\tOutput_Code\tAddition\n");
    for (int i = 1; i < strlen(s1); i++) {
        c[0] = s1[i];

        char pc[MAX_TABLE_SIZE] = "";
        strcpy(pc, p);
        strcat(pc, c);

        int nodeCode = findnode(table, code, pc);
        if (nodeCode != -1) {
            strcpy(p, pc);
        }
        else {
            printf("%s\t%d\t\t%s\t%d\n", p, findnode(table, code, p), pc, code);
            output_code[output_index++] = findnode(table, code, p);
            insertnode(table, code, pc, code);
            code++;
            strcpy(p, c);
        }
    }
    printf("%s\t%d\n", p, findnode(table, code, p));
    output_code[output_index++] = findnode(table, code, p);

    for (int i = 0; i < code; i++) {
        freenode(table[i]);
    }
    free(table);

    FILE* encoding_output_file = fopen("encoding_output.txt", "w");
    if (encoding_output_file == NULL) {
        printf("Failed to open encoding output file.\n");
        return;
    }

    //printf(encoding_output_file, "Output Codes are: ");
    for (int i = 0; i < output_index; i++) {
        fprintf(encoding_output_file, "%d ", output_code[i]);
    }
    fprintf(encoding_output_file, "\n");

    fclose(encoding_output_file);
}

void decode(const int* op, int op_size) {
    printf("\nDecoding\n");
    FILE* decoding_output_file = fopen("decoding_output.txt", "w");
    node** table = malloc(sizeof(node*) * MAX_TABLE_SIZE);
    
    for (int i = 0; i < 256; i++) {
        char ch[2] = { (char)i, '\0' };
        table[i] = createnode(ch, i);
        //printf("i = %d,   ch = %s\n",i,table[i]->sequence);
    }

    int old = op[0];
    int n;
    char *s=(char *)malloc(sizeof(char) * MAX_TABLE_SIZE);
    strcpy(s, table[old]->sequence);
    char c[MAX_TABLE_SIZE] = ""; 
    printf("%s", s);
    
    fprintf(decoding_output_file, "%s", s);
    int count = 256;
    
 	
    for (int i = 0; i < op_size-1; i++) {
    	 
         n = op[i+1];
         
        
        strcpy(s,table[n]->sequence);
        


        printf("%s", s);
	fprintf(decoding_output_file, "%s", s);
        

        char c[MAX_TABLE_SIZE] = ""; 
        char ch = s[0];
        strncat(c,&ch,1);
        // printf("c = %s\n",c);
        char temp[MAX_TABLE_SIZE] = "";
        strcpy(temp, table[old]->sequence);
        // printf("old sequence = %s\n",temp);


        strcat(temp,c);
        // printf("temp = %s\n",temp);
        
        
        table[count] = createnode(temp, count);
        // printf("   count = %d   string = %s\n",count,temp);
        count++;
        old = n;
    }

   
}