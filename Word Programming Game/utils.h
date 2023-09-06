#include <stdio.h>
#include <stdlib.h>
#define DICT_SIZE 5757
#define WORD_SIZE 5


void load_word_list(char dictionary[DICT_SIZE][WORD_SIZE+1]){

    int idx = 0;

    FILE* file = fopen("C:\\Users\\furqa\\Desktop\\University\\MTU\\Year 2 Sem 2\\C Programming\\Word Programming Game\\word_list.txt","r");

    if (file == NULL) {
        printf("Error: could not open word list file.\n");
        exit(1);
    }else{
        while(fscanf(file, "%s", dictionary[idx++]) != EOF);
    }

    fclose(file);
}