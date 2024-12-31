#pragma once

#include <array>

namespace HappyFlood::Core {

template <int SIZE> class ServerStatistic {
public:
  using integer_type = std::size_t;
  using data_type = std::array<integer_type, SIZE>;
  using data_iterator = typename data_type::iterator;
  using data_const_iterator = typename data_type::const_iterator;

public:
  ServerStatistic() = default;
  ServerStatistic(const ServerStatistic &) = default;
  ServerStatistic &operator=(const ServerStatistic &) = default;

  void add(integer_type value) {
    if (m_iterator == m_data.end()) {
      m_iterator = m_data.begin();
    }
    *m_iterator = value;
    ++m_iterator;
    if (m_sizeOfRealData < SIZE) {
      ++m_sizeOfRealData;
    }
  }

  [[nodiscard]] double getAverage(integer_type n = SIZE) const {
    double average{0};
    data_const_iterator iterator{m_iterator - 1};
    if (m_sizeOfRealData >= n) {
      for (int i = 0; i < n; ++i) {
        if (iterator < m_data.begin()) {
          iterator = m_data.end() - 1;
        }
        average += (*iterator) / static_cast<double>(n);
        --iterator;
      }
    } else {
      for (int i = 0; i < m_sizeOfRealData; ++i) {
        if (iterator < m_data.begin()) {
          iterator = m_data.end() - 1;
        }
        average += (*iterator) / static_cast<double>(m_sizeOfRealData);
        --iterator;
      }
    }
    return average;
  }

  ~ServerStatistic() = default;

private:
  data_type m_data = {};
  data_iterator m_iterator{m_data.begin()};
  integer_type m_sizeOfRealData{0};
};

} // namespace HappyFlood::Core