// hometask https://youtu.be/3aqxaZsvn80?t=1153

#include <iostream>
#include <vector>
#include <future>

int main() {

  const int number_of_threads = 10;
  const int number_of_elements = 1000 * 1000 * 1000;
  const int step = number_of_elements / number_of_threads;
  std::vector<std::future<uint64_t>> tasks;

  for (uint64_t i{}; i < number_of_threads; ++i) {
    // std::async returns std::future
    tasks.push_back(
      std::async(
        [i, step] {
          uint64_t sum{};
          for (uint64_t j = i * step; j < (i + 1) * step /* correct only when number_of_elements is even */; ++j) {
            sum += j;
          }
          return sum;
        }
      )
    );
  }

  uint64_t total {};

  for (auto &t : tasks) {
    total += t.get(); // t.get() as alternative to join()
                      // but with getting return value
  }

  std::cout << "total: " << total << std::endl;

  return 0;
}
