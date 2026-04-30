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

void exit_error(char *msg, t_client *client_lst) {
    t_client *tmp;

    tmp = client_lst;
    while (client_lst) {
        tmp = tmp->next;
        close(client_lst->fd);
        free(client_lst->buffer);
        free(client_lst);
        client_lst = tmp;
    }
    sprintf(stderr, msg);
    exit(1);
}

void send_message(char *msg, t_client *client_lst, t_client *sender) {
    while (client_lst) {
        if (client_lst != sender && FD_ISSET(client_lst->fd, &fd_wr)) {
            send(client_lst->fd, msg, strlen(msg), 0);
        }
        client_lst = client_lst->next;
    }
}


t_client *add_new_client(t_client *client_lst, int connfd, int client_id) {
    t_client *new, *last;
    char str[100];

    new = malloc(sizeof(t_client));
    if (!new) {
        exit_error("Fatal error\n", client_lst);
    }
    new->id = client_id;
    new->fd = connfd;
    new->buffer = NULL;
    new->next = NULL;
    FD_SET(connfd, &fd_all);
    sprintf(str, "server: client %d just arrived\n", client_id);
    send_message(str, client_lst, new);
    if (!client_lst) {
        return(new);
    }
    last = client_lst;
    while (last->next) {
        last = last->next;
    }
    last->next = new;
    return client_lst;
}

char *str_join(char *str1, char *str2) {
    char *new;
    int len;

    if (str1 == 0) {
        len = 0;
    } else {
        len = strlen(str1);
    }
    new = malloc(sizeof(char *) * (len + strlen(str2) + 1));
    new[0] = 0;
    if (str1 != 0) {
        strcat(new, str1);
    }
    free(str1);
    strcat(new, str2);
    return new;
}

ssize_t recieve_message(t_client *client_lst, t_client *curr) {
    ssize_t ret;
    char buffer[4097];

    ret = recv(curr->fd, buffer, 4096, 0);
    if (ret <= 0) {
        return ret;
    }
    buffer[ret] = 0;
    curr->buffer = str_join(curr->buffer, buffer);
    if (!curr->buffer) {
        exit_error("Fatal error\n", client_lst);
    }
    return ret;
}

t_client *remove_client(t_client *client_lst, t_client *curr_client) {
    char str[100];
    t_client *prev;

    if (client_lst == curr_client) {
        client_lst = client_lst->next;
    } else {
        prev = client_lst;
        while (prev->next != curr_client) {
            prev = prev->next;
        }
        prev->next = curr_client->next;
    }
    sprintf(str, "server: client %d just left\n", curr_client->id);
    send_message(str, client_lst, curr_client);
    FD_CLR(curr_client->fd, &fd_all);
    close(curr_client->fd);
    free(curr_client->buffer);
    free(curr_client);
    return (client_lst);
}

ssize_t extract_message(char **buf, char **msg) {
    char *new;
    int i;

    *msg = 0;
    if (*buf == 0) {
        return 0;
    }
    i = 0;
    while ((*buf)[i]) {
        if ((*buf)[i] == '\n') {
            new = calloc(1, sizeof(*new) * (strlen(*buf + i + 1) + 1));
            if (new == 0) {
                return -1;
            }
            strcpy(new, *buf + i + 1);
            *msg = *buf;
            (*msg)[i + 1] = 0;
            *buf = new;
            return 1;            
        }
        i++;
    }
    return 0;
}

t_client *handle_clients(t_client *client_lst) {
    char *msg, *str;
    ssize_t ret;
    t_client *it, *curr_client;

    it = client_lst;
    while (it) {
        curr_client = it;
        it = curr_client->next;
        if (FD_ISSET(curr_client->fd, &fd_rd)) {
            ret = recieve_message(client_lst, curr_client);
            if (ret == 0) {
                client_lst = remove_client(client_lst, curr_client);
            } else if (ret > 0) {
                while ((ret = extract_message(&curr_client->buffer, &msg)) ==  1) {
                    if (!(str = malloc(sizeof(char) * (20 + strlen(msg))))) {
                        exit_error("Fatal error\n", client_lst);
                    }
                    sprintf(str, "client %d: %s", curr_client->id, msg);
                    send_message(str, client_lst, curr_client);
                    free(msg);
                    free(str);
                }
                if (ret == -1) {
                    exit_error("Fatal error\n", client_lst); 
                }
            }
        }
    }
    return client_lst;
}

void handle_server(int sockfd) {
    int connfd, maxfd, client_id;
    t_client *client_lst;

    FD_ZERO(&fd_all);
    FD_SET(sockfd, &fd_all);
    client_lst = NULL;
    maxfd = sockfd;
    client_id = 0;
    while (1) {
        fd_rd = fd_all;
        fd_wr = fd_all;
        if (select(maxfd + 1, &fd_rd, &fd_wr, NULL, NULL) < 0) {
            continue;
        }

        if (FD_ISSET(sockfd, &fd_rd)) {
            connfd = accept(sockfd, NULL, NULL);
            if (connfd >= 0) {
                client_lst = add_new_client(client_lst, connfd, client_id);
                maxfd = connfd > maxfd ? connfd : maxfd;
                ++client_id;
            }
        }
        client_lst = handle_clients(client_lst);
    }
}

int main(int ac, char **av) {
    int port;
    int sockfd;
    struct sockaddr_in address;

    if (ac < 2) {
        exit_error("Wrong number of arguments\n", NULL);
    }
    port = atoi(av[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        exit_error("Fatal error\n", NULL);
    }
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = port;

    if (bind(sockfd, (const struct sockaddr *)&address, sizeof(address))) {
        exit_error("Fatal error\n", NULL);
    }
    if (listen(sockfd, 10) != 0) {
        exit_error("Fatal error\n", NULL);
    }

    handle_server(sockfd);

    return 0;
}