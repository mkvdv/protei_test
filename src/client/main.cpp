#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <arpa/inet.h>
#include <iostream>

#include "tcp_client.h"
#include "udp_client.h"

namespace {
  class print_help_exception : public std::exception {};

  void set_up_arguments(const int argc, const char *const *argv,
                        in_addr &ip_addr, uint16_t &port, protei_test::Protocol &protocol) {
      namespace po = boost::program_options;
      std::string protocol_str;

      // Declare the supported options.
      po::options_description desc("Allowed options");
      desc.add_options()
          ("help", "produce help message")
          ("addr", po::value<std::string>()->required(), "set server port")
          ("port", po::value<unsigned short>(&port)->required(), "set server port")
          ("proto", po::value<std::string>(&protocol_str)->required(), "set server protocol: tcp or upd");

      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, desc), vm);

      if (vm.count("help")) {
          std::cout << desc << "\n";
          throw print_help_exception();
      }

      po::notify(vm); // after help checking

      if (protocol_str == "tcp") {
          protocol = protei_test::Protocol::TCP;
      } else if (protocol_str == "udp") {
          protocol = protei_test::Protocol::UDP;
      } else {
          throw std::invalid_argument("Wrong protocol name: use 'tcp' or 'udp'");
      }

      // validate ip
      const auto &addr_input = vm["addr"].as<std::string>();

      bool valid_ip = true;
      if (addr_input.length() <= INET_ADDRSTRLEN) { // can check
          char addr_buf[INET_ADDRSTRLEN] = {0};
          memcpy(addr_buf, addr_input.c_str(), addr_input.length());
          int rc = inet_pton(AF_INET, addr_buf, &ip_addr);
          if (rc != 1) {
              valid_ip = false;
          }
      } else {
          valid_ip = false;
      }

      if (!valid_ip) {
          throw std::invalid_argument("Invalid ip addr");
      }
  }
} // anonymous namespace



int main(int argc, char *argv[]) {
    try {
        uint16_t port;
        protei_test::Protocol protocol;
        in_addr ip_addr;
        const size_t MAX_BUF_SIZE = 1024;

        set_up_arguments(argc, argv, ip_addr, port, protocol);
        if (protocol == protei_test::Protocol::TCP) {
            protei_test::TcpClient client(ip_addr, port, MAX_BUF_SIZE);
            client.run();
        } else if (protocol == protei_test::Protocol::UDP) {
            protei_test::UdpClient client(ip_addr, port, MAX_BUF_SIZE);
            client.run();
        } else {
            std::cerr << "Unsupported protocol" << std::endl;
            return EXIT_FAILURE;
        }

    } catch (const print_help_exception &e) {
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Throwed something unexpected from set_up_arguments" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
