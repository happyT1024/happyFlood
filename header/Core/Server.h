#pragma once

#include <Config/ServerConfig.h>
#include <Core/ServerStatistic.h>

namespace HappyFlood::Core {

class Server {
public:
  Server() = delete;
  Server(const Server &) = delete;
  Server &operator=(const Server &) = delete;
  explicit Server(const Config::ServerConfig &config);
  explicit Server(int argc, char *argv[]);

  void run();
  [[nodiscard]] int get_exit_code() const { return m_exitCode; }

  ~Server() = default;

private:
  const Config::ServerConfig m_config;
  int m_socket{-1};
  sockaddr_in m_addr{};
  int m_exitCode{EXIT_SUCCESS};
  int m_msgPerSecond{};
  int m_ipVersion{};
  ServerStatistic<5*60> m_sentKbStatistic;
  ServerStatistic<5*60> m_sentPackagesStatistic;

private:
  void create_socket();
  void create_address();
  void send_msg() const;
  void add_statistic(int sendPackages);
  void output_statistic(int seconds) const;
};

} // namespace HappyFlood::Core