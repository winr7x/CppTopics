// https://youtu.be/jwJ4Eh_2Umo?t=170

#include <cassert>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

unsigned long g_counter;

void Incrementer() {
  for (size_t i{}; i < 100; ++i) {
    g_counter++;
  }
}

int main() {
  int N {3000};
  std::map<int, int> count;

  for (int i{}; i < N; i++) { // 1000 times print out g_counter
    g_counter = 0;
    std::vector<std::thread> threads;

    for (int i{}; i < 100; i++) {
      threads.push_back(std::thread(Incrementer));
    }

    for (auto &t : threads) {
      t.join();
    }

    // g_count is expected 100000 here

    std::cout << g_counter
              << (g_counter == 10000 ? "," : "!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!#!,")
              << ' ';
    ++count[g_counter];

    // You will see huge sequence of 10000, 10000, 10000, 10000, ... in output
    // But you can find among them 9921 for example! 
    // In rare cases. maybe you will need launch exe file several times

    // Why so? Because g_counter++ is not atomic operation!
    // It includes Read -> Increment -> Write steps;

    // Even Read or Write steps are not atomic!
  }

  std::cout << std::endl;
  // Assert that we always got 100*100
  assert(count[100 * 100] == N);
  // When assert failed, count can be (for example):
  // count[10000] == N-2
  // count[9904] == 1
  // count[9927] == 1

  return 0;
}
