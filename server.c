#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080),
        .sin_addr.s_addr = INADDR_ANY
    };

    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock, 10);
    
    int client_fd = accept(sock, 0, 0);  // file descriptor of client socket

    char buffer[256] = {0};
    recv(client_fd, buffer, 256, 0);

    // GET /index.html
    char *f = buffer + 5;
    *strchr(f, ' ') = 0;
    int opened_fd = open(f, O_RDONLY);   // opened file descriptor
    sendfile(client_fd, opened_fd, 0, 256);
    close(opened_fd);
    close(sock);

    return 0;
}
