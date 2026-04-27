#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_game {
    int i;
    int j;
    char **map;
    char **next_map;
    int pen_i;
    int pen_j;
    int is_drawing;
} t_game;

char **new_map(int map_i, int map_j) {
    char **map = calloc(map_i, sizeof(char *));
    if (!map) {
        return NULL;
    }

    for (int i = 0; i < map_i; i++) {
        map[i] = calloc(map_j, sizeof(char));
        if (!map[i]) {
            while (--i >= 0) {
                free(map[i]);
            }
            free(map);
            return NULL;
        }
    }
    return map;
}

void free_maps(t_game *game) {
    if (game->map) {
        for (int i = 0; i < game->i; i++) free(game->map[i]);
        free(game->map);
    }
    if (game->next_map) {
        for (int i = 0; i < game->i; i++) free(game->next_map[i]);
        free(game->next_map);
    }
}

void mark(t_game *game) {
    if (game->pen_i >= 0 && game->pen_i < game->i && game->pen_j >= 0 && game->pen_j < game->j) {
        game->map[game->pen_i][game->pen_j] = 1;
    }
}

void parse_input(t_game *game) {
    char c;

    while (read(0, &c, 1) > 0) {
        if (c == 'w' && game->pen_i > 0) {
            game->pen_i--;
        } else if (c == 's' && game->pen_i < game->i - 1) {
            game->pen_i++;
        } else if (c == 'a' && game->pen_j > 0) {
            game->pen_j--;
        } else if (c == 'd' && game->pen_j < game->j - 1) {
            game->pen_j++;
        } else if (c == 'x') {
            game->is_drawing = !game->is_drawing;
            if (game->is_drawing) {
                mark(game);
            }
        }

        if (game->is_drawing && (c == 'w' || c == 'a' || c == 's' || c == 'd')) {
            mark(game);
        }
    }
}

int count_n(t_game *game, int i, int j) {
    int count = 0;
    int ni, nj;

    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue;
            ni = i + di;
            nj = j + dj;
            if (ni >= 0 && ni < game->i && nj >= 0 && nj < game->j) {
                if (game->map[ni][nj] == 1) {
                    count++;
                }
            }
        }
    }

    return count;
}

void simulate(t_game *game) {
    int neighbours;

    for (int i = 0; i < game->i; i++) {
        for (int j = 0; j < game->j; j++) {
            neighbours = count_n(game, i, j);

            if (game->map[i][j] == 1) {
                if (neighbours == 2 || neighbours == 3) {
                    game->next_map[i][j] = 1;
                } else {
                    game->next_map[i][j] = 0;
                }
            } else {
                if (neighbours == 3) {
                    game->next_map[i][j] = 1;
                } else {
                    game->next_map[i][j] = 0;
                }
            }
        }
    }

    for (int i = 0; i < game->i; i++) {
        for (int j = 0; j < game->j; j++) {
            game->map[i][j] = game->next_map[i][j];
        }
    }
}

void print_map(t_game *game) {
    for (int i = 0; i < game->i; i++) {
        for (int j = 0; j < game->j; j++) {
            if (game->map[i][j] == 1) {
                putchar('O');
            } else {
                putchar(' ');
            }
        }
        putchar('\n');
    }
}

int main(int ac, char **av) {
    t_game game;
    int iter;

    if (ac != 4) {
        return 0;
    }

    game.j = atoi(av[1]);
    game.i = atoi(av[2]);
    iter = atoi(av[3]);
    game.pen_i = 0;
    game.pen_j = 0;
    game.is_drawing = 0;

    game.map = new_map(game.i, game.j);
    game.next_map = new_map(game.i, game.j);
    if (!game.map || !game.next_map) {
        return 1;
    }

    parse_input(&game);

    for (int i = 0; i < iter; i++) {
        simulate(&game);
    }

    print_map(&game);
    free_maps(&game);
    return 0;
}