#include <cassert>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <random>
#include <shared_mutex>
#include <thread>
#include <vector>

std::mutex g_mutex;
std::condition_variable g_cv;
bool g_ready{false};

void producer() {
  std::unique_lock<std::mutex> ul(g_mutex);
  g_ready = true;
  ul.unlock();
  g_cv.notify_one();
  std::cout << "producer completed" << std::endl;
}

void consumer() {
  std::unique_lock<std::mutex> ul(g_mutex);
  g_cv.wait(ul, [](){ return g_ready == true; });
  std::cout << "consumer completed" << std::endl;
}

int main() {
  std::thread t1(producer);
  std::thread t2(consumer);

  t1.join();
  t2.join();

  return 0;
}

