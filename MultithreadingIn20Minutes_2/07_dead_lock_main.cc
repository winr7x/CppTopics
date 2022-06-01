// https://youtu.be/jwJ4Eh_2Umo?t=1047

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

void Incrementer_Bad1() {
  for (size_t i{}; i < 100; ++i) {
    std::cout << "Incrementer_Bad1(): Here deadlock can happen g_mutex1!" << std::endl;
    std::lock_guard<std::mutex> lock1(g_mutex1);
    std::cout << "Incrementer_Bad1(): Here deadlock can happen g_mutex2!" << std::endl;
    std::lock_guard<std::mutex> lock2(g_mutex2);
    ++g_counter;
    std::cout << "Incrementer_Bad1() completed. g_counter == " << g_counter << std::endl;
  }
}

void Incrementer_Bad2() {
  for (size_t i{}; i < 100; ++i) {
    std::cout << "Incrementer_Bad2(): Here deadlock can happen g_mutex2!" << std::endl;
    std::lock_guard<std::mutex> lock2(g_mutex2);
    std::cout << "Incrementer_Bad2(): Here deadlock can happen g_mutex1!" << std::endl;
    std::lock_guard<std::mutex> lock1(g_mutex1);
    ++g_counter;
    std::cout << "Incrementer_Bad2() completed. g_counter == " << g_counter << std::endl;
  }
}

int main() {
  std::thread t1(Incrementer_Bad1);
  std::thread t2(Incrementer_Bad2);

  t1.join();
  t2.join();

  return 0;
}

