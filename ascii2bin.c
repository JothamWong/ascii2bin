// Modified from https://github.com/bibrakc/HPX_graphs/blob/main/utils/ASCII2Bin.c

/*
 * Author:  Bibrak Qamar
 * File:    ASCII2Bin.c
 * 2018-2023
 *
 * Basic C code to read an ASCII graph file and
 * write the edges to a binay file
 *
 */

/*
 * compile: gcc ASCII2Bin.c
 * run: ./a.out ./scale16_s.mm 1
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *exe;

void error_if(int test) {
    if (test) {
        printf("%s edgelists [filemask] [n_partition]\n", exe);
        printf("%s mapping [filename]\n", exe);
        exit(-1);
    }
}

void to_bin_edge(char *path, int part_id) {
    FILE *f = NULL;

    if ((f = fopen(path, "r")) == NULL) {
        printf("Could not open file %s.\n", path);
        exit(-1);
    }

    FILE *f_wtr;
    char filename_w[256];

    strcpy(filename_w, path);
    strcat(filename_w, ".bin");

    if ((f_wtr = fopen(filename_w, "wb")) == NULL) {
        printf("Could not open file %s.\n", filename_w);
        exit(-1);
    }

    fwrite(&part_id, sizeof(part_id), 1, f_wtr);
    uint32_t vert_from, vert_to;
    float weight;

    while (fscanf(f, "%d\t%d\t%f", &vert_from, &vert_to, &weight) != EOF) {
        fwrite(&vert_from, sizeof(vert_from), 1, f_wtr);
        fwrite(&vert_to, sizeof(vert_to), 1, f_wtr);
        fwrite(&weight, sizeof(weight), 1, f_wtr);
    }

    fclose(f);
    fclose(f_wtr);
}

void to_bin_map(char *path) {
    FILE *f = NULL;

    if ((f = fopen(path, "r")) == NULL) {
        printf("Could not open file %s.\n", path);
        exit(-1);
    }

    FILE *f_wtr;
    char filename_w[256];

    strcpy(filename_w, path);
    strcat(filename_w, ".bin");

    if ((f_wtr = fopen(filename_w, "wb")) == NULL) {
        printf("Could not open file %s.\n", filename_w);
        exit(-1);
    }

    uint32_t from, to;

    while (fscanf(f, "%d\t%d", &from, &to) != EOF) {
        fwrite(&from, sizeof(uint32_t), 1, f_wtr);
        fwrite(&to, sizeof(uint32_t), 1, f_wtr);
    }

    fclose(f);
    fclose(f_wtr);
}

int main(int argc, char *argv[]) {
    exe = argv[0];
    error_if(argc < 2);
    char *opt = argv[1];
    if (strcmp(opt, "edgelists") == 0) {
        error_if(argc < 4);
        char *filemask = argv[2];
        char filename[256];
        for (int i = 0; i < atoi(argv[3]); i++) {
            printf("Handling %d", i);
            sprintf(filename, filemask, i);
            to_bin_edge(filename, i);
        }
    } else if (strcmp(opt, "mapping") == 0) {
        error_if(argc < 3);
        to_bin_map(argv[2]);
    }

    return 0;
}