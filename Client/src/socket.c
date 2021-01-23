#include "../inc/header.h"

void Socket() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(PORT);
    inet_pton(AF_INET, ADDR, &adr.sin_addr);
    if (connect(client_socket, (struct sockaddr *) &adr, sizeof adr) == -1) {
        printf("DISCONNECT\n");
        close(client_socket);   
    }
}