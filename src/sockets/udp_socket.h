#pragma once

#include "types.h"
#include <netinet/in.h>

namespace protei_test {
  class UdpSocket {
   public:
    static sock_t create();
    static void bind(sock_t sock, uint16_t port);
    static void set_sock_opt(sock_t sock, int opts);
    static size_t receive_data(sock_t sock, void *buf, size_t buf_size, int flags,
                               sockaddr_in *addr, socklen_t *socklen);
    static void send_data(sock_t sock, const void *buf, size_t buf_size, int flags,
                          const sockaddr_in *addr, socklen_t socklen);
    static void close(sock_t sock);
  };
} // namespace protei_test
