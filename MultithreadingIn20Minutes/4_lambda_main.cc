// https://youtu.be/3aqxaZsvn80?t=725

#include <numeric> // std::accumulate
#include <iostream>
#include <vector>
#include <thread>

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
      // Here lambda function without parameters and return value!!!
      // In format [ <capture> ] { <body> }
      std::thread([i, &partial_sums, step] {
        for (uint64_t j = i * step; j < (i + 1) * step /* correct only when number_of_elements is even */; ++j) {
          partial_sums[i] += j;
        }
      })
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

  uint64_t total = 
    std::accumulate(partial_sums.begin(), partial_sums.end(), uint64_t(0));

  PrintVector(partial_sums);

  std::cout << "total: " << total << std::endl;

  return 0;
}
