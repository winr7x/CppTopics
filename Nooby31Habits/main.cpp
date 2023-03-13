// https://www.youtube.com/watch?v=i_wDa2AS_8w

#include <array>   // for std::array
#include <bit>     // for std::bit_cast
#include <cstdlib> // for EXIT_SUCCESS
#include <cstddef> // for std::to_integer
#include <format>  // for std::format
#include <iomanip> // for std::setw()
#include <iostream> // for std::cout
#include <vector>
#include <unordered_map>


// ### 5 ### BAD
void f([[maybe_unused]] int *arr,
       [[maybe_unused]] int n)                                // passing n (size) is BAD !
{
  // whatever
}

// ### 5 ### GOOD
template<std::size_t size>
void f_better([[maybe_unused]] std::array<int, size> &arr)    // size is deduced. GOOD !
{
  // whatever
}

// ### 5 ###
void using_c_array_BAD() {
  const int n = 256;
  int arr[n] = {0};
  f(arr, n);
}

// ### 5 ###
void using_c_array_GOOD() {
  const int n = 256;
  std::array<int, n> arr {};                                  // use std::array<> GOOD !
  f_better(arr);
}


// ### 6 ### BAD
void any_use_of_reinterpret_cast_cpp()
{
  long long x = 0;
  auto xp = reinterpret_cast<char *>(x);                      // undefined behaviour. BAD !
  [[maybe_unused]] auto x2 = reinterpret_cast<long long>(xp);                  // reinterpreting back to the original type. GOOD !
}

// ### 6 ### BAD
void any_use_of_reinterpret_cast_c()
{
  std::cout << "### 6 ### any_use_of_reinterpret_cast_c(): \n";
  float y = .123f;
  std::cout << "y == " << y << '\n';
  long i = *(long *) &y;                                      // undefined behaviour. BAD !
  // ...
  std::cout << "i == " << y << '\n';
  y = *(float *) &i;                                          // reinterpreting back to the original type. GOOD !
  std::cout << "y == " << y << '\n';
}

// ### 6 ### GOOD
template <typename T>
void print_bytes(const T &input)
{
  auto *bytes = reinterpret_cast<const std::byte *>(&input);  // * after auto to deduce pointer !
                                                              // std::byte type is useful for bitwise operations and it doesn't have overloaded << operator for cout !

  std::cout << "### 6 ### print_bytes(): "
            << "0x"
            << std::hex << std::setw(2) << std::setfill('0')
                                                              // current system is Little-endian: least significant byte is stored first !
            << std::to_integer<short>(*(bytes + 2))           // but for default people Big-endian notation is more natural, so print out most significant byte first !
            << std::to_integer<short>(*(bytes + 1))
            << std::to_integer<short>(*bytes)                 // here least significant byte is stored !
            << '\n';

  std::cout << std::format("### 6 ### print_bytes() [std::format()]: 0x{:02x}{:02x}{:02x}\n", // The 0 option pads the field with leading zeros
    std::to_integer<short>(bytes[2]),
    std::to_integer<short>(bytes[1]),
    std::to_integer<short>(bytes[0])
  );
}

// ### 6 ### BETTER
template <typename T>
void print_bytes_better(const T &input)
{
  using Bytearray = std::array<std::byte, sizeof(T)>;
  const auto &bytes = std::bit_cast<Bytearray, T>(input);

  // Before C++20 the correct way to reinterpret a value was a call to std::memcpy, copying the exact binary representation from a variable of one type into a variable of another.
  // Although canonical, the use of std::memcpy might still be confusing, it is verbose, and it might introduce performance overhead if the compiler does not recognize the idiom
  // and does not remove the function call.

  std::cout << "### 6 ### print_bytes_better(): "
            << "0x"
            << std::hex << std::setw(2) << std::setfill('0')
                                                              // current system is Little-endian: least significant byte is stored first !
            << std::to_integer<short>(bytes[2])               // but for default people Big-endian notation is more natural, so print out most significant byte first !
            << std::to_integer<short>(bytes[1])
            << std::to_integer<short>(bytes[0])               // here least significant byte is stored !
            << '\n';

  std::cout << std::format("### 6 ### print_bytes_better() [std::format()]: 0x{:02x}{:02x}{:02x}\n", // The 0 option pads the field with leading zeros
    std::to_integer<short>(bytes[2]),
    std::to_integer<short>(bytes[1]),
    std::to_integer<short>(bytes[0])
  );
}

