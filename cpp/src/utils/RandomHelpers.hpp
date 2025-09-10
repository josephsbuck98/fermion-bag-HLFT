#include <random>
#include <algorithm>
#include <map>
#include <vector>

inline std::mt19937_64& globalRNG() {
  //TODO: Use the randomSeed from user input 

  // static thread_local std::mt19937_64 rng;
  //   static thread_local bool seeded = false;

  //   if (!seeded || force) {
  //       if (seed == 0) {
  //           // fallback: nondeterministic
  //           rng.seed(std::random_device{}());
  //       } else {
  //           // deterministic
  //           rng.seed(seed);
  //       }
  //       seeded = true;
  //   }

  //   return rng;
  
  static thread_local std::mt19937_64 rng{std::random_device{}()};
  return rng;
}

inline int bernoulli(double p) {
  p = std::clamp(p, 0.0, 1.0);
  std::bernoulli_distribution dist(p);
  return dist(globalRNG()); 
}

inline double chooseUnifRandDoubWithBounds(double low, double high) {
  std::uniform_real_distribution<double> dist(low, high);
  return dist(globalRNG()); 
}

inline int chooseUnifRandIntWithBounds(int low, int high) {
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