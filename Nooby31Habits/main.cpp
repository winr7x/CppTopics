// https://www.youtube.com/watch?v=i_wDa2AS_8w

#include <array>   // for std::array
#include <bit>     // for std::bit_cast
#include <cstdlib> // for EXIT_SUCCESS
#include <cstddef> // for std::to_integer
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

int main(int argc, char *argv[]) {
  (void)argc; (void)argv;


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

  return EXIT_SUCCESS;
}