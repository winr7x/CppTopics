// https://youtu.be/jwJ4Eh_2Umo?t=2031

// ESSENCE: homework: how to notify only specific thread

// PREVIOUS: notify another thread. good example with conditional var
// NEXT: homework2

// SOLUTION: use notify_all (both in producer and in consumers). Specific consumer reacts only if see that data is for him.

// * technically notify specific thread is impossible
//     thread_one() notifies only one of threads, waiting with wait(), chosen randomly
//     thread_all() notifies all threads at once, waiting with wait()
// * but effect of notify specific thread is achievable through filtering in predicate only data designated for specific thread


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
std::mutex g_mutex2;
std::condition_variable g_cv;
int g_data{};
int g_ready{};

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

void producer() {
  while(true) {
    std::unique_lock<std::mutex> ul(g_mutex); // HERE
    // Produce data
    g_data = GenRandomValue(1, 999);

    std::cout << "produced data: " << g_data << std::endl;
    g_ready = true;
    ul.unlock();

    // notify consumer1 thread
    g_cv.notify_all();
    ul.lock();
    while (!(g_ready == 0)) {
      // std::cout << "producer wait" << std::endl;
      g_cv.wait(ul);
    }
  }
  std::cout << "producer completed" << std::endl;
}

bool evenData() {
  return g_data % 2 == 0;
}

bool oddData() {
  return g_data % 2 == 1;
}

void consumer1() {
  int data{};
  while (true) {
    std::unique_lock<std::mutex> ul(g_mutex);
    g_cv.wait(ul, [](){ return (g_ready && evenData()); }); // react on message from producer only if data is even
    // Sample data
    data = g_data;
    std::cout << "consumer1 data: " << data << std::endl;
    g_ready = 0;
    ul.unlock();
    g_cv.notify_all();                                      // if use notify_one(), producer can never receive notification
                                                            // because notification targets are consumer2 and producer and
                                                            // and one of them will be chosen randomly
  }
  std::cout << "consumer1 completed" << std::endl;
}

void consumer2() {
  int data{};
  while (true) {
    std::unique_lock<std::mutex> ul(g_mutex);
    g_cv.wait(ul, [](){ return (g_ready && oddData()); }); // react on message from producer only if data is odd
    // Sample data
    data = g_data;
    std::cout << "consumer2 data: " << data << std::endl;
    g_ready = 0;
    ul.unlock();
    g_cv.notify_all();                                      // if use notify_one(), producer can never receive notification
                                                            // because notification targets are consumer1 and producer and
                                                            // and one of them will be chosen randomly
  }
  std::cout << "consumer2 completed" << std::endl;
}



int main() {

  std::thread t1(producer);
  std::thread t2(consumer1);
  std::thread t3(consumer2);

  t1.join();
  t2.join();
  t3.join();

  return 0;
}

