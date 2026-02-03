#include "bsq.h"

int main(int ac, char **av) {
    FILE *file;
    
    if (ac == 1) {
        bsq(stdin);
    } else {
        for (int i = 1; i < ac; i++) {
            file = fopen(av[i], "r");
            if (file) {
                bsq(file);
                fclose(file);
            } else {
                fprintf(stderr, "map error\n");
            }
        }
    }

    return 0;
}