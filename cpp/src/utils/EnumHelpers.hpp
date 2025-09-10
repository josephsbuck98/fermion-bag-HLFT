#pragma once
#include <unordered_map>
#include <stdexcept>
#include <string>

template <typename K, typename V>
K keyFromValue(const std::unordered_map<K, V>& map, const V& value) {
  for (const auto& [key, val] : map) {
    if (val == value) {
      return key;
    }
  }
  throw std::invalid_argument("Value not found in map");
}
