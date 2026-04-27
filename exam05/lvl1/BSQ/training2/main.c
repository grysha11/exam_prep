#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_map {
    int i;
    int j;
    char empty;
    char obstacle;
    char full;
    char **map;
} t_map;

typedef struct s_solution {
    int i;
    int j;
    int size;
} t_solution;

void free_map(t_map *map) {
    if (map->map) {
        for (int i = 0; i < map->i; ++i) {
            free(map->map[i]);
        }
        free(map->map);
    }
}

void parse_map(t_map *map, FILE *file) {
    if (fscanf(file, "%d %c %c %c", &map->i, &map->empty, &map->obstacle, &map->full) != 4 || map->empty == map->full || map->empty == map->obstacle || map->obstacle == map->full) {
        return;
    }

    map->map = calloc(map->i, sizeof(char *));
    if (!map->map) {
        return;
    }

    char *dummy = NULL;
    size_t dummy_size;
    getline(&dummy, &dummy_size, file);
    free(dummy);

    char *line = NULL;
    size_t size;
    ssize_t nread;
    for (int i = 0; i < map->i; ++i) {
        nread = getline(&line, &size, file);
        if (nread <= 1 || line[nread - 1] != '\n') {
            map->i = i;
            free_map(map);
            free(line);
            return;
        }
        line[nread - 1] = '\0';

        if (i == 0) {
            map->j = nread - 1;
            if (map->j == 0) {
                map->i = i + 1;
                free_map(map);
                free(line);
                return;
            }
        } else if (nread - 1 != map->j) {
            map->i = i + 1;
            free_map(map);
            free(line);
            return;
        }

        for (int j = 0; j < map->j; ++j) {
            if (line[j] != map->empty && line[j] != map->obstacle) {
                map->i = i + 1;
                free_map(map);
                free(line);
                return;
            }
        }

        map->map[i] = line;
        line = NULL;
    }
    free(line);
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

void algo(t_map *map, t_solution *sol) {
    sol->size = 0;
    int **int_map = calloc(map->i, sizeof(int *));
    for (int i = 0; i < map->i; ++i) {
        int_map[i] = calloc(map->j, sizeof(int));
    }

    for (int i = 0; i < map->i; ++i) {
        for (int j = 0; j < map->j; j++) {
            if (map->map[i][j] == map->obstacle) {
                int_map[i][j] = 0;
            } else if (i == 0 || j == 0) {
                int_map[i][j] = 1;
            } else {
                int_map[i][j] = 1 + min3(int_map[i][j - 1], int_map[i - 1][j], int_map[i - 1][j - 1]);
            }

            if (int_map[i][j] > sol->size) {
                sol->size = int_map[i][j];
                sol->i = i;
                sol->j = j;
            }
        }
    }

    for (int i = 0; i < map->i; ++i) {
        free(int_map[i]);
    }
    free(int_map);
}

void bsq(FILE *file) {
    t_map map = {0};
    t_solution sol;

    parse_map(&map, file);
    if (!map.map) {
        fprintf(stderr, "map error\n");
        return;
    }

    algo(&map, &sol);

    int sol_i = sol.i - sol.size + 1;
    int sol_j = sol.j - sol.size + 1;

    for (int i = sol_i; i <= sol.i; ++i) {
        for (int j = sol_j; j <= sol.j; ++j) {
            map.map[i][j] = map.full;
        }
    }

    for (int i = 0; i < map.i; ++i) {
        fprintf(stdout, "%s\n", map.map[i]);
    }

    free_map(&map);
}

int main(int ac, char **av) {
    FILE *file;

    if (ac == 1) {
        bsq(stdin);
    } else {
        for (int i = 1; i < ac; ++i) {
            file = fopen(av[i], "r");
            if (!file) {
                fprintf(stderr, "map error\n");
            } else {
                bsq(file);
                fclose(file);
            }
        }
    }
}