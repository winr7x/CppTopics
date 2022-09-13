// https://leetcode.com/problems/two-sum/

#include <iostream>
#include <vector>

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

    for (int i = 0; i < static_cast<int>(nums.size()) - 1; ++i) {
      for (int j = i + 1; j < static_cast<int>(nums.size()); ++j) {
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
  std::cout << "}\n";
}

int main()
{
  Solution solution;

  print("solution.twoSum({2,7,11,15}, 9)", solution.twoSum({2,7,11,15}, 9));
  print("solution.twoSum({3,2,4}, 6)", solution.twoSum({3,2,4}, 6));
  print("solution.twoSum({2 1 3 7 8}, 15)", solution.twoSum({2,1,3,7,8}, 15));

  return 1;
}