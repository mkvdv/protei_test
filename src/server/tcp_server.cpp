#include "tcp_server.h"
#include "../sockets/check_errors.h"

#include <iostream>
#include <cstring>

namespace protei_test {
  TcpServer::TcpServer(uint16_t port, std::unique_ptr<IResponseProcessor> &&processor, size_t max_buf_size)
      : m_port(port), m_processor(std::move(processor)), MAX_BUF_SIZE(max_buf_size) {}

  TcpServer::~TcpServer() {
      // all unclosed sockets are in epoll and will be closed by os at exit
      // so no need to close anything here?
  }

  void TcpServer::set_running(bool value) {
      m_running = value;
  }

  void TcpServer::run() {
      // bind and begin listen here
      m_server_socket = TcpSocket::create();
      TcpSocket::set_nonblocking(m_server_socket);
      TcpSocket::start_listen(m_server_socket, m_port);

      epoll_desc_t epoll_desc = Epoll::create();
      Epoll::add_to_epoll(m_server_socket, epoll_desc);

      loop(epoll_desc);
  }

  void TcpServer::loop(epoll_desc_t epoll_desc) {
      std::unique_ptr<char[]> buf = std::make_unique<char[]>(MAX_BUF_SIZE);

      // server main loop
      const int MAX_EVENTS = 32;
      while (m_running) {
          epoll_event events[MAX_EVENTS];
          int events_count = Epoll::epoll_wait(epoll_desc, events, MAX_EVENTS);

          for (int i = 0; i != events_count; ++i) {
              std::cout << "some event " << events_count << std::endl;
              if (events[i].data.fd == m_server_socket) {
                  sock_t client_socket = TcpSocket::accept(m_server_socket);
                  TcpSocket::set_nonblocking(client_socket);
                  Epoll::add_to_epoll(client_socket, epoll_desc);
              } else {
                  sock_t client_socket = events[i].data.fd;
                  answer_response(client_socket, buf.get(), MAX_BUF_SIZE);
              }
          }
      }
  }

  void TcpServer::answer_response(sock_t client_socket, char *read_buffer, size_t buffer_size) {
      ssize_t recv_res = recv(client_socket, read_buffer, buffer_size, MSG_NOSIGNAL);

      if (recv_res < 0) {                 /* read error? */
          if (errno == EINTR) {           /* interrupted? or it can't occur cause of MSG_NOSIGNAL ? */
              return;                     /* read later */
          }
          check_or_throw(recv_res, "recv error");

      } else if (recv_res == 0) {
          if (errno != EAGAIN) {   /* EOF? 0 and not interrupted by signal */
              TcpSocket::shut_n_close(client_socket); // no need to remove from epoll, os will be done by os?
              return;
          } else { // if errno == EAGAIN => do nothing, will try again later after new epoll notification
              return;
          }
      }

      // else recv_res > 0
      std::string response = m_processor->create_response(read_buffer, static_cast<size_t>(recv_res));
      TcpSocket::send_data(client_socket, response.c_str(), response.length());
  }
} // namespace protei_test
