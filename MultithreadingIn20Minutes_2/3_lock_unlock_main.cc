// https://youtu.be/jwJ4Eh_2Umo?t=610 k

#include <cassert>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

std::mutex g_mutex;
unsigned long g_counter;

const int  increment_times{100};

void Incrementer() {
  for (size_t i{}; i < increment_times; ++i) {
    g_mutex.lock();
    g_counter++;
    g_mutex.unlock();
  }
}

int main() {
  int N {2000};
  const int  threads_count{100};
  std::map<int, int> count;

  for (int i{}; i < N; i++) { // N times print out g_counter
    g_counter = 0;
    std::vector<std::thread> threads;

    for (int i{}; i < threads_count; i++) {
      threads.push_back(std::thread(Incrementer));
    }

    for (auto &t : threads) {
      t.join();
    }

    // g_count is expected increment_times * threads_count here

    std::cout << g_counter << ", ";
              // << (g_counter == 10000 ? "," : "!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!,")
              // << ' ';
    ++count[g_counter];
  }

  std::cout << std::endl;
  // Assert that we always got 100*100
  assert(count[increment_times * threads_count] == N);

  return 0;
}
