// https://youtu.be/3aqxaZsvn80?t=928

#include <numeric> // std::accumulate
#include <iostream>
#include <vector>
#include <future>

// Function for std::async which returns value
uint64_t GetRangeSum(uint64_t start, uint64_t end) {
  uint64_t sum = 0;
  for (uint64_t i = start; i < end; ++i) {
    sum += i;
  }
  return sum;
}

int main() {

  const int number_of_threads = 10;
  const int number_of_elements = 1000 * 1000 * 1000;
  const int step = number_of_elements / number_of_threads;
  std::vector<std::future<uint64_t>> tasks;

  for (uint64_t i{}; i < number_of_threads; ++i) {
    // std::async returns std::future
    tasks.push_back(std::async(GetRangeSum, i * step, (i + 1) * step /* correct only when number_of_elements is even */));
  }

  uint64_t total {};

  for (auto &t : tasks) {
    total += t.get(); // t.get() as alternative to join()
                      // but with getting return value
  }

  std::cout << "total: " << total << std::endl;

  return 0;
}
