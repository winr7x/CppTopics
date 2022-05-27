// https://youtu.be/jwJ4Eh_2Umo?t=1047

#include <cassert>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <shared_mutex>
#include <thread>
#include <vector>

std::shared_mutex g_shared_mutex;

const int  threads_count{100};

void SharedLock() {
  std::shared_lock <std::shared_mutex> sl(g_shared_mutex);
  std::cout << "shared_lock ";
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void ExclusiveLock() {
  std::lock_guard <std::shared_mutex> sl(g_shared_mutex);
  std::cout << "lock_guard ";
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void ExclusiveLockWriter() {
  std::lock_guard <std::shared_mutex> sl(g_shared_mutex);
  std::cout << "lock_guard_writer ";
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

int main() {
  std::vector<std::thread> threads;

  std::cout << "########### ExclusiveLock\n" << std::endl;

  for (int i{}; i < 3; i++) {
    threads.push_back(std::thread(ExclusiveLock));
  }

  for (auto &t : threads) {
    t.join();
  }

  threads.clear();

  std::cout << std::endl << "\n########### SharedLock\n" << std::endl;

  for (int i{}; i < threads_count; i++) {
    threads.push_back(std::thread(SharedLock));
  }
  threads.push_back(std::thread(ExclusiveLockWriter)); // writer waits until all readers have completed their work
  //                                                      see https://stackoverflow.com/a/11837714

  for (auto &t : threads) {
    t.join();
  }

  return 0;
}

