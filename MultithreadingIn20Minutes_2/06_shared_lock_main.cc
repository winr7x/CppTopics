// https://youtu.be/jwJ4Eh_2Umo?t=1087

#include <cassert>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <shared_mutex>
#include <thread>
#include <vector>

std::shared_mutex g_shared_mutex;

void ExclusiveLock() {
  std::lock_guard <std::shared_mutex> sl(g_shared_mutex);
  std::cout << "lock_guard ";
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

/*
 * Threads are started simultaneously but due to lock_guarg()
 * critical section is executed sequentially thread by thread
 */
void demonstrateExclusiveLock() {
  std::vector<std::thread> threads;

  std::cout << "########### ExclusiveLock\n" << std::endl;

  for (int i{}; i < 3; i++) {
    threads.push_back(std::thread(ExclusiveLock));
  }

  for (auto &t : threads) {
    t.join();
  }
}


void SharedLock() {
  std::shared_lock <std::shared_mutex> sl(g_shared_mutex);
  std::cout << "shared_lock ";
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void ExclusiveLockWriter() {
  std::lock_guard <std::shared_mutex> sl(g_shared_mutex);
  std::cout << "lock_guard_writer ";
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

/*
 * Demonstrate shared lock (Writer and Readers pattern https://stackoverflow.com/a/11837714)
 *
 * Threads (readers) are started simultaneously and due to shared_lock()
 * critical section is executed simultaneously too, by all threads (readers) at the same time
 *
 * But last started thread (writer) will run only after all previous threads (readers) finished
 * due to lock_guard()
 */
void demonstrateSharedLock() {
  std::vector<std::thread> threads;

  std::cout << std::endl << "\n########### SharedLock\n" << std::endl;

  for (int i{}; i < 100; i++) {
    threads.push_back(std::thread(SharedLock));
  }
  threads.push_back(std::thread(ExclusiveLockWriter)); // writer waits until all readers have completed their work
  //                                                      see https://stackoverflow.com/a/11837714

  for (auto &t : threads) {
    t.join();
  }

}

int main() {

  demonstrateExclusiveLock(); // Just Mutual Exclusive Lock
  demonstrateSharedLock();    // Writer And Readers Pattern

  return 0;
}

// Output:
// & .\06_shared_lock_main.exe
// ########### ExclusiveLock
//
// lock_guard lock_guard lock_guard
//
// ########### SharedLock
//
// shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock \
// shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock \
// shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock \
// shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock \
// shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock \
// shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock \
// shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock shared_lock \
// shared_lock shared_lock
// lock_guard_writer
// PS C:\Users\leha\Documents\CppTopics\build>

