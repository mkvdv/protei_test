#include "udp_socket.h"
#include "check_errors.h"
#include <unistd.h>
#include <cstring>

namespace protei_test {
  sock_t UdpSocket::create() {
      sock_t sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      check_or_die(sock, "Can't create udp socket!");
      return sock;
  }

  void UdpSocket::bind(sock_t sock, uint16_t port) {
      UdpSocket::set_sock_opt(sock, SO_REUSEADDR);

      sockaddr_in server_sa{};
      memset(&server_sa, 0, sizeof(server_sa));
      server_sa.sin_family = AF_INET;
      server_sa.sin_port = htons(port);
      server_sa.sin_addr.s_addr = htonl(INADDR_ANY);

      check_or_die(::bind(sock, (sockaddr *) (&server_sa), sizeof(server_sa)), "server get_socket bind failed");
  }

  void UdpSocket::set_sock_opt(sock_t sock, int opts) {
      const int on_flag = 1;
      check_or_throw(setsockopt(sock, SOL_SOCKET, opts, &on_flag, sizeof(on_flag)),
                     "Can't setsockopt");
  }

  size_t UdpSocket::receive_data(sock_t sock, void *buf, const size_t buf_size, int flags,
                                 sockaddr_in *addr, socklen_t *socklen) {
      while (true) {
          ssize_t recieved = recvfrom(sock, buf, buf_size, flags, (sockaddr *) addr, socklen);
          if (recieved < 0) {                 /* read error? */
              if (errno == EINTR) {           /* interrupted? or it can't occur cause of MSG_NOSIGNAL ? */
                  continue;                   /* read again */
              }
              check_or_throw(recieved, "recvfrom error");
          } else { // recieved > 0
              return static_cast<size_t>(recieved);
          }
      }
  }

  void UdpSocket::send_data(sock_t sock, const void *buf, const size_t buf_size, int flags,
                            const sockaddr_in *addr, socklen_t socklen) {
      ssize_t sended = sendto(sock, buf, buf_size, flags, (const sockaddr *) (addr), socklen);
      check_or_throw(sended, "sendto error");
      if (sended != static_cast<ssize_t>(buf_size)) {
          check_or_throw(-1, "Can't send response properly"); // todo maybe remove
      }
  }

  void UdpSocket::close(sock_t sock) {
      check_or_throw(::close(sock), "Can't close, what?");
  }

} // namespace protei_test