// ### 7 ### BAD
const std::string & more_frequent_BAD(const std::unordered_map<std::string, int> &word_counts,
                                  const std::string &word1,
                                  const std::string &word2) 
{
  auto &counts = const_cast<std::unordered_map<std::string, int> &>(word_counts); // use at() instead of const_cast<>() !
  return counts[word1] > counts[word2] ? word1 : word2;
}

// ### 7 ### GOOD
const std::string & more_frequent_GOOD(const std::unordered_map<std::string, int> &word_counts,
                                  const std::string &word1,
                                  const std::string &word2) 
{
    return word_counts.at(word1) > word_counts.at(word2) ? word1 : word2;
}

// ### 9 ### BAD
void print_vec_one_per_line_BAD(std::vector<int> &arr) {
  std::cout << "### 6 ### print_vec_one_per_line_BAD():\n";
  for (const auto &x: arr) {
    std::cout << x << '\n';
  }
}

// ### 9 ### GOOD
void print_vec_one_per_line_GOOD(const std::vector<int> &arr) {
  std::cout << "### 6 ### print_vec_one_per_line_BAD():\n";
  for (const auto &x: arr) {
    std::cout << x << '\n';
  }
}

// ### 10 ### GOOD
const char* string_literal_lifetimes()
{
  // literal is guaranteed to live for the entire life of a program
  // even thought it looks like it is a reference to a local variable
  return "string literals";
}

// ### 11 ### BAD
void loop_map_items_BAD()
{
  std::unordered_map<std::string, std::string> colors = {
    {"RED", "#FF0000"},
    {"GREEN", "#00FF00"},
    {"BLUE", "#0000FF"}
  };

  std::cout << "### 11 ### loop_map_items_BAD():\n";

  for (const auto &pair: colors) {
    std::cout << "name: " << pair.first << ", hex: " << pair.second << '\n';
  }
}

// ### 11 ### GOOD
void loop_map_items_GOOD()
{
  std::unordered_map<std::string, std::string> colors = {
    {"RED", "#FF0000"},
    {"GREEN", "#00FF00"},
    {"BLUE", "#0000FF"}
  };

  std::cout << "### 11 ### loop_map_items_GOOD():\n";

  for (const auto &[name, hex]: colors) {
    std::cout << "name: " << name << ", hex: " << hex << '\n';
  }
}

// ### 11 ### BETTER
struct S
{
  std::string name;
  std::string hex;
};

// ### 11 ### BETTER
void loop_map_items_BETTER()
{
  std::vector<S> colors = {
    {"RED", "#FF0000"},
    {"GREEN", "#00FF00"},
    {"BLUE", "#0000FF"}
  };

  std::cout << "### 11 ### loop_map_items_BETTER():\n";

  for (const auto &[name, hex]: colors) {
    std::cout << "name: " << name << ", hex: " << hex << '\n';
  }
}

// ### 12 ### BAD
void get_values_out_params(const int n, int &out1, int &out2)
{
  out1 = n;
  out2 = n + 1;
}

// ### 12 ### GOOD
struct Values
{
  int x, y;
};

// ### 12 ### GOOD
Values get_values_return_struct(const int n)
{
  return {n, n + 1};
}

// ### 12 ### GOOD
void use_values()
{
  [[maybe_unused]] const auto &[x, y] = get_values_return_struct(2); // structured binding
}

// ### 13 ### BAD
int sum_of_1_to_n_BAD(const int n)
{
  // return sum of the first n integers
  // Arithmetic progression sum(n) formula
  return n * (n + 1) / 2;
}

