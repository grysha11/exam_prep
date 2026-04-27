#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

typedef struct s_client {
    int id;
    int fd;
    char *buffer;
    struct s_client *next;
} t_client;

fd_set fd_all;
fd_set fd_rd;
fd_set fd_wr;

int main(int ac, char **av) {
    
    return 0;
}