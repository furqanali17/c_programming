#include <stdio.h>

#define DICT_SIZE 5757
#define WORD_SIZE 5


void load_word_list(char dictionary[DICT_SIZE][WORD_SIZE + 1]) {

    char *file_name = "./word_list.txt";

    FILE *f = fopen(file_name, "r");
    int idx = 0;

    if (f == NULL) {
        printf("%s does not exist. Please ensure you have downloaded it from Canvas.", file_name);
    } else {
        while (fscanf(f, "%s", dictionary[idx++]) != EOF);
    }

}