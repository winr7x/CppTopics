// prefer lambda instead of bind

// std::bind takes a function as input and returns a new function Object as an output

// A function object, or functor, is any type that implements operator()

#include <algorithm>
#include <iostream>
#include <functional>                                                                // you need this include for bind !

int add(int a, int b)
{
    return a + b;
}

bool divisible(int a , int b)
{
  return a % b == 0;
}

class Class {
public:
  void method() {
    std::cout << "Class::method()\n";
  }
};

int main(int argc, char *argv[]) {
  (void)argc; (void)argv;
  //============================================================================================================================================

  auto add_bind = std::bind(&add,
                            std::placeholders::_1,                                   // _1 is in std::placeholders:: !
                            std::placeholders::_2);

  add(2, 2);
  add_bind(2, 2);                                                                    // add_bind() is equivalent to add() !

  //============================================================================================================================================

   auto add_bind_10 = std::bind(&add,
                            10,                                                      // fix 1st arg as 10 !
                            std::placeholders::_1);

  add_bind_10(2);

  //============================================================================================================================================

  int arr[10] = {1, 5, 3, 10};
  std::count_if(arr,
                arr + sizeof(arr) / sizeof(int),
                std::bind(&divisible,                                                // bind creates UnaryPredicate for count_if from divisible !
                          std::placeholders::_1,
                          5));

  //============================================================================================================================================

  std::count_if(arr,
                arr + sizeof(arr) / sizeof(int),
                [] (const int &v) {                                                  // prefer lambda instead of bind. more readable !
                  return divisible(v, 5);
                });

  //============================================================================================================================================

  Class c;
  auto method_bind = std::bind(&Class::method, &c);                                  // bind creates free function from method of class !
                                            //  ^                                    // method of class has implicit 1st argument: pointer to object of class !
  method_bind();                                                                     // method_bind() is equivalent to c.method() !

  //============================================================================================================================================

  auto method_lambda = [&c]() {                                                      // prefer lambda !
    c.method();
  };
  method_lambda();

  //============================================================================================================================================

  auto bind_front = std::bind_front(add, 50);                                        // fix starting arg as 50 !
  /*C++23*/ //auto bind_back = std::bind_back(add, 1);                               // fix ending arg as 1 !

  //============================================================================================================================================

  /*(deprecated in C++11)(removed in C++17)*/ // std::bind1st(add, 50);
  /*(deprecated in C++11)(removed in C++17)*/ // std::bind2nd(add, 1);
  
  return EXIT_SUCCESS;
}