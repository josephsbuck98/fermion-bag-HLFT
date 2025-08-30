#pragma once

#include <yaml-cpp/yaml.h>
#include <string>
#include <stdexcept>


template<typename T>
T getRequiredScalar(const YAML::Node& n, const std::string& key) {
  if (!n[key] || n[key].IsNull()) {
    throw std::runtime_error("YAML key '" + key + "' is defined but has no value");
  }
  if (!n[key].IsScalar()) {
    throw std::runtime_error("YAML key '" + key + "' must be a scalar");
  }
  
  try {
    return n[key].as<T>();
  } catch (const YAML::TypedBadConversion<T>&) {
    throw std::runtime_error("YAML key '" + key + "' cannot be converted to the requested type");
  }
}


template<typename T, typename MapType, typename DestType>
void assignFromMap(
  const YAML::Node& node,
  const std::string& key, 
  const MapType& lookupMap,
  DestType& dest, 
  const std::string& contextName = ""
) {
  if (!node[key]) return;
  T value = getRequiredScalar<T>(node, key);
  auto it = lookupMap.find(value);
  if (it == lookupMap.end()) {
    std::ostringstream message;
    message << contextName << ": '" << key << "' must be one of: ";
    for (auto it2 = lookupMap.begin(); it2 != lookupMap.end(); it2++) {
      message << it2->first;
      if (std::next(it2) != lookupMap.end()) {
        message << ", ";
      }
    }
    message << ".";
    throw std::runtime_error(message.str());
  }
  dest = it->second;
} 