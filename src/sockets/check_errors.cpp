#include "check_errors.h"
#include <iostream>
#include <cstring>

namespace protei_test {
  socket_error::socket_error(std::string &&mesg)
      : m_message(std::move(mesg)) {}
  const char *socket_error::what() const noexcept {
      return m_message.c_str();
  }

  void check_or_throw(ssize_t rc, const char *error_message) {
      if (rc < 0) {
          std::string mesg{};
          if (error_message) {
              mesg = std::string{error_message} + ", errno: " + strerror(errno) + '\n';
          } else {
              mesg = std::string{strerror(errno)} + '\n';
          }
          throw socket_error(std::move(mesg));
      }
  }

  void check_or_die(ssize_t rc, const char *error_message) {
      if (rc < 0) {
          if (error_message) {
              std::cerr << std::string{error_message} + ", errno: " + strerror(errno) << std::endl;
          } else {
              std::cerr << std::string{strerror(errno)} << std::endl;;
          }
          std::exit(EXIT_FAILURE);
      }
  }
} // namespace protei_test
