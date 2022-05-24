// https://youtu.be/3aqxaZsvn80?t=341

#include <numeric> // std::accumulate
#include <iostream>
#include <vector>
#include <thread>

// sum of numbers in the range [start, end)
// example: f(sum, 4, 9):
// sum = 4 + 5 + 6 + 7 + 8
void AccumulateRange(uint64_t &sum, uint64_t start, uint64_t end) {
  sum = 0;
  for (uint64_t i = start; i < end; i++) {
    sum += i;
  }
}

// example: '{ 7 123 1 3643 }'
void PrintVector(std::vector<uint64_t> &vector) {
  std::cout << "{ ";
  for (auto &e : vector) {
    std::cout << e << ' ';
  }
  std::cout << '}' << std::endl;
}

int main() {

  const int number_of_threads = 10;
  const int number_of_elements = 1000 * 1000 * 1000;
  const int step = number_of_elements / number_of_threads;
  std::vector<std::thread> threads;
  std::vector<uint64_t> partial_sums(number_of_threads);

  for (uint64_t i{}; i < number_of_threads; ++i) {
    threads.push_back(
      std::thread(
        AccumulateRange, std::ref(partial_sums[i]), i * step, (i + 1) * step
      )
    );
  }

  // A thread object is said to be joinable if it identifies/represent an active thread of execution
  // A thread IS joinable if:
  //   It's commands still are executing
  //   It's commands were completed but join() was not called
  // A thread is NOT joinable if:
  //   It was default-constructed: std::thread t1;
  //   If either of its member join or detach has been called: after t1.join()
  for (std::thread &t : threads) {
    // Here joinable() is not necessary.
    // Usually this check covers the case that someone already called join explicitly. If already called, second join() will cause exception.
    // Or If you don't know whether your thread was default constructed or not. If was, join() will cause exception.
    if (t.joinable()) { 
      t.join();
    }
  }

  uint64_t total = std::accumulate(partial_sums.begin(), partial_sums.end(), uint64_t(0) /* initial value of the sum */);

  PrintVector(partial_sums);

  std::cout << "total: " << total << std::endl;

  return 0;
}
