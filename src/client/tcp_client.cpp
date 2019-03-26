#include <iostream>
#include <unistd.h>
#include "tcp_client.h"
#include "../sockets/check_errors.h"

namespace protei_test {
  TcpClient::TcpClient(in_addr addr, uint16_t port, size_t max_buf_size)
      : m_addr(addr), m_port(port), MAX_BUF_SIZE(max_buf_size) {}

  TcpClient::~TcpClient() {
      // manually, cause TcpSoclet::shut_n_close throw exception, and we are in dtor
      check_or_die(::shutdown(m_client_socket, SHUT_RDWR), "Can't shutdown socket in TcpClient::~TcpClient()");
      check_or_die(::close(m_client_socket), "Can't close socket in TcpClient::~TcpClient");
  }

  void TcpClient::run() {
      m_client_socket = TcpSocket::create();
      TcpSocket::connect(m_client_socket, m_addr, m_port);
      loop();
  }

  void TcpClient::loop() {
      std::unique_ptr<char[]> buf = std::make_unique<char[]>(MAX_BUF_SIZE + 1);
      std::string input;

      while (std::getline(std::cin, input)) {
          // send message
          input += '\n'; // as marker of end
          TcpSocket::send_data(m_client_socket, input.c_str(), input.length());

          // get response
          size_t recieved = TcpSocket::read_blocking(m_client_socket, buf.get(), MAX_BUF_SIZE);
          buf[recieved] = '\0'; // for printing

          // print response
          std::cout << buf.get() << std::flush;
      }
  }

} // namespace protei_test
