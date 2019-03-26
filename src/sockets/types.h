#pragma once

namespace protei_test {
  enum class Protocol {
    TCP, UDP
  };
  using sock_t = int;
  using epoll_desc_t  = int;
}