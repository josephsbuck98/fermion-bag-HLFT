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