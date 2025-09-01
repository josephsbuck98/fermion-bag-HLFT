#include <random>
#include <algorithm>


inline std::mt19937_64& globalRNG() {
    static thread_local std::mt19937_64 rng{std::random_device{}()};
    return rng;
}

inline int bernoulli(double p) {
  p = std::clamp(p, 0.0, 1.0);
  std::bernoulli_distribution dist(p);
  return dist(globalRNG());
}