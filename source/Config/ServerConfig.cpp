#include <Config/ServerConfig.h>

namespace HappyFlood::Config {

ServerConfig::ServerConfig(int argc, char *argv[]) {
  if (argc > 1)
    m_dstHost = argv[1];
  if (argc > 2)
    m_dstPort = std::stoi(argv[2]);
}

[[nodiscard]] int ServerConfig::getPort() const { return m_dstPort; }

[[nodiscard]] const std::string &ServerConfig::getHost() const { return m_dstHost; }

[[nodiscard]] __socket_type ServerConfig::getSocketType() const {
  return m_socketType;
}

[[nodiscard]] int ServerConfig::getSpeed() const { return m_speed; }

[[nodiscard]] std::string ServerConfig::getMsg() const { return m_msg; }

[[nodiscard]] bool ServerConfig::getWriteStatistics() const { return m_writeStatistics; }

} // namespace HappyFlood::Config