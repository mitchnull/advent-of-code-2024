#include <iostream>
#include <cstdint>
#include <numeric>
#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

using uint = std::uint32_t;
using Strings = std::vector<std::string>;
using Nums = std::vector<int>;


int
main() {
  auto text = std::string(std::istreambuf_iterator<char>(std::cin), {});
  std::transform(text.begin(), text.end(), text.begin(), [](auto c) { return std::isspace(c) ? ' ' : c; });

  auto r = std::regex("mul[(]([0-9]{1,3}),([0-9]{1,3})[)]");

  int res1 = 0;
  for (auto it = std::sregex_token_iterator(text.begin(), text.end(), r, {1, 2}), end = std::sregex_token_iterator(); it != end; ++it) {
    res1 += std::stoi(*it) * std::stoi(*++it);
  }
  std::cout << "1: " << res1 << "\n";

  // ------------------------------------------------------------------------ //

  auto dr = std::regex("don't[(][)].*?(do[(][)]|$)");
  std::string text2 = std::regex_replace(text, dr, "");

  int res2 = 0;
  for (auto it = std::sregex_token_iterator(text2.begin(), text2.end(), r, {1, 2}), end = std::sregex_token_iterator(); it != end; ++it) {
    res2 += std::stoi(*it) * std::stoi(*++it);
  }
  std::cout << "2: " << res2 << "\n";

  return 0;
}