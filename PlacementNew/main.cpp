// These are examples of placement new operator
// Question is from https://youtu.be/a18qTcWn-II?t=1386

#include <format>
#include <iostream>
using namespace std;

void example1() {
    // buffer on stack
    std::byte buf[sizeof(int) * 2];           // reserve memory for 2 ints at stack!

    // placement new in buf
    int *pInt = new (buf) int(3);                 // placement-new 1st int at stack with value 3!
    int *qInt = new (buf + sizeof(int)) int(5);   // placement-new 2nd int at stack with value 5!

    int *pBuf = reinterpret_cast<int*>(buf + 0);                  // get manually pointer to 1st int
    int *qBuf = reinterpret_cast<int*>(buf + sizeof(int));        // get manually pointer to 2nd int

     cout << std::format(
         "=== Example 1:\n"
         "Buff Addr\t\tInt Addr\n"
         "{}\t\t{}\n"
         "{}\t\t{}\n"
         "-----------------------\n"
         "1st Int\t\t\t2nd Int\n"
         "{}\t\t\t{}\n",
         static_cast<void*>(pBuf), static_cast<void*>(pInt),
         static_cast<void*>(qBuf), static_cast<void*>(qInt),
         *pBuf, *qBuf
     );
}

void example2() {
    int X = 10;

    cout << std::format(
        "=== Example 2:\n"
        "Before placement new :\n"
        "X : {}\n"
        "&X : {}\n",
        X, static_cast<void*>(std::addressof(X))
    );

    // placement new changes the value of X to -9
    int *mem = new (std::addressof(X)) int(-9);                  // placement-new int at stack!

    cout << std::format(
        "After placement new :\n"
        "X : {}\n"
        "&X : {}\n"
        "mem : {}\n",
        X, static_cast<void*>(std::addressof(X)), static_cast<void*>(mem)
    );
}

int main()
{
    example1();
    example2();

    return 0;
}

