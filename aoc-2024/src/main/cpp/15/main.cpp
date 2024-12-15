#include <iostream>
#include <string>
#include <sstream>
#include <numeric>
#include <ranges>

namespace views = std::views;

/* ------------------------------------------------------------------------ */

struct Dir {
  int dx, dy;

  Dir& operator*=(auto n) {
    dx *= n;
    dy *= n;
    return *this;
  }
  friend Dir operator*(Dir d, auto n) {
    return d *= n;
  }
  Dir operator-() {
    return Dir{-dx, -dy};
  }
  inline auto friend operator<=>(const Dir& a, const Dir& b) = default;
};

/* ------------------------------------------------------------------------ */

struct Pos {
  int x, y;

  Pos& operator+=(Dir d) {
    x += d.dx;
    y += d.dy;
    return *this;
  }
  friend Pos operator+(Pos p, Dir d) {
    return p += d;
  }
  Pos& operator-=(Dir d) {
    return (*this) += -d;
  }
  friend Pos operator-(Pos p, Dir d) {
    return p -= d;
  }

  inline auto friend operator<=>(const Pos& a, const Pos& b) = default;
};

/* ------------------------------------------------------------------------ */

template<typename T>
class Map {
  using value_type = std::conditional<std::is_same<T, bool>::value, char, T>::type;
  int w_, h_;
  std::vector<value_type> data_;
  const value_type off_;

  template <typename V>
  struct Iter {
    int x, y;
    V v;
  };
  template <typename M>
  static auto iter_(M& map) {
    return views::iota(0UZ, map.data_.size()) | views::transform([&map](int i) { return Iter<decltype(map[0,0])>{i % map.w_, i / map.w_, map.data_[i]}; });
  }
public:
  template<typename Tr = std::identity>
  Map(std::vector<std::string> lines, Tr tr = {}, value_type off = {}) : w_(lines[0].size()), h_(lines.size()), data_{}, off_(off) {
    data_.reserve(w_ * h_);
    for (const auto& line: lines) {
      for (char c: line) {
        data_.push_back(tr(c));
      }
    }
  }

  Map(int w, int h, T init = {}, T off = {}) : w_(w), h_(h), data_(w * h, init), off_(off) {}

  const value_type& operator[](int x, int y) const {
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return data_[y * w_ + x];
    }
    return off_;
  }
  value_type& operator[](int x, int y) {
    static value_type off;
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return data_[y * w_ + x];
    }
    return off = off_;
  }
  const value_type& operator[](Pos pos) const { return (*this)[pos.x, pos.y]; }
  value_type& operator[](Pos pos) { return (*this)[pos.x, pos.y]; }

  int w() const { return w_; }
  int h() const { return h_; };

  auto iter() { return iter_(*this); }
  auto iter() const { return iter_(*this); }
};

using Board = Map<char>;

/* ------------------------------------------------------------------------ */

static Dir
dir(char c) {
  switch (c) {
    case '^': return {0, -1};
    case '>': return {1, 0};
    case 'v': return {0, 1};
    case '<': return {-1, 0};
  }
  return {};
}

static Pos
move(Board& board, Pos p, Dir d) {
  auto pp = p + d;
  switch (board[pp]) {
    case '#':
      return p;
    case 'O':
      if (move(board, pp, d) == pp) {
        return p;
      }
  }
  std::swap(board[pp], board[p]);
  return pp;
}

static void
print(const Board& board) {
  for (int y = 0; y < board.h(); ++y) {
    for (int x = 0; x < board.w(); ++x) {
      std::cout << board[x, y];
    }
    std::cout << "\n";
  }
}

/* ------------------------------------------------------------------------ */

int
main() {

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line) && !line.empty()) {
    lines.push_back(line);
  }
  auto moves = std::vector<char>(std::istream_iterator<char>(std::cin), std::istream_iterator<char>());
  auto board = Board(lines);
  Pos start = (board.iter() | views::filter([](auto i) { return i.v == '@'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();

  Pos p = start;
  for (char c : moves) {
    p = move(board, p, dir(c));
  }
  auto gpsScores = board.iter() | views::filter([](auto i) { return i.v == 'O'; }) | views::transform([](auto i) { return i.y * 100 + i.x; });
  int res1 = std::reduce(gpsScores.begin(), gpsScores.end(), 0);
  std::cout << "1: " << res1 << "\n";

  return 0;
}