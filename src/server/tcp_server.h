#pragma once

#include "response_processor.h"
#include "../sockets/tcp_socket.h"

#include <cstdint>
#include <string>
#include <memory>

namespace protei_test {
  class TcpServer {
   public:
    TcpServer(uint16_t port, std::unique_ptr<IResponseProcessor> &&processor, size_t max_buf_size);
    ~TcpServer();

    void run();
    void set_running(bool value);

   private:
    void loop(epoll_desc_t epoll_desc);
    void answer_response(sock_t client_socket, char *read_buffer, size_t buffer_size);

   private:
    sock_t m_server_socket = -1;
    bool m_running = true;
    uint16_t m_port;
    std::unique_ptr<IResponseProcessor> m_processor;
    const size_t MAX_BUF_SIZE;
  };
} // namespace protei_test
