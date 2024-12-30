#include <Core/Server.h>

namespace HappyFlood::Core {

Server::Server(const Config::ServerConfig &config) : m_config(config) {
  create_socket();
  create_address();
  int ipDataSize = static_cast<int>(m_config.getMsg().size() / 8) +
                   static_cast<bool>(m_config.getMsg().size() % 8);
  m_msgPerSecond = (m_config.getSpeed() * 1024) / ipDataSize;
}

Server::Server(int argc, char *argv[])
    : Server(Config::ServerConfig(argc, argv)) {}

void Server::run() {
  for (int n = 0; n < 5; ++n) {
    for (int i = 0; i < m_msgPerSecond; ++i) {
      send_msg();
    }
    sleep(1);
  }
}

void Server::create_socket() {
  char buf[INET6_ADDRSTRLEN];
  m_ipVersion = AF_INET;
  if (inet_pton(AF_INET6, m_config.getHost().c_str(), buf)) {
    m_ipVersion = AF_INET6;
  }
  m_socket = socket(m_ipVersion, m_config.getSocketType(), 0);
  if (m_socket < 0) {
    m_exitCode = EXIT_FAILURE;
    throw std::runtime_error("socket creation failed");
  }
}

void Server::create_address() {
  memset(&m_addr, 0, sizeof(m_addr));
  char buf[INET6_ADDRSTRLEN];
  if (inet_pton(AF_INET, m_config.getHost().c_str(), buf)) {
    m_addr.sin_family = AF_INET;
    inet_pton(AF_INET, m_config.getHost().c_str(), &(m_addr.sin_addr));
  } else if (inet_pton(AF_INET6, m_config.getHost().c_str(), buf)) {
    m_addr.sin_family = AF_INET6;
    inet_pton(AF_INET6, m_config.getHost().c_str(), &(m_addr.sin_addr));
  }
  m_addr.sin_port = htons(m_config.getPort());
}

void Server::send_msg() const {
  sendto(m_socket, m_config.getMsg().c_str(), strlen(m_config.getMsg().c_str()),
         MSG_NOSIGNAL, reinterpret_cast<const sockaddr *>(&m_addr),
         sizeof(m_addr));
}
} // namespace HappyFlood::Core