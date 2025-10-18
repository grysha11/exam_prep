#include "bsq.h"

int main(int ac, char **av) {
    if (ac != 2) {
        fprintf(stderr, "map error\n");
        return 1;
    }

    FILE *file = fopen(av[1], "r");
    if (!file) {
        fprintf(stderr, "map error\n");
        return 1;
    }

    bsq(file);

    fclose(file);
    return 0;
}
