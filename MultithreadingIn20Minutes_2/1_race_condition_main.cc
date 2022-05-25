// https://youtu.be/jwJ4Eh_2Umo?t=138

#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

unsigned long g_x;

int main() {
  for (int i{}; i < 3000; ++i) {
    auto t1 = std::thread([]() { g_x = 1; });
    auto t2 = std::thread([]() { g_x = 2; });
    t1.join();
    t2.join();
    std::cout << g_x << ", ";
  }

  std::cout << std::endl;

  // You will see huge sequence of 2, 2, 2, 2, ... in output
  // But you can find among them 1 too! in rare cases

  return 0;
}
