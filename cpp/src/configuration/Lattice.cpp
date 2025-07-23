#include <array>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "Lattice.hpp"

Lattice::Lattice(std::map<std::string, std::pair<float, float>> lims, std::map<std::string, int> npts) {
  validateInputs(lims, npts);
  
  //TODO: Assume for now that xnpts and xlims are present, without checking.
  numSites = npts.at("xnpts"); //TODO: times ynpts times znpts
  float xstep = (lims.at("xlims").second - lims.at("xlims").first) / (npts.at("xnpts") - 1);
  sites.resize(npts.at("xnpts"));
  sites[0] = lims.at("xlims").first;
  for (int i = 1; i < npts.at("xnpts"); i++) {
    sites[i] = sites[i - 1] + xstep;
  }
};

void validateInputs(std::map<std::string, std::pair<float, float>> lims, std::map<std::string, int> npts) {
  //TODO: Check that if xlims is present, xnpts is too, and vice versa, and so on.
  std::unordered_set<std::string> validLimKeys = {"xlims", "ylims", "zlims"};
  std::unordered_set<std::string> validNptsKeys = {"xnpts", "ynpts", "znpts"};
  
  // Validate lims
  for (const auto& limsElem : lims) {
    if (validLimKeys.count(limsElem.first) == 0) {
      throw std::invalid_argument("Lattice limit key " + limsElem.first + " is not valid.");
    } else if (limsElem.second.first > limsElem.second.second) {
      throw std::invalid_argument("Lower lattice limit " + 
                std::to_string(limsElem.second.first) +
                " cannot be greater than upper limit " + 
                std::to_string(limsElem.second.second) + ".");
    }
  }

  // Validate npts 
  for (const auto& nptsElem : npts) {
    if (validNptsKeys.count(nptsElem.first) == 0) {
      throw std::invalid_argument("Npts key " + nptsElem.first + " is not valid.");
    } else if (nptsElem.second < 0) {
      throw std::invalid_argument("Npts in any direction must be at least 0, "
        "but was " + std::to_string(nptsElem.second)) + ".";
    }
  }
};

float Lattice::operator[](const int index) const {
  return sites[index];
};