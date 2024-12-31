#include <Core/Server.h>

namespace HappyFlood::Core {

Server::Server(const Config::ServerConfig &config) : m_config(config) {
  create_socket();
  create_address();
  int ipDataSize = static_cast<int>(m_config.getMsg().size() / 8) +
                   static_cast<bool>(m_config.getMsg().size() % 8);
  m_msgPerSecond = (m_config.getSpeed() * 1024) / static_cast<int>(m_config.getMsg().size());
}

Server::Server(int argc, char *argv[])
    : Server(Config::ServerConfig(argc, argv)) {}

void Server::run() {
  using namespace std::chrono_literals;
  for (int n = 0; n < 100; ++n) {
    auto start = std::chrono::high_resolution_clock::now();
    int i = 0;
    for (; i < m_msgPerSecond; ++i) {
      send_msg();
      if ((i % 1000) == 0) {
        if (std::chrono::high_resolution_clock::now() - start > 1000ms) {
          break;
        }
      }
    }
    add_statistic(i);
    output_statistic(n+1);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    if (elapsed < 1000ms) {
      std::this_thread::sleep_for(1000ms - elapsed);
    }
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

void Server::add_statistic(int sendPackages) {
  if (!m_config.getWriteStatistics())
    return;
  m_sentKbStatistic.add(sendPackages * m_config.getMsg().size() / 1024);
  m_sentPackagesStatistic.add(sendPackages);
}

void Server::output_statistic(int seconds) const {
  if (!m_config.getWriteStatistics())
    return;
  system("clear");
  std::cout << "██╗  ██╗ █████╗ ██████╗ ██████╗ ██╗   ██╗███████╗██╗      ██████╗  ██████╗ ██████╗ \n"
               "██║  ██║██╔══██╗██╔══██╗██╔══██╗╚██╗ ██╔╝██╔════╝██║     ██╔═══██╗██╔═══██╗██╔══██╗\n"
               "███████║███████║██████╔╝██████╔╝ ╚████╔╝ █████╗  ██║     ██║   ██║██║   ██║██║  ██║\n"
               "██╔══██║██╔══██║██╔═══╝ ██╔═══╝   ╚██╔╝  ██╔══╝  ██║     ██║   ██║██║   ██║██║  ██║\n"
               "██║  ██║██║  ██║██║     ██║        ██║   ██║     ███████╗╚██████╔╝╚██████╔╝██████╔╝\n"
               "╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝        ╚═╝   ╚═╝     ╚══════╝ ╚═════╝  ╚═════╝ ╚═════╝ \n"
               << std::endl;;

  std::cout<< "Target hostport: " << m_config.getHost() << ":" << m_config.getPort() << std::endl;
  std::cout<< "Speed limit: " << m_config.getSpeed() << "Kb\\s" << std::endl;
  std::cout<< "Message: " << m_config.getMsg() << std::endl;

  std::cout<<"Uptime: " << seconds << " seconds" << std::endl;
  std::cout << "Average sent by 1 second: " << m_sentKbStatistic.getAverage(1)
            << " Kb\\s " << m_sentPackagesStatistic.getAverage(1) << " packages"
            << std::endl;
  std::cout << "Average sent by 5 second: " << m_sentKbStatistic.getAverage(5)
            << " Kb\\s " << m_sentPackagesStatistic.getAverage(5) << " packages"
            << std::endl;
  std::cout << "Average sent by 30 second: " << m_sentKbStatistic.getAverage(30)
            << " Kb\\s " << m_sentPackagesStatistic.getAverage(30) << " packages"
            << std::endl;
  std::cout << "Average sent: " << m_sentKbStatistic.getAverage()
            << " Kb\\s " << m_sentPackagesStatistic.getAverage() << " packages"
            << std::endl;
}

} // namespace HappyFlood::Core