// ### 13 ### GOOD
constexpr int sum_of_1_to_n_GOOD(const int n)
{
  return n * (n + 1) / 2;
}

// ### 13 ###
void use_sum()
{
  const int limit = 1000;
  [[maybe_unused]] auto triangle_n_BAD = sum_of_1_to_n_BAD(limit);
  [[maybe_unused]] auto triangle_n_GOOD = sum_of_1_to_n_GOOD(limit);
}

// ### 14 ### BAD
class BaseWithNonvirtualDestructor
{
public:
  void foo() {
    std::cout << "do foo\n";
  }

  ~BaseWithNonvirtualDestructor() {                           // Use virtual here !
    std::cout << "called base destructor\n";
  }
};

// ### 14 ### BAD
class DerivedBad : public BaseWithNonvirtualDestructor
{
public:
  ~DerivedBad() {
    std::cout << "called derived destructor\n";
  }
};

// ### 14 ### BAD
void consume_base_bad(std::unique_ptr<BaseWithNonvirtualDestructor> p)
{
  p->foo();
  // deletes p when done
}

// ### 14 ### GOOD
class Base
{
public:
  void foo() {
    std::cout << "do foo\n";
  }

  virtual ~Base() {
    std::cout << "called base destructor\n";
  }
};

// ### 14 GOOD
class DerivedGood : public Base
{
public:
  ~DerivedGood() override {                                   // use override here !
    std::cout << "called derived destructor\n";
  }
};

// ### 14 ### GOOD
void consume_base_good(std::unique_ptr<Base> p)
{
  p->foo();
  // deletes p when done
}

// ### 16 ###
struct S2
{
  int n, m;
  std::string s;
};

// ### 16 ###
void default_vs_value_initialization()
{
  [[maybe_unused]] int x;               // x is default initialized. It is initialized by garbage
  [[maybe_unused]] int *x2 = new int;   // x2 is default initialized. It is initialized by garbage
  [[maybe_unused]] int x3();            // this is FUNCTION DECLARATION at all - not variable !!!
  delete x2;

  [[maybe_unused]] int y{};             // y is value initialized. It is initialized by 0
  [[maybe_unused]] int *y2 = new int{}; // y2 is value initialized. It is initialized by 0
  [[maybe_unused]] int *y3 = new int(); // y3 is value initialized. It is initialized by 0

  [[maybe_unused]] S2 o;                                  // o is default initialized. n and m is initialized by garbage. s is empty string.
  [[maybe_unused]] S2 *o2 = new S2;                        // o2 is default initialized. n and m is initialized by garbage. s is empty string.

  [[maybe_unused]] S2 p{};                                // p is value initialized. n and m is initialized by 0. s is empty string.
  [[maybe_unused]] S2 *p2 =  new S2{};                     // p2 is value initialized. n and m is initialized by 0. s is empty string.
  [[maybe_unused]] S2 *p3 = new S2();                      // p3 is value initialized. n and m is initialized by 0. s is empty string.
}

// ### 17 ###
float energy_bad(float m)
{
  return m * 299792458.0 * 299792458.0;
}

// ### 17 ###
float energy_good(float m)
{
  constexpr float SPEED_OF_LIGHT = 299792458.0;
  return m * SPEED_OF_LIGHT * SPEED_OF_LIGHT;
}

// ### 18 ###
void modify_while_iterating_BAD()
{
  std::cout << "### 18 ### modify_while_iterating_BAD():\n";
  std::vector<int> v{1, 2, 3, 4};

  // We're trying to put copy of the vector to the end of the vector (by pushing each iterated element) BAD
  for (auto x : v) {
    v.push_back(x);  // adding or removing an element to the vector may INVALIDATE THE ITERATORS to the vector
                     // push_back might need to resize the vector and move all the elements to a new location
  }

  // We're trying to put copy of the vector to the end of the vector (by pushing each iterated element) BAD
  for (auto it = v.begin(), end = v.end(); it != end; ++it) {
    v.push_back(*it);
  }

  for (auto x : v) {
    std::cout << x << ' ';
  }
  
  std::cout << '\n';
}

