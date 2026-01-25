#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "parser.h"

#define PROTOCOL_MAGIC 0xCAFEBABEu
#define MAX_MESSAGE_BODY_SIZE 256
#define MSG_USER_TEXT 300
#define BUFFER_SIZE 4096
#define MAX_CONNS 10000

struct message_header {
    uint32_t magic;
    uint32_t length;
    uint16_t type;
    uint16_t reserved;
} __attribute__((packed));

static volatile int keep_running = 1;
static int active_connections = 0;

void signal_handler(int sig) {
    if (sig == SIGINT) {
        keep_running = 0;
    }
}

struct connection {
    int fd;
    char buffer[BUFFER_SIZE];
    size_t buf_start;
    size_t buf_end;
};

static int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static int try_parse_message(struct connection* conn) {
    size_t available = conn->buf_end - conn->buf_start;
    if (available < sizeof(struct message_header)) {
        return 0;
    }

    struct message_header* hdr = (struct message_header*)(conn->buffer + conn->buf_start);
    uint32_t magic = ntohl(hdr->magic);
    uint32_t length = ntohl(hdr->length);
    uint16_t type = ntohs(hdr->type);

    if (magic != PROTOCOL_MAGIC || type != MSG_USER_TEXT || length > MAX_MESSAGE_BODY_SIZE) {
        return -1;
    }

    if (available < sizeof(struct message_header) + length) {
        return 0;
    }

    printf("📡 Raw: '");
    for (size_t i = 0; i < length; ++i) {
        unsigned char c = (unsigned char)conn->buffer[conn->buf_start + sizeof(struct message_header) + i];
        if (c >= 32 && c <= 126) {
            putchar(c);
        } else {
            printf("\\x%02x", c);
        }
    }
    printf("'\n");

    unsigned long long id;
    char name[64], email[64];
    if (parse_user_text_c(conn->buffer + conn->buf_start + sizeof(struct message_header),
                          length, &id, name, email) == 0) {
        printf("✅ Parsed: ID=%llu, Name='%s', Email='%s'\n", id, name, email);
    } else {
        printf("❌ Invalid\n");
    }

    conn->buf_start += sizeof(struct message_header) + length;
    return 1;
}

static int connection_handle_read(struct connection* conn) {
    while (1) {
        ssize_t n = read(conn->fd, conn->buffer + conn->buf_end,
                         BUFFER_SIZE - conn->buf_end);
        if (n > 0) {
            conn->buf_end += n;
            int result;
            while ((result = try_parse_message(conn)) > 0) {
                // keep parsing complete messages
            }
            if (result < 0) return -1;

            if (conn->buf_start > 0) {
                memmove(conn->buffer, conn->buffer + conn->buf_start,
                        conn->buf_end - conn->buf_start);
                conn->buf_end -= conn->buf_start;
                conn->buf_start = 0;
            }
            if (conn->buf_end == BUFFER_SIZE) {
                return -1;
            }
        } else if (n == 0) {
            // 🟢 Client closed — process any remaining complete messages!
            int result;
            while ((result = try_parse_message(conn)) > 0) {
                // process final messages
            }
            return -1;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            return -1;
        }
    }
    return 0;
}

int main() {
    signal(SIGINT, signal_handler);

    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(listen_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(listen_sock);
        return 1;
    }

    if (listen(listen_sock, SOMAXCONN) == -1) {
        perror("listen");
        close(listen_sock);
        return 1;
    }

    int flags = fcntl(listen_sock, F_GETFL, 0);
    fcntl(listen_sock, F_SETFL, flags | O_NONBLOCK);

    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        close(listen_sock);
        return 1;
    }

    struct epoll_event ev = {0};
    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        perror("epoll_ctl listen");
        close(listen_sock);
        close(epfd);
        return 1;
    }

    struct connection* connections = calloc(MAX_CONNS, sizeof(struct connection));
    if (!connections) {
        perror("calloc");
        close(listen_sock);
        close(epfd);
        return 1;
    }

    int next_id = 0;
    printf("🚀 Server running on 0.0.0.0:8080 (PID: %d)\n", getpid());
    fflush(stdout);

    struct epoll_event events[64];
    while (keep_running) {
        int nfds = epoll_wait(epfd, events, 64, 1000);
        if (nfds == -1) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == listen_sock) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(listen_sock, (struct sockaddr*)&client_addr, &client_len);
                if (client_fd == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
                    perror("accept");
                    continue;
                }

                flags = fcntl(client_fd, F_GETFL, 0);
                fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

                if (next_id >= MAX_CONNS) {
                    close(client_fd);
                    continue;
                }

                char ip_str[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
                printf("🔌 New connection from %s (active: %d)\n", ip_str, ++active_connections);
                fflush(stdout);

                struct connection* conn = &connections[next_id++];
                conn->fd = client_fd;
                conn->buf_start = 0;
                conn->buf_end = 0;

                ev.events = EPOLLIN | EPOLLRDHUP;
                ev.data.ptr = conn;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl add");
                    close(client_fd);
                    active_connections--;
                    next_id--;
                }
            } else {
                struct connection* conn = (struct connection*)events[i].data.ptr;
                if (events[i].events & (EPOLLERR | EPOLLRDHUP | EPOLLHUP)) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, conn->fd, NULL);
                    close(conn->fd);
                    active_connections--;
                    printf("🗑️ Connection closed (active: %d)\n", active_connections);
                    fflush(stdout);
                    continue;
                }

                if (connection_handle_read(conn) != 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, conn->fd, NULL);
                    close(conn->fd);
                    active_connections--;
                    printf("🗑️ Connection error (active: %d)\n", active_connections);
                    fflush(stdout);
                }
            }
        }
    }

    printf("\n🛑 Shutting down gracefully...\n");
    free(connections);
    close(epfd);
    close(listen_sock);
    return 0;
}
