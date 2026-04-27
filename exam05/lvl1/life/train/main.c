#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_game {
    int i;
    int j;
    int iter;
    char **map;
    char **next_map;
    int pen_i;
    int pen_j;
    int pen_is_drawing;
} t_game;

char **new_map(int s_i, int s_j) {
    char **map = calloc(s_i, sizeof(char *));
    for (int i = 0; i < s_i; i++) {
        map[i] = calloc(s_j, sizeof(char));
    }

    return map;
}

void free_maps(t_game *game) {
    if (game->map) {
        for (int i = 0; i < game->i; i++) {
            free(game->map[i]);
        }
        free(game->map);
    }
    if (game->next_map) {
        for (int i = 0; i < game->i; i++) {
            free(game->next_map[i]);
        }
        free(game->next_map);
    }
}

void mark(t_game *game) {
    game->map[game->pen_i][game->pen_j] = 1;
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
            game->pen_is_drawing = !game->pen_is_drawing;
        }

        if (game->pen_is_drawing) {
            mark(game);
        }
    }
}

int count_n(t_game *game, int i, int j) {
    int res = 0;
    int ni, nj;

    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) {
                continue;
            }
            ni = di + i;
            nj = dj + j;
            if (ni >= 0 && ni < game->i && nj >= 0 && nj < game->j) {
                if (game->map[ni][nj] == 1) {
                    res++;
                }
            }
        }
    }

    return res;
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

int main(int ac, char **av) {
    if (ac != 4) {
        return 1;
    }
    t_game game;

    game.i = atoi(av[2]);
    game.j = atoi(av[1]);
    game.iter = atoi(av[3]);
    game.pen_i = 0;
    game.pen_j = 0;
    game.pen_is_drawing = 0;
    game.map = new_map(game.i, game.j);
    game.next_map = new_map(game.i, game.j);
    if (!game.map || !game.next_map) {
        free_maps(&game);
        return 1;
    }

    parse_input(&game);

    for (int i = 0; i < game.iter; i++) {
        simulate(&game);
    }

    for (int i = 0; i < game.i; i++) {
        for (int j = 0; j < game.j; j++) {
            if (game.map[i][j] == 0) {
                putchar(' ');
            } else if (game.map[i][j] == 1) {
                putchar('O');
            }
        }
        putchar('\n');
    }

    free_maps(&game);
}