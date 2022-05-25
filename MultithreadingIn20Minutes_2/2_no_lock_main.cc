// https://youtu.be/jwJ4Eh_2Umo?t=170

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
  int N {1000};
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

    // You will see huge sequence of 10000, 10000, 10000, 10000, ... in output
    // But you can find among them 9921 for example! in rare cases

    // Why so? Because g_counter++ is not atomic operation!
    // It includes Read -> Increment -> Write steps;

    // Even Read or Write steps are not atomic!
  }

  std::cout << std::endl;

  return 0;
}
