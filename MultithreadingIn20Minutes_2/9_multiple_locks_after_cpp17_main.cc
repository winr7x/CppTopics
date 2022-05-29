// https://youtu.be/jwJ4Eh_2Umo?t=1294

#include <cassert>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <shared_mutex>
#include <thread>
#include <vector>

std::mutex g_mutex1, g_mutex2;
unsigned long g_counter;

const size_t iterations = 10000;

void Incrementer_Good1() {
  for (size_t i{}; i < iterations; ++i) {
    std::scoped_lock scoped_lock_name(g_mutex1, g_mutex2);
    ++g_counter;
    std::cout << "Good 1 " << std::endl;
  }
}

void Incrementer_Good2() {
  for (size_t i{}; i < iterations; ++i) {
    std::scoped_lock scoped_lock_name(g_mutex2, g_mutex1);
    ++g_counter;
    std::cout << "Good 2 " << std::endl;
  }
}

int main() {
  std::thread t1(Incrementer_Good1);
  std::thread t2(Incrementer_Good2);

  t1.join();
  t2.join();

  std::cout << "Completed: g_counter : " << g_counter << std::endl;

  return 0;
}

