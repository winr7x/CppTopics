// https://leetcode.com/problems/two-sum/

// This is hash map solution with complexity O(n)

// Usage: ./<binary>                  - default output
//        ./<binary> true             - verbose output

#include <iostream>
#include <map>
#include <vector>

bool debug {false};

void logInsert(const int number)
{
  if (debug) {
    std::cout << "insert " << number << " to map\n";
  }
}

void logContain(const int searchNum)
{
  if (debug) {
    std::cout << "map contains " << searchNum << "\n";
  }
}

void logSearchInMap(const int target, const int currNum)
{
  if (debug) {
    std::cout << "search in map " << target - currNum << " (" << target << " - " << currNum << ")\n";
  }
}

class Solution
{
public:
  std::vector<int> twoSum(const std::vector<int> &nums, int target) {

    std::map<int, int> map;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
      const auto currNum = nums[i];
      const auto searchNum = target - currNum;         logSearchInMap(target, currNum);       // we search in map value == target - currNum !
      if (map.contains(searchNum)) {                   logContain(searchNum);                 // if value found then we succeeded !
          const auto searchNumIndex = map[searchNum];
          return {searchNumIndex, i};
      } else {
        map[currNum] = i;                              logInsert(currNum);                    // insert number each iteration until target - currNum will not found in map
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
  Solution solution;

  const auto debugArg = std::string(argv[1]);
  const auto wasDebugArgSet = argc > 1 &&  debugArg == "true";

  if (wasDebugArgSet) {
    debug = true;
  }

  print("solution.twoSum({2,7,11,15}, 9)", solution.twoSum({2,7,11,15}, 9));
  print("solution.twoSum({3,2,4}, 6)", solution.twoSum({3,2,4}, 6));
  print("solution.twoSum({2 1 3 7 8}, 15)", solution.twoSum({2,1,3,7,8}, 15));
  print("solution.twoSum({1 1 1}, 15)", solution.twoSum({1,1,1}, 15));

  return EXIT_SUCCESS;
}