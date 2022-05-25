// hometask https://youtu.be/3aqxaZsvn80?t=1153

#include <iostream>
#include <vector>
#include <future>


// sum of numbers in the range [start, end)
// example: Functor functor(); functor (4, 9):
// functor() --> 4 + 5 + 6 + 7 + 8
class ReturnFunctor {
public:
  uint64_t operator()(uint64_t start, uint64_t end) {
    uint64_t sum {};
    for (auto i = start; i < end; ++i) {
      sum += i;
    }
    return sum;
  }
};

int main() {

  const int number_of_threads = 10;
  const int number_of_elements = 1000 * 1000 * 1000;
  const int step = number_of_elements / number_of_threads;
  std::vector<std::future<uint64_t>> tasks;

  std::vector<std::unique_ptr<ReturnFunctor>> functors;

  for (uint64_t i{}; i < number_of_threads; ++i) {
    functors.push_back(std::make_unique<ReturnFunctor>()); // push_back will call the unique_ptr move constructor when constructing the element to be appended to the vector.
    // std::async returns std::future
    tasks.push_back(std::async(*functors.back(), i * step, (i + 1) * step /* correct only when number_of_elements is even */));
  }

  uint64_t total {};

  for (auto &t : tasks) {
    total += t.get(); // t.get() as alternative to join()
                      // but with getting return value
  }

  std::cout << "total: " << total << std::endl;

  return 0;
}
