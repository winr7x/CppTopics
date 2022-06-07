// https://youtu.be/jwJ4Eh_2Umo?t=2046

// ESSENCE: homework: how to break the infinite loop in producer/consumer

// PREVIOUS: homework1: how to notify specific thread

// SOLUTION: exit from producer's loop and consumer[1,2]'s loop when shared data is special

// * when 2 seconds passed from starting of a program producer generates special value -777, notifies all consumers and exits
// * consumers get special value -777 and exit without notify back to producer

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

std::chrono::steady_clock::time_point g_begin;
std::mutex g_mutex;
std::condition_variable g_cv;
int g_data{};
const int terminateConstant = -777;

// some snippet from internet
int GenRandomValue(int start, int end) {
  std::random_device rd;
  std::mt19937::result_type seed = rd() ^ (
          (std::mt19937::result_type)
          std::chrono::duration_cast<std::chrono::seconds>(
              std::chrono::system_clock::now().time_since_epoch()
              ).count() +
          (std::mt19937::result_type)
          std::chrono::duration_cast<std::chrono::microseconds>(
              std::chrono::high_resolution_clock::now().time_since_epoch()
              ).count() );

  std::mt19937 gen(seed);
  std::uniform_int_distribution<unsigned> distrib(start, end);

  return distrib(gen);
}

bool aboutToTerminate() {
  return g_data == terminateConstant;
}

void producer() {
  bool isRunning{true};
  while(isRunning) {
    std::unique_lock<std::mutex> ul(g_mutex); // HERE
    // Produce data
    const auto secondsPassed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - g_begin).count();
    g_data = (secondsPassed >= 2) ? terminateConstant : GenRandomValue(1, 999);

    std::cout << "produced data: " << g_data << std::endl;
    ul.unlock();

    // notify consumer1 thread
    g_cv.notify_all();
    if (!aboutToTerminate()) {
      ul.lock();
      g_cv.wait(ul, [](){ return g_data == 0; });
    } else {
      isRunning = false;
    }
  }
  std::cout << "producer completed" << std::endl;
}

bool evenData() {
  return g_data % 2 == 0;
}

void consumer1() {
  bool isRunning{true};
  while (isRunning) {
    std::unique_lock<std::mutex> ul(g_mutex);
    g_cv.wait(ul, [](){ return ((g_data && evenData()) || aboutToTerminate()); }); // react on message from producer only if data is even or terminate value
    // Sample data
    std::cout << "consumer1 data: " << g_data << std::endl;
    if (!aboutToTerminate()) {
      g_data = 0;
      ul.unlock();
      g_cv.notify_all();                                      // if use notify_one(), producer can never receive notification
                                                              // because notification targets are consumer2 and producer and
                                                              // and one of them will be chosen randomly
    } else {
      isRunning = false;
    }
  }
  std::cout << "consumer1 completed" << std::endl;
}

bool oddData() {
  return g_data % 2 == 1;
}

void consumer2() {
  bool isRunning{true};
  while (isRunning) {
    std::unique_lock<std::mutex> ul(g_mutex);
    g_cv.wait(ul, [](){ return ((g_data && oddData()) || aboutToTerminate()); }); // react on message from producer only if data is odd or terminate value
    // Sample data
    std::cout << "consumer2 data: " << g_data << std::endl;
    if (!aboutToTerminate()) {
      g_data = 0;
      ul.unlock();
      g_cv.notify_all();                                      // if use notify_one(), producer can never receive notification
                                                              // because notification targets are consumer1 and producer and
                                                              // and one of them will be chosen randomly
    } else {
      isRunning = false;
    }
  }
  std::cout << "consumer2 completed" << std::endl;
}



int main() {
  g_begin = std::chrono::steady_clock::now();

  std::thread t1(producer);
  std::thread t2(consumer1);
  std::thread t3(consumer2);

  t1.join();
  t2.join();
  t3.join();

  return 0;
}

