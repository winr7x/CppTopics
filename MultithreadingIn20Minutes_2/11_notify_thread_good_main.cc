// https://youtu.be/jwJ4Eh_2Umo?t=1666

// cmake .. ; cmake --build . --config Release --clean-first --target 11_notify_thread_good_main; .\11_notify_thread_good_main

// ESSENCE: 1) notify (send message to) another thread
//          2) good example of producer-consumer pattern

// PREVIOUS: the opposite - bad example of producer-consumer pattern with using shared memory and sleep() loop

// SOLUTION: another thread waits by means of wait() for notification and flag (g_ready) and read message (g_data)

// * why good? because another thread gets notification without sleep() loop

// * producer will produce data infinitely
// * consumer will consume data infinitely
// * producer thread and consumer thread wait each other
// * so producer will continue only after consumer has read data (has set g_ready to false)
// * so consumer will continue only after producer has produced data (has set g_ready to true)
// * so you can start consumer long time after producer and program will still work correctly (see ZZ1)
//     producer will continue after consumer's thread will run and g_ready will become false in that thread
// * so you can start producer long time after consumer and program will still work correctly (see ZZ2)
//     consumer will continue after producer's thread will run and g_ready will become true in that thread
// * actually in this example producer notifies consumer and consumer notifies producer back
// * actually g_cv.wait(ul, predicate()) waits notify only if predicate() returned false. Otherwise it just continues without any waiting (see ZZ3)
//     so you see word 'wait' but actually no any waiting!! 'wait_if' more correct

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

void ConsumeData(int data) {

}

void producer() {
  while(true) {
    std::unique_lock<std::mutex> ul(g_mutex);
    // Produce data
    g_data = GenRandomValue(1, 999);
    std::cout << "produced data: " << g_data << std::endl;
    g_ready = true;
    ul.unlock();
    g_cv.notify_one();                        // you don't need to hold g_mutex locked to call notify_one()
    ul.lock();                                // !! you must lock mutex before calling wait(). This is a requirement from documenation
                                              // !! if you forget to lock, then undefined behavior
    g_cv.wait(ul, [](){ return g_ready == false; });  // !! wait for 1) notification and 2) g_ready == false
  }
}

void consumer() {
  int data{};
  while (true) {
    std::unique_lock<std::mutex> ul(g_mutex);
    g_cv.wait(ul, [](){ return g_ready; });   // !! wait until g_ready becomes true. If already true - DON'T WAIT AT ALL HERE!
                                              // actually wait notification only if Predicat is false. Otherwise don't wait at all. Because equevalent construction is: ZZ3
                                              // while (!Predicate()) {
                                              //   wait(lock);
                                              // }
                                              // while wait() is waiting, g_mutex is unlocked in this thread (to allow another thread to produce data)
                                              // yes, here g_mutex, locked in previous line, becomes unlocked
                                              // just after getting notification, in code below wait(), g_mutex becomes locked in this thread (to allow you to read data)
                                              // or, if g_ready already was true, g_mutex stays locked in this thread (because was locked in ul declaration)
    // Sample data
    data = g_data;
    std::cout << "consumed data: " << data << std::endl;
    g_ready = false;
    ul.unlock();
    g_cv.notify_one();                        // you don't need to hold g_mutex locked to call notify_one()
    ConsumeData(data);
    // ul.lock(); // unique_lock::unlock() function IS NOT called on destruction of ul if unlock() was called manually before
                  // so unlock() signal will NOT be received on given mutex twice
                  // so undefined behavior will NOT happen
                  // so you don't need ul.lock() unlocked mutex here
  }
}

int main() {

  std::thread t1(producer);
  std::thread t2(consumer);

  // std::thread t1(producer);
  // std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // ZZ1
  // std::thread t2(consumer);

  // std::thread t2(consumer);
  // std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // ZZ2
  // std::thread t1(producer);


  t1.join();
  t2.join();

  return 0;
}

