#ifndef BSQ_H
# define BSQ_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct s_map {
    int rows;
    int cols;
    char empty;
    char obstacle;
    char full;
    char **map;
} t_map;

typedef struct s_solution {
    int row;
    int col;
    int size;
} t_solution;

void bsq(FILE *stream);

#endif