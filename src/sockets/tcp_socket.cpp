#include "check_errors.h"
#include "tcp_socket.h"

#include <fcntl.h>
#include <unistd.h>
#include <cstring>

namespace protei_test {
  namespace {
    int set_nonblock(sock_t sock) {
#ifdef O_NONBLOCK
        int flags = fcntl(sock, F_GETFL, 0);
        if (flags == -1) {
            flags = 0;
        }
        return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#else
        int flags = 1;
        return ioctl(sock, FIONBIO, &flags);
#endif
    }
  } // anonymous namespace

  /*-------------------------------------------------------------------------*/
  sock_t TcpSocket::create() {
      sock_t sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      check_or_throw(sock, "Can't create get_socket!");
      return sock;
  }

  void TcpSocket::set_nonblocking(sock_t sock) {
      check_or_throw(set_nonblock(sock), "Can't set get_socket to nonblocking mode");
  }

  void TcpSocket::start_listen(sock_t sock, uint16_t port) {
      const int on_flag = 1;
      check_or_throw(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on_flag, sizeof(on_flag)),
                     "Can't setsockopt SO_REUSEADDR");

      sockaddr_in server_sa{};
      memset(&server_sa, 0, sizeof(server_sa));
      server_sa.sin_family = AF_INET;
      server_sa.sin_port = htons(port);
      server_sa.sin_addr.s_addr = htonl(INADDR_ANY);

      check_or_throw(bind(sock, (sockaddr *) (&server_sa), sizeof(server_sa)),
                     "Server socket bind failed");

      check_or_throw(listen(sock, SOMAXCONN), "Listen server socket failed");
  }

  sock_t TcpSocket::accept(sock_t listener) {
      int client_socket = ::accept(listener, nullptr, nullptr);
      check_or_throw(client_socket, "accept failed");
      check_or_throw(set_nonblock(client_socket), "Can't set get_socket to nonblocking mode");
      return client_socket;
  }

  void TcpSocket::send_data(sock_t sock, const void *buf, size_t buf_size) {
      ssize_t rc = send(sock, buf, buf_size, MSG_NOSIGNAL);
      check_or_die(rc, "Can't send response");

      if (rc != static_cast<ssize_t>(buf_size)) {
          check_or_die(-1, "Can't send the whole response?"); // todo maybe remove
      }
  }
  void TcpSocket::shut_n_close(sock_t sock) {
      check_or_throw(::shutdown(sock, SHUT_RDWR), "Can't shutdown socket, what?");
      check_or_throw(::close(sock), "Can't close socket, what?");
  }
  void TcpSocket::connect(sock_t sock, in_addr addr, uint16_t port) {
      sockaddr_in server_sa;
      memset(&server_sa, 0, sizeof(server_sa));
      server_sa.sin_family = AF_INET;
      server_sa.sin_port = htons(port);
      server_sa.sin_addr = addr;

      check_or_throw(::connect(sock, (sockaddr *) &server_sa, sizeof(server_sa)),
                     "server get_socket connect failed");
  }
  size_t TcpSocket::read_blocking(sock_t sock, void *buf, size_t buf_size) {
      // get response
      while (true) {
          ssize_t recieved = recv(sock, buf, buf_size, MSG_NOSIGNAL);
          // handle errors
          if (recieved < 0) {                 /* read error? */
              if (errno == EINTR) {           /* interrupted? or it can't occur cause of MSG_NOSIGNAL ? */
                  continue;                   /* read later */
              }
              check_or_throw(recieved, "recv error"); // throw
          } else { // recieved > 0
              return static_cast<size_t>(recieved);
          }
      }
  }

  /*-------------------------------------------------------------------------*/
  epoll_desc_t Epoll::create() {
      int epoll_desc = epoll_create1(0);
      check_or_throw(epoll_desc, "Can't create epoll");
      return epoll_desc;
  }

  void Epoll::add_to_epoll(sock_t sock, int epoll_desc) {
      epoll_event epoll_event;
      memset(&epoll_event, 0, sizeof(epoll_event));
      epoll_event.data.fd = sock;
      epoll_event.events = EPOLLIN;
      check_or_throw(epoll_ctl(epoll_desc, EPOLL_CTL_ADD, sock, &epoll_event),
                     "Can't add get_socket to epoll");
  }

  int Epoll::epoll_wait(epoll_desc_t epoll_desc, epoll_event *events, int max_events) {
      int events_count = ::epoll_wait(epoll_desc, events, max_events, -1); // endless waiting
      check_or_throw(events_count, "epoll_wait failed");
      return events_count;
  }
} // namespace protei_test
