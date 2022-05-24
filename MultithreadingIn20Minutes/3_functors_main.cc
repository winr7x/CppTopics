// https://youtu.be/3aqxaZsvn80?t=341

#include <numeric> // std::accumulate
#include <iostream>
#include <vector>
#include <thread>

// sum of numbers in the range [start, end)
// example: Functor functor(); functor (4, 9):
// _sum = 4 + 5 + 6 + 7 + 8
class AccumulateFunctor {
public:
  void operator()(uint64_t start, uint64_t end) {
    _sum = 0;
    for (auto i = start; i < end; ++i) {
      _sum += i;
    }
  }

  uint64_t _sum;
};

// example: '{ 7 123 1 3643 }'
void PrintFunctorSums(const std::vector<std::unique_ptr<AccumulateFunctor>> &functors) {
  std::cout << "{ ";
  for (const auto &f : functors) {
    std::cout << f->_sum << ' ';
  }
  std::cout << '}' << std::endl;
}

int main() {

  const int number_of_threads = 10;
  const int number_of_elements = 1000 * 1000 * 1000;
  const int step = number_of_elements / number_of_threads;
  std::vector<std::thread> threads;

  std::vector<std::unique_ptr<AccumulateFunctor>> functors;

  for (uint64_t i{}; i < number_of_threads; ++i) {
    functors.push_back(std::make_unique<AccumulateFunctor>()); // push_back will call the unique_ptr move constructor when constructing the element to be appended to the vector.
    threads.push_back(
      std::thread(
        std::ref(*functors.back()), i * step, (i + 1) * step
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

  uint64_t total = 0;
  for (auto &f : functors) {
    total += f->_sum;
  }

  PrintFunctorSums(functors);

  std::cout << "total: " << total << std::endl;

  return 0;
}
