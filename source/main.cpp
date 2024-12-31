#include <Core/Server.h>

int main(int argc, char *argv[]) {
  try {
    HappyFlood::Core::Server server{argc, argv};
    server.run();
    return server.get_exit_code();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
