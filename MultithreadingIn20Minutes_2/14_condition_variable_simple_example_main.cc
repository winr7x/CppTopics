// ESSENCE: very simple example of conditional variable

// * producer thread notifies consumer thread (without confirmation of receipt of the notification)
// * if consumer thread will run much later, then its predicate will return true and consumer will be compileted without any waiting of notify_one()
// * if consumer thread will run much earlier, then its predicate will return false and consumer will wait for notify_one() and will be completed only after that

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

