// This is the solution for task https://youtu.be/pG_ziTMbPWA?t=1099 using two stacks:
// 1. operators stack (see ZZ1)
// 2. arguments stack (see ZZ2)

#include <iostream>
#include <format>
#include <numeric>
#include <stack>
#include <vector>

using std::cout;
using std::format;
using std::stack;
using std::vector;

using namespace std::string_literals;

// parse expressions like these:
// ( * 2 2 3 1 ) ---> 12
// ( + ( * 2 2 ) ( * 3 3 ) ) --> 13
// ( + ( * 3 2 ) 7 2 ( + 9 9 ) ) ---> 33
int parsePolishNotation(const std::string& expression, bool debug = false) {
    stack<char> operatorsStack; // ZZ1
    stack<vector<int>> argumentsStack; // ZZ2

    for (const auto& token : expression) {
        if (token == ')')
        {
            const auto operator_ = operatorsStack.top(); operatorsStack.pop();
            const auto arguments = argumentsStack.top(); argumentsStack.pop();

            auto result =
                operator_ == '+'
                ? std::accumulate(std::begin(arguments), std::end(arguments), 0)
                : std::accumulate(std::begin(arguments), std::end(arguments), 1, std::multiplies<int>());

            if (argumentsStack.empty()) {
                return result;
            }
            else {
                argumentsStack.top().push_back(result);
            }
        }
        else if (token == '+' || token == '*')
        {
            operatorsStack.push(token);
            argumentsStack.push({});
        }
        else if (isdigit(token)) {
            argumentsStack.top().push_back(int(token - '0'));
        }
    }

    return 0;
}

int main()
{
    vector<std::string> examples = {
        "( * ( + ( * 7 2 ) 1 ) 3 ( * 4 5 ) 2 )",
        "( * 4 5 3 )",
        "( + 2 7 ( * 8 9 ) )",
        "( + ( * 3 2 ) 7 2 ( + 9 9 ) )"
    };
    for (const auto& example : examples) {
        cout << format("'{}' --> {}\n", example, parsePolishNotation(example));
    }
}
