#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

class Config {
public:
  Config() = default;
  int PORT = 8080;
  std::string HOST = "192.168.0.12";
  int N = 3; // number of sockets
  int M = 1; // number of sends
  std::vector<const char *> messages{"XXX", "INVITE"};
};

Config handle_params(int argc, char *argv[]) {
  Config config;
  if (argc > 1)
    config.HOST = argv[1];
  if (argc > 2)
    config.PORT = std::stoi(argv[2]);
  if (argc > 3)
    config.N = std::stoi(argv[3]);
  if (argc > 4)
    config.M = std::stoi(argv[4]);
  if (argc > 5) {
    config.messages.clear();
    for (int i = 5; i < argc; i++) {
      config.messages.push_back(argv[i]);
    }
  }
  return config;
}

void create_sockets(std::vector<int> &socks, const Config &config) {
  socks.resize(config.N);
  for (auto &s : socks) {
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
    }
  }
}

sockaddr_in create_address(const Config &config) {
  sockaddr_in servaddr{};
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(config.PORT);
  inet_pton(AF_INET, config.HOST.c_str(), &(servaddr.sin_addr));
  return servaddr;
}

void flood(int sock, const Config &config, const sockaddr_in &targetAddr) {
  for (auto &message : config.messages) {
    sendto(sock, message, strlen(message), MSG_NOSIGNAL,
           reinterpret_cast<const sockaddr *>(&targetAddr), sizeof(targetAddr));
  }
}

int main(int argc, char *argv[]) {
  auto config = handle_params(argc, argv);
  std::vector<int> socks{};
  create_sockets(socks, config);
  sockaddr_in targetAddr = create_address(config);
  for (int i = 0; i < config.M; i++) {
    std::cout << "i: " << i << ":\n";
    for (auto &s : socks) {
      flood(s, config, targetAddr);
      std::cout << "\t" << "s: " << s << "\n";
    }
    std::cout << "\n";
  }
  return 0;
}
