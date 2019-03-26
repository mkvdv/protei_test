#pragma once

#include "../sockets/tcp_socket.h"

#include <cstdint>
#include <string>
#include <memory>
#include <netinet/in.h>

namespace protei_test {
  class TcpClient {
   public:
    TcpClient(in_addr addr, uint16_t port, size_t max_buf_size);
    ~TcpClient();
    void run();

   private:
    void loop();

   private:
    sock_t m_client_socket = -1;
    in_addr m_addr;
    uint16_t m_port;
    const size_t MAX_BUF_SIZE;
  };
} // namespace protei_test
