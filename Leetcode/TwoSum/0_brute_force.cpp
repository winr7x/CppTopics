// https://leetcode.com/problems/two-sum/

// This is brute force solution with complexity O(n^2)

// Usage: ./<binary>                  - default output
//        ./<binary> true             - verbose output

#include <iostream>
#include <vector>

bool debug {false};

void logCheckPair(const int i, const int j, const int iNum, const int jNum)
{
  if (debug) {
    std::cout << "Check i,j (" << i << "," << j << ") (" << iNum << "+" << jNum << ")\n";
  }
}

class Solution
{
public:

  // Algorithm:
  // input: 2 1 3 7 8, target = 15

  // Iteration:

  // 2 1 3 7 8
  // ^ *

  // 2 1 3 7 8
  // ^   *

  // 2 1 3 7 8
  // ^     *

  // 2 1 3 7 8
  // ^       *

  // 2 1 3 7 8
  //   ^ *

  // 2 1 3 7 8
  //   ^   *

  // 2 1 3 7 8
  //   ^     *

  // 2 1 3 7 8
  //     ^ *

  // 2 1 3 7 8
  //     ^   *

  // 2 1 3 7 8
  //       ^ *

  std::vector<int> twoSum(const std::vector<int> &nums, int target) {

    const auto beforeLast = static_cast<int>(nums.size()) - 1;            // iterate up to penultimate element !

    for (int i = 0; i < beforeLast; ++i) {
      for (int j = i + 1; j < static_cast<int>(nums.size()); ++j) {
        logCheckPair(i, j, nums[i], nums[j]);
        if (nums[i] + nums[j] == target) {
          return {i, j};
        }
      }
    }
    return {0, 0};

  }
};

void print(const std::string &input, const std::vector<int> &result)
{
  std::cout << input << " --> { ";
  for (const auto &r : result) {
    std::cout << r << " ";
  }
  std::cout << "}\n\n";
}

int main(int argc, char *argv[])
{
  const auto debugArg = std::string(argv[1]);
  const auto wasDebugArgSet = argc > 1 &&  debugArg == "true";

  if (wasDebugArgSet) {
    debug = true;
  }

  Solution solution;

  print("solution.twoSum({2,7,11,15}, 9)", solution.twoSum({2,7,11,15}, 9));
  print("solution.twoSum({3,2,4}, 6)", solution.twoSum({3,2,4}, 6));
  print("solution.twoSum({2 1 3 7 8}, 15)", solution.twoSum({2,1,3,7,8}, 15));

  return EXIT_SUCCESS;
}