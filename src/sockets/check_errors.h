#pragma once

#include <string>

namespace protei_test {
  class socket_error : public std::exception {
   public:
    explicit socket_error(std::string &&mesg);
    const char *what() const noexcept override;
   private:
    std::string m_message;
  };

  void check_or_throw(ssize_t rc, const char *error_message);
  void check_or_die(ssize_t rc, const char *error_message);
} // namespace protei_test
