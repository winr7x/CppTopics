// Task: https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/
// Solution: https://youtu.be/cQ1Oz4ckceM?t=242  <------------------------------- SEE ALGORITHM HERE!

// This is left-right pointers solution with complexity O(n)

// Usage: ./<binary>                  - default output
//        ./<binary> true             - verbose output

#include <iostream>
#include <vector>

bool debug {false};

void logShiftRight(const int iNum, const int jNum, const int target, const int j)
{
  if (debug) {
    std::cout << iNum << " + " << jNum << " > " << target << "(target): Shift right pointer from index " << j << " to " << j - 1 << "\n";
  }
}

void logShiftLeft(const int iNum, const int jNum, const int target, const int i)
{
  if (debug) {
    std::cout << iNum << " + " << jNum << " < " << target << "(target): Shift left pointer from index " << i << " to " << i + 1 << "\n";
  }
}

void logSuccess(const int iNum, const int jNum, const int target)
{
  if (debug) {
    std::cout << iNum << " + " << jNum << " == " << target << "(target): Solution is found\n";
  }
}

void logNotFound(const int i, const int j)
{
  if (debug) {
    std::cout << "!(i < j) !(" << i << " < " << j << "): Solution is not found\n";
  }
}

void print(const std::string &input, const std::vector<int> &result)
{
  std::cout << input << " --> { ";
  for (const auto &r : result) {
    std::cout << r << " ";
  }
  std::cout << "}\n\n";
}

class Solution
{
public:
  std::vector<int> twoSum(const std::vector<int> &nums, int target) {

    std::vector<int> result {0, 0};

    auto i = 0;
    auto j = static_cast<int>(nums.size()) - 1;
    do {
        if (nums[i] + nums[j] == target) {              logSuccess(nums[i], nums[j], target);
          result = {i, j};
          break;
        } else if (nums[i] + nums[j] > target) {        logShiftRight(nums[i], nums[j], target, j);
          --j;
        } else if (nums[i] + nums[j] < target) {        logShiftLeft(nums[i], nums[j], target, i);
          ++i;
        }
    } while (i < j);                                    logNotFound(i, j);
    
    // do 1-indexed result
    return {result.front() + 1, result.back() + 1};

  }
};

int main(int argc, char *argv[])
{
  Solution solution;

  const auto debugArg = std::string(argv[1]);
  const auto wasDebugArgSet = argc > 1 &&  debugArg == "true";

  if (wasDebugArgSet) {
    debug = true;
  }

  print("solution.twoSum({1,3,4,5,7,10,11}, 9)", solution.twoSum({1,3,4,5,7,10,11}, 9));
  print("solution.twoSum({1,3,4}, 9)", solution.twoSum({1,3,4}, 9));

  return EXIT_SUCCESS;
}