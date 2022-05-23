// https://youtu.be/3aqxaZsvn80?t=44

#include <numeric>
#include <iostream>
#include <vector>
#include <thread>

void AccumulateRange(uint64_t &sum, uint64_t start, uint64_t end) {
  sum = 0;
  for (uint64_t i = start; i < end; i++) {
    sum += i;
  }
}

void PrintVector(std::vector<uint64_t> &vector) {
  std::cout << "{ ";
  for (auto &e : vector) {
    std::cout << e << ' ';
  }
  std::cout << '}' << std::endl;
}

int main() {

  const int number_of_threads = 2;
  const int number_of_elements = 1000 * 1000 * 1000;
  const int step = number_of_elements / number_of_threads;
  std::vector<uint64_t> partial_sums(number_of_threads);

  std::thread t1(AccumulateRange, std::ref(partial_sums[0]), 0, step);
  std::thread t2(AccumulateRange, std::ref(partial_sums[1]), step, number_of_threads * step);

  t1.join();
  t2.join();

  uint64_t total = std::accumulate(partial_sums.begin(), partial_sums.end(), uint64_t(0));

  PrintVector(partial_sums);

  std::cout << "total: " << total << std::endl;

  return 0;
}