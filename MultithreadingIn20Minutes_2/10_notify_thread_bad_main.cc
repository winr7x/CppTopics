// https://youtu.be/jwJ4Eh_2Umo?t=1488

// cmake .. ; cmake --build . --config Release --clean-first --target 10_notify_thread_bad_main; .\10_notify_thread_bad_main

// ESSENCE: 1) notify (send message to) another thread
//          2) bad example of producer-consumer pattern

// NEXT: the opposite - good example of producer-consumer pattern with using conditional variable

// SOLUTION: another thread monitors flag (g_ready) and read message (g_data)

// * why bad? because g_ready is monitored through sleep loop with the need to adjust sleep time (see ZZ3)
// * producer is the lead loop that will iterate from start to finish
// * producer will produce all planned data
// * consumer is a seсondary loop
// * consumer will iterate as many times as it can wedge between locks of producer
// * there is no guarantee that consumer will read all producer's data when producer.millisecondsand & consumer.milliseconds are equal
// * for example 100 values produced and 96 values consumed when producer.milliseconds(30) and consumer.milliseconds(30)
// * yes, it may happen that 100 values produced and 100 values consumed when producer.milliseconds(30) and consumer.milliseconds(30)
// * so, values consumed <= values produced
// * you can organize code to guarantee that consumer will read all producer's data (see ZZ1)
// * yes, chance that consumer will read all producer's data when producer.millisecondsand(100) & consumer.milliseconds(1) is extremely large
// * consumer consumes last produced g_data
// * produced values that the consumer did not manage to catch are lost forever
// * so, when you add super long delay before thread2 creation, only one last produced data will be caught (see ZZ2)

#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <random>
#include <shared_mutex>
#include <thread>
#include <vector>

std::mutex g_mutex;
int g_data{};
bool g_ready{};
bool g_producer_completed{};

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

const size_t iterations = 100;

void producer() {
  for (size_t i{}; i < iterations; ++i) {
    std::unique_lock<std::mutex> ul(g_mutex);

    // Produce data
    g_data = GenRandomValue(1, 999);
    std::cout << "producer[" << i << "] : g_data = " << g_data << std::endl;

    // Announce that data is produced
    g_ready = true;
    g_producer_completed = (i == (iterations - 1));


    ul.unlock(); // let consumer read data from here
    std::this_thread::sleep_for(std::chrono::milliseconds(30)); // ZZ3
    // ul.lock(); // unique_lock::unlock() function IS NOT called on destruction of ul if unlock() was called manually before
                  // so unlock() signal will NOT be received on given mutex twice
                  // so undefined behavior will NOT happen
                  // so you don't need call ul.lock() here

#if 0 // ZZ1 uncomment section to guarantee that consumer will read all producer's data
    for (ul.lock(); g_ready; ) {
      ul.unlock();                                                 // wait for customer
      std::this_thread::sleep_for(std::chrono::milliseconds(1));   // wait for customer
      ul.lock();
    }
#endif
  }

  std::cout << "Producer completed" << std::endl;
}


void consumer() {
  for (size_t i{}; i < iterations; ++i) {
    std::unique_lock<std::mutex> ul(g_mutex);                      // 1) lock and check g_ready
    // Busy waiting
    while(!g_ready) {
      ul.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(30));  // 2) unlock and sleep  // ZZ3
      ul.lock();                                                   // 3) lock and check g_ready
    }
                                                                   // 4) handle g_data being locked
    std::cout << "consumer[" << i << "] : g_data = " << g_data << std::endl;
    g_ready = false;
    if (g_producer_completed) {
      break;
    }
  }                                                                // 5) unlock
  std::cout << "Consumer completed" << std::endl;
}

int main() {

  std::thread t1(producer);
  // std::this_thread::sleep_for(std::chrono::milliseconds(20000)); // ZZ2
  std::thread t2(consumer);

  t1.join();
  t2.join();

  return 0;
}

