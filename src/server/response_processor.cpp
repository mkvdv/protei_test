#include "response_processor.h"

#include <vector>
#include <algorithm>

namespace protei_test {
  std::string ResponseProcessor::create_response(const char *recieved_buf, size_t size) const {
      std::string res;
      std::vector<uint64_t> numbers;
      uint64_t sum = 0;
      const int BASE = 10;

      const char *buf_begin = recieved_buf;
      const char *buf_end = std::find(recieved_buf, recieved_buf + size, '\n');
      while (buf_begin != buf_end) {
          if (isdigit(*buf_begin)) {
              char *end;
              uint64_t extracted = std::strtoull(buf_begin, &end, BASE);
              buf_begin = end;

              sum += extracted;
              numbers.push_back(extracted);
          } else {
              ++buf_begin;
          }
      }

      std::sort(numbers.begin(), numbers.end());
      for (auto it = numbers.begin(); it != numbers.end(); ++it) {
          if (it != numbers.begin()) {
              res += ' ';
          }
          res += std::to_string(*it);
      }
      res += '\n' + std::to_string(sum);
      res += '\n'; // todo may be remove

      return res;
  }
} // namespace protei_test
