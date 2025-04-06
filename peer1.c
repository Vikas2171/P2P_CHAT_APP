#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>

#define MAX 80
#define PORT 8080          // Port this peer listens on
#define PEER_IP "127.0.0.1"
#define PEER_PORT 8081
#define SA struct sockaddr

// Try connecting as a client
int try_connect() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    struct sockaddr_in peer_addr;
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(PEER_PORT);
    peer_addr.sin_addr.s_addr = inet_addr(PEER_IP);

    if (connect(sock, (SA*)&peer_addr, sizeof(peer_addr)) == 0) {
        printf("Connected to peer...\n");
        return sock;
    }

    close(sock);
    return -1;
}

// Start as a server
int start_server() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(server_fd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_fd, 1) != 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Waiting for peer connection...\n");

    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
    int conn_fd = accept(server_fd, (SA*)&cli, &len);
    if (conn_fd < 0) {
        perror("Accept failed");
        exit(1);
    }

    printf("Peer connected...\n");
    close(server_fd);
    return conn_fd;
}

int main() {
    int sockfd = try_connect();
    if (sockfd == -1) {
        printf("Starting server mode...\n");
        sockfd = start_server();
    }

    char buff[MAX];
    fd_set readfds;
    int maxfd = (sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO) + 1;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(sockfd, &readfds);

        int activity = select(maxfd, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select error");
            break;
        }

        // Check if data is available from peer
        if (FD_ISSET(sockfd, &readfds)) {
            memset(buff,0,MAX);
            int n = read(sockfd, buff, MAX);
            if (n <= 0 || strncmp(buff, "exit", 4) == 0) {
                printf("Peer disconnected...\n");
                break;
            }
            printf("From peer: %.*s", n, buff);
        }

        // Check if user typed something
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            memset(buff,0,MAX);
            if (fgets(buff, MAX, stdin) == NULL) {
                printf("EOF on input\n");
                break;
            }

            if (strncmp(buff, "exit", 4) == 0) {
                write(sockfd, buff, strlen(buff));
                printf("Exiting chat...\n");
                break;
            }

            write(sockfd, buff, strlen(buff));
        }
    }

    close(sockfd);
    return 0;
}
