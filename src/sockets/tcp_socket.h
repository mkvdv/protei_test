#pragma once
#include <sys/types.h>
#include <exception>
#include <string>
#include <sys/epoll.h>
#include <netinet/in.h>

#include "types.h"

namespace protei_test {
  class Epoll {
   public:
    static epoll_desc_t create();
    static void add_to_epoll(sock_t sock, int epoll_desc);
    static int epoll_wait(epoll_desc_t epoll_desc, epoll_event *events, int max_events);
  };

  class TcpSocket {
   public:
    static sock_t create();
    static void set_nonblocking(sock_t sock);
    static void connect(sock_t sock, in_addr addr, uint16_t port);
    static sock_t accept(sock_t listener);
    static void start_listen(sock_t sock, uint16_t port);
    static void shut_n_close(sock_t sock);
    static void send_data(sock_t sock, const void *buf, size_t buf_size);
    static size_t read_blocking(sock_t sock, void *buf, size_t buf_size);
  };
} // namespace protei_test
