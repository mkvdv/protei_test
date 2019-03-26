#include "udp_server.h"
#include "../sockets/udp_socket.h"

namespace protei_test {
  protei_test::UdpServer::UdpServer(uint16_t port, std::unique_ptr<IResponseProcessor> &&processor, size_t max_buf_size)
      : m_port(port), m_processor(std::move(processor)), MAX_BUF_SIZE(max_buf_size) {}

  UdpServer::~UdpServer() {
      UdpSocket::close(m_server_socket); // but still unnecessary? (at exit it will closed anyway)
  }

  void UdpServer::set_running(bool value) {
      m_running = value;
  }

  void UdpServer::run() {
      m_server_socket = UdpSocket::create();
      UdpSocket::bind(m_server_socket, m_port);
      loop();
  }
  void UdpServer::loop() {
      sockaddr_in peer;
      socklen_t peerlen;

      std::unique_ptr<char[]> buf = std::make_unique<char[]>(MAX_BUF_SIZE);
      int flags = MSG_NOSIGNAL;

      while (m_running) {
          peerlen = sizeof(peer);
          size_t recieved = UdpSocket::receive_data(m_server_socket, buf.get(), MAX_BUF_SIZE, flags, &peer, &peerlen);
          std::string response = m_processor->create_response(buf.get(), recieved);
          UdpSocket::send_data(m_server_socket, response.c_str(), response.length(), flags, &peer, peerlen);
      }
  }

} // namespace protei_test
