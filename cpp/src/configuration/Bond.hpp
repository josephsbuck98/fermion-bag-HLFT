#pragma once

#include <iostream>
#include <set>

#include "LatticeBase.hpp"


// class Site; // Forward declaration

class Bond { 
public:
  //DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // Bond(const std::set<int>& indices);

  struct SiteSumLess {
    bool operator()(const SiteBase* a, const SiteBase* b) const {
      // compute sum of coordinates
      double sumA = a->xi + a->yi + a->zi;
      double sumB = b->xi + b->yi + b->zi;

      // sort ascending by sum
      if (sumA != sumB) return sumA < sumB;
      return a < b;
    }
  };

  Bond(const std::set<const SiteBase*>& sites_);

  int getNumSites() const;
  // const std::set<int>& getIndices() const;
  const std::set<const SiteBase*, SiteSumLess>& getSites() const;

  bool operator==(const Bond& other) const;
  bool operator!=(const Bond& other) const;

  friend std::ostream& operator<<(std::ostream& os, const Bond& bond);

private:
  int numSites;

  //DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // std::set<int> indices; 

  std::set<const SiteBase*, SiteSumLess> sites;
};
