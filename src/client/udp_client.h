#pragma once

#include "../sockets/udp_socket.h"

#include <cstdint>
#include <string>
#include <memory>
#include <netinet/in.h>

namespace protei_test {
  class UdpClient {
   public:
    UdpClient(in_addr addr, uint16_t port, size_t max_buf_size);
    ~UdpClient();
    void run();

   private:
    void loop(sockaddr_in &addr);

   private:
    sock_t m_client_socket = -1;
    in_addr m_addr;
    uint16_t m_port;
    const size_t MAX_BUF_SIZE;
  };
} // namespace protei_test
