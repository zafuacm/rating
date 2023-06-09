#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using u8 = char;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;
using f32 = float;
using f64 = double;
using f80 = long double;

// https://github.com/meooow25/carrot/blob/master/carrot/src/background/predict.js


const i32 INIT_RATING = 1400;

struct Contestant {
  std::string name{};
  i32 rating = 0;
  i32 original_rank = -1;

  i32 rank = 0;
  f80 seed = 0;
  i32 delta = 0;
  i32 performance = 0;
  Contestant(const std::string &name_, i32 rating_, i32 rank_)
      : name(name_), rating(rating_), original_rank(rank_) {}
};

bool sort_by_rating(const Contestant &l, const Contestant &r) {
  return l.rating > r.rating;
}

bool sort_by_rank(const Contestant &l, const Contestant &r) {
  return u32(l.rank) < u32(r.rank);
}

bool sort_by_original_rank(const Contestant &l, const Contestant &r) {
  return u32(l.original_rank) < u32(r.original_rank);
}

f80 win_p(f80 ri, f80 rj) { return 1 / (1 + std::pow(10, (rj - ri) / 400)); }

struct Calculator {
  std::map<std::string, i32> old_rating;
  std::vector<Contestant> contestant;
  i32 adjust = 0;

  void read_past_rating(const std::string &path);
  void read_contestant(const std::string &path);
  void write_result(const std::string &path);

  void calc() {
    calcSeed();
    reassignRanks();
    calcDeltas();
    adjustDeltas();
    calcPerfs();
  }

  void calcSeed();
  void reassignRanks();
  void calcDeltas();
  void adjustDeltas();
  void calcPerfs();

  i32 calcDelta(Contestant &c, i32 assumedRating);
  f80 getSeed(i32 rating, i32 exclude);
};

void Calculator::read_past_rating(const std::string &path) {
  std::ifstream rating_file(path, std::ios::in);
  if (!rating_file.is_open()) {
    std::cerr << "[error]: open past-rating '" << path << "' failed."
              << std::endl;
    std::exit(1);
  }
  i32 number = 0;
  rating_file >> number;
  std::cerr << "reading " << number << " logs." << std::endl;

  // rank rating perf name
  for (i32 i = 1; i <= number; ++i) {
    std::string name;
    i32 rating, _;
    rating_file >> _ >> rating >> _ >> name;
    if (old_rating.count(name)) {
      std::cerr << name << ' ' << rating << std::endl;
      std::cerr << "[error]: Contestant '" << name << "' apperance twice."
                << std::endl;
      std::exit(1);
    }
    old_rating[name] = rating;
  }
}

void Calculator::read_contestant(const std::string &path) {
  std::ifstream rank_file(path, std::ios::in);

  if (!rank_file.is_open()) {
    std::cerr << "[error]: open contest-rank '" << path << "' failed."
              << std::endl;
    std::exit(1);
  }
  i32 number = 0;
  rank_file >> number;
  std::cerr << "reading " << number << " logs." << std::endl;

  std::vector<std::pair<std::string, i32>> rank;
  for (i32 i = 1; i <= number; ++i) {
    std::string name;
    i32 rank, rating;
    rank_file >> name >> rank;
    if (old_rating.count(name)) {
      rating = old_rating[name];
    } else {
      std::cerr << "[warn]: " << name << " not exists, init with " << INIT_RATING << "."
                << std::endl;
      rating = INIT_RATING;
    }
    contestant.emplace_back(name, rating, rank);
  }
}

void Calculator::write_result(const std::string &path) {

  std::ofstream output(path, std::ios::out);
  if (!output.is_open()) {
    std::cerr << "[error]: open output '" << path << "' failed." << std::endl;
    std::exit(1);
  }

  std::map<std::string, bool> in_contest;
  std::vector<Contestant> after = contestant;
  for (auto &ci : after) {
    in_contest[ci.name] = true;
    ci.rating += ci.delta + adjust;
  }

  for (const auto &[name, rating] : old_rating) {
    if (!in_contest[name]) {
      after.emplace_back(name, rating, -1);
      after.back().performance = -1;
      after.back().rank = -1;
    }
  }

  // for (auto &ci : after) {
  //   if (!old_rating.count(ci.name)) {
  //     ci.rating = ci.delta;
  //   }
  // }

  std::sort(after.begin(), after.end(), sort_by_rating);

  output << after.size() << std::endl;

  // rank rating perf name
  for (const auto &ci : after) {
    output << ci.rank << '\t' << ci.rating << '\t' << ci.performance << '\t'
           << ci.name << std::endl;
  }
}

void Calculator::calcSeed() {
  for (auto &ci : contestant) {
    ci.seed = getSeed(ci.rating, ci.rating);
  }
}

void Calculator::reassignRanks() {
  std::sort(contestant.begin(), contestant.end(), sort_by_original_rank);
  i32 rank = contestant.size(), past = -1;
  for (i32 i = rank - 1; i >= 0; i--) {
    i32 cur = contestant[i].original_rank;
    if (past != cur) {
      past = cur, rank = i + 1;
    }
    contestant[i].rank = rank;
  }
}

f80 Calculator::getSeed(i32 rating, i32 exclude) {
  f80 s = -win_p(exclude, rating);
  for (const auto &ci : contestant) {
    s += win_p(ci.rating, rating);
  }
  return s + 1;
}

i32 Calculator::calcDelta(Contestant &c, i32 assumedRating) {
  f80 seed = getSeed(assumedRating, c.rating);
  f80 mid = std::sqrt(seed * c.rank);

  i32 L = 2, R = 6000;
  while (R - L > 1) {
    i32 M = (L + R) / 2;
    bool flag = getSeed(M, c.rating) < mid;
    (flag ? R : L) = M;
  }
  return (L - 1 - assumedRating) / 2;
}

void Calculator::calcDeltas() {
  for (auto &ci : contestant) {
    ci.delta = calcDelta(ci, ci.rating);
  }
}

void Calculator::adjustDeltas() {
  f80 dsum = 0;
  for (auto &ci : contestant) {
    dsum += ci.delta;
  }
  adjust = std::round(-dsum / contestant.size()) - 1;
}

void Calculator::calcPerfs() {
  for (auto &ci : contestant) {
    if (ci.rank == 1) {
      ci.performance = -1;
    } else {
      i32 L = -500, R = 6000;
      while (R - L > 1) {
        i32 M = (L + R) / 2;
        auto cd = calcDelta(ci, M);
        bool flag = cd + adjust <= 0;
        (flag ? R : L) = M;
      }
      ci.performance = L;
    }
  }
}

i32 main(i32 argv, u8 *args[]) {
  if (argv <= 3) {
    std::cerr << "usage: rating <past-rating> <contest-rank> <output>"
              << std::endl;
    std::exit(0);
  }
  auto &past_ratingpath = args[1];
  auto &rank_path = args[2];
  auto &output_path = args[3];

  Calculator c;
  c.read_past_rating(args[1]);
  c.read_contestant(args[2]);

  c.calc();

  c.write_result(args[3]);

  return 0;
}
