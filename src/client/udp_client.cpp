#include "udp_client.h"
#include "../sockets/check_errors.h"
#include <unistd.h>
#include <iostream>
#include <cstring>

namespace protei_test {
  protei_test::UdpClient::UdpClient(in_addr addr, uint16_t port, size_t max_buf_size)
      : m_addr(addr), m_port(port), MAX_BUF_SIZE(max_buf_size) {}

  protei_test::UdpClient::~UdpClient() {
      check_or_die(::close(m_client_socket), "Can't close socket in UdpClient::~UdpClient()");
  }

  void protei_test::UdpClient::run() {
      m_client_socket = UdpSocket::create();

      sockaddr_in server_sa;
      memset(&server_sa, 0, sizeof(server_sa));
      server_sa.sin_port = htons(m_port);
      server_sa.sin_family = AF_INET;
      server_sa.sin_addr = m_addr;

      loop(server_sa);

  }

  void protei_test::UdpClient::loop(sockaddr_in &addr) {
      std::unique_ptr<char[]> buf = std::make_unique<char[]>(MAX_BUF_SIZE + 1);
      std::string input;
      int flags = MSG_NOSIGNAL;

      while (std::getline(std::cin, input)) { // till eof?
          // send message
          input += '\n'; // as marker of end
          UdpSocket::send_data(m_client_socket, input.c_str(), input.length(), flags, &addr, sizeof(addr));

          // get response
          size_t recieved = UdpSocket::receive_data(m_client_socket, buf.get(), MAX_BUF_SIZE, flags,
                                                    nullptr, nullptr);
          buf[recieved] = '\0'; // for printting

          // print readed
          std::cout << buf.get() << std::flush;
      }
  }

} // namespace protei_test
