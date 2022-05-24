// https://youtu.be/3aqxaZsvn80?t=44

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

  const int number_of_threads = 2;
  const int number_of_elements = 1000 * 1000 * 1000;
  const int step = number_of_elements / number_of_threads;
  std::vector<uint64_t> partial_sums(number_of_threads);

  std::thread t1(AccumulateRange, std::ref(partial_sums[0]), 0, step);
  std::thread t2(AccumulateRange, std::ref(partial_sums[1]), step, number_of_threads * step /* correct only when number_of_elements is even */);

  t1.join();
  t2.join();

  uint64_t total = std::accumulate(partial_sums.begin(), partial_sums.end(), uint64_t(0) /* initial value of the sum */);

  PrintVector(partial_sums);

  std::cout << "total: " << total << std::endl;

  return 0;
}

// 1: expected total: 0                 real total: 0
// 2: expected total: 1  0+1            real total: 1
// 3: expected total: 3  0+1+2          real total: 1      odd number
// 4: expected total: 6  0+1+2+3        real total: 6
// 5: expected total: 10 0+1+2+3+4      real total: 6      odd number
// 6: expected total: 15 0+1+2+3+4+5    real total: 15
// 7: expected total: 21 0+1+2+3+4+5+6  real total: 15     odd number

// number_of_elements = 6
// step = 3
// std::thread t1(AccumulateRange, std::ref(partial_sums[0]), 0, 3);
// std::thread t1(AccumulateRange, std::ref(partial_sums[0]), 3, 6);
// total: 15

// number_of_elements = 3
// step = 1
// std::thread t1(AccumulateRange, std::ref(partial_sums[0]), 0, 1);
// std::thread t1(AccumulateRange, std::ref(partial_sums[0]), 1, 2 /* must be 3 */);
// total: 1 /* must be 3 */

// number_of_elements = 7
// step = 3
// std::thread t1(AccumulateRange, std::ref(partial_sums[0]), 0, 3);
// std::thread t1(AccumulateRange, std::ref(partial_sums[0]), 3, 6 /* must be 7 */);
// total: 15 /* must be 7 */