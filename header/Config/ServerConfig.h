#pragma once

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

namespace HappyFlood::Config {

class ServerConfig {
public:
  ServerConfig() = default;
  ServerConfig(const ServerConfig &) = default;
  ServerConfig &operator=(const ServerConfig &other) = default;
  ServerConfig(int argc, char *argv[]);

  [[nodiscard]] int getPort() const;
  [[nodiscard]] const std::string &getHost() const;
  [[nodiscard]] __socket_type getSocketType() const;
  [[nodiscard]] int getSpeed() const;
  [[nodiscard]] std::string getMsg() const;

  ~ServerConfig() = default;

private:
  int m_dstPort{8080};
  std::string m_dstHost{"192.168.0.12"};
  std::string m_msg{"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"};
  int m_speed{100}; // Kb/s
  __socket_type m_socketType{SOCK_DGRAM};
};

}