#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "tcp_server.h"
#include "udp_server.h"

#include <iostream>

namespace {
  class print_help_exception : public std::exception {};

  void set_up_arguments(const int argc, const char *const *argv,
                        uint16_t &port, protei_test::Protocol &protocol) {
      namespace po = boost::program_options;
      std::string protocol_str;

      // Declare the supported options.
      po::options_description desc("Allowed options");
      desc.add_options()
          ("help", "produce help message")
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
  }
} // anonymous namespace

int main(int argc, char *argv[]) {
    try {
        uint16_t port;
        protei_test::Protocol protocol;
        const size_t MAX_BUF_SIZE = 1024;

        set_up_arguments(argc, argv, port, protocol);
        if (protocol == protei_test::Protocol::TCP) {
            protei_test::TcpServer server(port, std::make_unique<protei_test::ResponseProcessor>(), MAX_BUF_SIZE);
            server.run();
        } else if (protocol == protei_test::Protocol::UDP) {
            protei_test::UdpServer server(port, std::make_unique<protei_test::ResponseProcessor>(), MAX_BUF_SIZE);
            server.run();
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
