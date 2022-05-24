// https://stackoverflow.com/a/15530592

/*

You should think of using std::ref when a function:

    * takes a template parameter by value

    * copies/moves a template parameter, such as std::bind or the constructor for std::thread.

std::ref creates a copyable value type that behaves like a reference.

*/

#include <functional>
#include <thread>
#include <iostream>

void increment(int &i) {
  ++i;
}

int main() {
  int i = 0;

  // Here, we bind increment to a COPY of i...
  std::bind( increment, i ) ();
  //                        ^^ (...and invoke the resulting function object)

  std::cout << "i is still 0, because the copy was incremented.\n";
  std::cout << "i == " << i << "\n";

  std::cout << '\n';

  std::cout << "Now, we bind increment to std::ref(i)\n";
  std::bind( increment, std::ref(i) ) ();
  std::cout << "i has now been incremented.\n";
  std::cout << "i == " << i << "\n";

  std::cout << '\n';

  std::cout << "The same applies for std::thread\n";
  std::thread( increment, std::ref(i) ).join();
  std::cout << "i == " << i << "\n";

  return 0;
}