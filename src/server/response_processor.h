#pragma once

#include <string>

namespace protei_test {
  class IResponseProcessor {
   public:
    virtual std::string create_response(const char *recieved_buf, size_t size) const = 0;
    virtual ~IResponseProcessor() = default;
  };

  // todo may be better static class? or simple function?
  class ResponseProcessor : public IResponseProcessor {
   public:
    std::string create_response(const char *recieved_buf, size_t size) const override;
  };
} // namespace protei_test
