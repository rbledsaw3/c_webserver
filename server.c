#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>

enum {
    PORT = 8080,
    MAX_CONNECTIONS = 10,
    BUFFER_SIZE = 256,
    PADDING_SIZE = 5
};

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    (void) bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock, MAX_CONNECTIONS);
    
    int client_fd = accept(sock, 0, 0);  // file descriptor of client socket

    char buffer[BUFFER_SIZE] = {0};
    recv(client_fd, buffer, BUFFER_SIZE, 0);

    // GET /index.html
    char *file = buffer + PADDING_SIZE;
    *strchr(file, ' ') = 0;
    int opened_fd = open(file, O_RDONLY);   // opened file descriptor
    sendfile(client_fd, opened_fd, 0, BUFFER_SIZE);
    close(opened_fd);
    close(sock);

    return 0;
}