// ### 18 ###
void modify_while_iterating_GOOD()
{
  std::cout << "### 18 ### modify_while_iterating_GOOD():\n";
  std::vector<int> v{1, 2, 3, 4};

  // We're trying to put copy of the vector to the end of the vector GOOD
  const std::size_t size = v.size();
  for (std::size_t i = 0; i < size; ++i) {
    v.push_back(v[i]); // it doesn't matter if the contents of your vector get moved somewhere else the i-th element is still the i-th element
  }

  for (auto x : v) {
    std::cout << x << ' ';
  }
  
  std::cout << '\n';
}

// ### 19 ###
std::vector<int> make_vector_BAD([[maybe_unused]] const int n)
{
  std::vector<int> v{1, 2, 3, 4, 5};

  // do whatever with vector

  // Speaking of RVO (Return Value Optimization), return std::move(w); prohibits it
  // It means "use move constructor or fail to compile", whereas 
  // return w; means "use RVO, and if you can't, use move constructor, and if you can't, use copy constructor, and if you can't, fail to compile."
  return std::move(v); // BAD !
}

// ### 19 ###
std::vector<int> make_vector_GOOD([[maybe_unused]] const int n)
{
  std::vector<int> v{1, 2, 3, 4, 5};

  // do whatever with vector

  return v; // if you had just tried to return v directly, 
            // there would have been no copy and no move because of RVO (Return Value Optimization)
            // And even if compiler can't do RVO, the compile knows that it can move a local variable
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {

  // ### 5 ### Using a C-style array when you could haved used a standard library
  using_c_array_BAD();
  using_c_array_GOOD();


  // ### 6 ### Any use of reinterpret_cast<>()
  any_use_of_reinterpret_cast_cpp(); // BAD !
  any_use_of_reinterpret_cast_c();   // BAD !
  print_bytes(777777); // 0x0BDE31  // GOOD !
  print_bytes_better(777777); // 0x0BDE31  // BETTER !

  // ### 7 ### Casting away const
  more_frequent_BAD({{"a", 3}, {"b", 7}, {"c", 90}},
                    "a",
                    "c");
  more_frequent_GOOD({{"a", 3}, {"b", 7}, {"c", 90}},
                    "a",
                    "c");

  // ### 8 ### Not knowing that [] operator in map inserts the element into the map if it doesn't already exist

  // ### 9 ### Ignoring const correctness
  std::vector<int> nums {8, 4, 1};
  print_vec_one_per_line_BAD(nums);
  print_vec_one_per_line_GOOD(nums);

  // ### 10 ### Not knowing about string literal lifetimes
  string_literal_lifetimes();

  // ### 11 ### Not using structured bindings
  loop_map_items_BAD();
  loop_map_items_GOOD();
  loop_map_items_BETTER();

  // ### 12 ### Using multiple out parameters when you want to return multiple things from function
  int out1, out2;
  get_values_out_params(0, out1, out2); // BAD !
  [[maybe_unused]] const auto values = get_values_return_struct(0); // GOOD !
  use_values(); // GOOD !

  // ### 13 ### Doing work at runtime that could have been done at compile time
  use_sum();

  // ### 14 ### Forgetting to mark destructors virtual
  consume_base_bad(std::make_unique<DerivedBad>());
  consume_base_good(std::make_unique<DerivedGood>());

  // ### 15 ### Thinking that class members are initialized in the order they appear in the
  // initializer list

  // ### 16 ### Not realizing there's a difference between default and value initialization
  default_vs_value_initialization();

  // ### 17 ### Overuse of magic numbers
  energy_bad(7.3);
  energy_good(7.3);

  // ### 18 ### Attempting to add or remove elements from a container while looping over it
  modify_while_iterating_BAD();
  modify_while_iterating_GOOD();

  // ### 19 ### Returning a moved local variable
  auto make_vector_BAD_var = make_vector_BAD(3);
  auto make_vector_GOOD_var = make_vector_GOOD(2);
  
  return EXIT_SUCCESS;
}