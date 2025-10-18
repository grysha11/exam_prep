#include "bsq.h"

static void free_map(t_map *map) {
    if (!map) {
        return;
    }
    if (map->map) {
        for (int i = 0; i < map->rows; i++) {
            free(map->map[i]);
        }
        free(map->map);
    }
    free(map);
}

int min3(int a, int b, int c) {
    int res = a;
    if (res > b) {
        res = b;
    }
    if (res > c) {
        res = c;
    }

    return res;
}

void algo(t_map *map, t_solution *solution) {
    int **value_map = malloc(sizeof(int *) * map->rows);
    for (int i = 0; i < map->rows; ++i) {
        value_map[i] = calloc(map->cols, sizeof(int));
    }

    solution->size = 0;

    for (int i = 0; i < map->rows; ++i) {
        for (int j = 0; j < map->cols; ++j) {
            if (map->map[i][j] == map->obstacle) {
                value_map[i][j] = 0;
            } else {
                if (i == 0 || j == 0) {
                    value_map[i][j] = 1;
                } else {
                    value_map[i][j] = 1 + min3(value_map[i - 1][j], value_map[i][j - 1], value_map[i - 1][j - 1]);
                }
                if (value_map[i][j] > solution->size) {
                    solution->size = value_map[i][j];
                    solution->row = i;
                    solution->col = j;
                }
            }
        }
    }

    for (int i = 0; i < map->rows; ++i) {
        free(value_map[i]);
    }
    free(value_map);
}

static t_map *parse_map(FILE *stream) {
    t_map *res = calloc(1, sizeof(t_map));

    if (!res) {
        return NULL;
    }

    if (fscanf(stream, "%d %c %c %c", &res->rows, &res->empty, &res->obstacle, &res->full) != 4 ||
        res->rows <= 0 || res->empty == res->obstacle || res->empty == res->full || res->obstacle == res->full) {
            printf("privet1\n");
            free(res);
            return NULL;
        }
    
    res->map = malloc(sizeof(char *) * res->rows);
    if (!res->map) {
        printf("privet2\n");
        free_map(res);
        return NULL;
    }

    char *dummy = NULL;
    size_t dummy_size = 0;
    getline(&dummy, &dummy_size, stream);
    free(dummy);

    char *line = NULL;
    size_t size = 0;
    for (int i = 0; i < res->rows; ++i) {
        ssize_t nread = getline(&line, &size, stream);
        if (nread <= 1 || line[nread - 1] != '\n') {
            res->rows = i;
            free(line);
            free_map(res);
            return NULL;
        }

        line[nread - 1] = '\0';

        if (i == 0) {
            res->cols = nread - 1;
            if (res->cols == 0) {
                res->rows = i + 1;
                free(line);
                free_map(res);
                return NULL;
            }
        } else if (res->cols != nread - 1) {
            res->rows = i + 1;
            free(line);
            free_map(res);
            return NULL;
        }

        for (int j = 0; j < res->cols; ++j) {
            if (line[j] != res->empty && line[j] != res->obstacle) {
                res->rows = i + 1;
                free(line);
                free_map(res);
                return NULL;
            }
        }
        res->map[i] = line;
        line = NULL;
    }

    free(line);
    return res;
}

void bsq(FILE *stream) {
    t_map *map = parse_map(stream);
    if (!map) {
        fprintf(stderr, "map error\n");
        return;
    }

    t_solution sol;
    algo(map, &sol);

    int sol_i = sol.row - sol.size + 1;
    int sol_j = sol.col - sol.size + 1;
    for (int i = sol_i; i <= sol.row; ++i) {
        for (int j = sol_j; j <= sol.col; ++j) {
            map->map[i][j] = map->full;
        }
    }

    for (int i = 0; i < map->rows; ++i) {
        fprintf(stdout, "%s\n", map->map[i]);
    }
    free_map(map);
}
