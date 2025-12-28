#pragma once

#include <random>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <vector>

inline std::mt19937_64& globalRNG(uint64_t seed = 0, bool force = false) {
  static thread_local std::mt19937_64 rng;
  static thread_local bool seeded = false;

  if (!seeded || force) {
    if (seed == 0) {
      // Fall back to nondeterministic
      rng.seed(std::random_device{}());
    } else {
      // Deterministic
      rng.seed(seed);
    }
    seeded = true;
  }

  return rng;
}

inline void restoreRNG(int seed, std::mt19937_64 state) {
  std::mt19937_64& rng = globalRNG(seed, true);
  rng = state;
}

inline int bernoulli(double p) {
  p = std::clamp(p, 0.0, 1.0);
  std::bernoulli_distribution dist(p);
  return dist(globalRNG()); 
}

inline double chooseUnifRandDoubWithBounds(double low, double high) {
  if (low > high) {
    throw std::runtime_error("RandomHelper: Upper bound of random range is "
      "less than the lower bound.");
  }
  std::uniform_real_distribution<double> dist(low, high);
  return dist(globalRNG()); 
}

inline int chooseUnifRandIntWithBounds(int low, int high) {
  if (low > high) {
    throw std::runtime_error("RandomHelper: Upper bound of random range is "
      "less than the lower bound.");
  }
  std::uniform_int_distribution<int> dist(low, high - 1);
  return dist(globalRNG()); 
}

inline int chooseWeightedRandInt(const std::map<int, double>& probs) {
  std::vector<int> keys;
  std::vector<double> weights;

  for (const auto& [k, p] : probs) {
    keys.push_back(k);
    weights.push_back(p);
  }

  std::discrete_distribution<int> dist(weights.begin(), weights.end());
  return keys[dist(globalRNG())]; 
}