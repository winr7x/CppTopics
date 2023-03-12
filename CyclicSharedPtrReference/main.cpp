// This is example of shared_ptr cyclic dependency issue and solution with weak_ptr

#include <format>
#include <iostream>
#include <memory>

#define ENABLE_SOLUTION // <--------------- SWITCH THIS FLAG!

using std::cout;
using std::format;

class B;

class A {
public:
    #ifdef ENABLE_SOLUTION
    std::weak_ptr<B> b;
    #else
    std::shared_ptr<B> b;
    #endif     
    ~A() { cout << "~A\n"; }
};

class B {
public:
    std::shared_ptr<A> a;
    ~B() { cout << "~B\n"; }
};


int main() {
  {
    auto a = std::make_shared<A>(); // 1)
    auto b = std::make_shared<B>(); // 2)

    cout << format("class A obj has {} references before assignment\n", a.use_count());
    cout << format("class B obj has {} references before assignment\n", b.use_count());
    
    a->b = b; // 3)
    b->a = a; // 4)

    cout << format("class A obj has {} references after assignment\n", a.use_count());
    cout << format("class B obj has {} references after assignment\n", b.use_count());
  }
  
  cout <<
#ifdef ENABLE_SOLUTION
      "1) main.b is destroyed since it was declared last\n"
      "2) then B obj is destroyed because it has 0 shared_ptrs referencing to it (only 1 weak_ptr from A obj)\n"
      "3) B.a attribute is destroyed as part of B obj\n"
      "4) main.a is destroyed since it is declared first\n"
      "5) A obj is destroyed because it has 0 shared_ptrs referencing to it\n"
      "6) A.b is destroyed as part of A obj\n";
#else
      "A and B objs are still in memory because main.a and main.b are destroyed, "
      "but objects they reference to still referece each other through their A.b and B.a attributes. "
      "Memory leak\n";
#endif
}