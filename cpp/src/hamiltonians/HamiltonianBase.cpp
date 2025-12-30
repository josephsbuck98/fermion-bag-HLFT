#include "HamiltonianBase.hpp"
#include "LatticeHolder.hpp"
#include "RandomHelpers.hpp"


HamiltonianBase::HamiltonianBase(InputParser::ParsedInput input) {
  type = input.hamiltonianInput.model;
  acceptProb = input.hamiltonianInput.acceptProb;
  insertProb = input.hamiltonianInput.insertProb;
  bondTypeProps = input.hamiltonianInput.bondTypeProps;

  normalizeBondTypeProps();
}

void HamiltonianBase::normalizeBondTypeProps() {
  //TODO: Probably only want bonds of length 1 and 2. Rethink what you want here
  //TODO: Currently, only supports integer bond sizes from 1 to 6. ISNT BONDTYPEPROPS A MAP???
  double total = 0.0;
  for (int i = 1; i <= 6; i++) { //TODO: Use an enum for 6?
    double& val = bondTypeProps[i];
    if (val <= 0.0) val = 0.0;
    total += val;
  }

  // Normalize if total is non-zero
  if (total > 0.0) {
    for (int i = 1; i <= 6; i++) {
      bondTypeProps[i] /= total;
    }
  } else { // Otherwise, set to uniform distribution
    for (int i = 1; i <= 6; i++) {
      bondTypeProps[i] = 1.0 / 6.0;
    }
  }
}


consts::BondActionType HamiltonianBase::applyUpdate(Configuration& configuration,
    int groupNum, Configuration::RegionData& regionData, const LatticeBase* lattice) const {
  double tauGroupWidth = regionData.upper - regionData.lower;
  bool insertResult = bernoulli(insertProb); 

  // Null objects
  std::pair<double, int> tauToInsRem = {-1.0, -1}; 
  Bond newBond({});

  if (insertResult) {
    // Tau and bond to insert are only needed this early if they will be used 
    // to compute the accept probability (such as in TVModel)
    if (type == consts::HamilModel::TVModel) {
      tauToInsRem = chooseTauToInsert(regionData, groupNum);
      newBond = createBondToInsert(lattice);
    }
    double acceptInsertProb = getAcceptProb( 
        configuration, consts::BondActionType::INSERTION, tauGroupWidth, 
        regionData.nBondsInRegion, tauToInsRem, newBond);
    bool acceptInsert = bernoulli(acceptInsertProb);
    if (acceptInsert) {
      handleInsert(configuration, groupNum, regionData, lattice, tauToInsRem,
        newBond);
      return consts::BondActionType::INSERTION;
    }
  } else {
    if (type == consts::HamilModel::TVModel) {
      tauToInsRem = chooseTauToRemove(regionData);
    }
    double acceptRemoveProb = getAcceptProb(
        configuration, consts::BondActionType::REMOVAL, tauGroupWidth, 
        regionData.nBondsInRegion, tauToInsRem, newBond);
    bool acceptRemove = bernoulli(acceptRemoveProb);
    if (acceptRemove) {
      handleRemoval(configuration, regionData, tauToInsRem);
      return consts::BondActionType::REMOVAL;
    }
  }
  return consts::BondActionType::REJECTION;
}

void HamiltonianBase::handleInsert(Configuration& configuration, int groupNum, 
    Configuration::RegionData& regionData, const LatticeBase* lattice, 
    std::pair<double, int> tauToInsert, Bond newBond) const { 
  // If tauToInsert is a default value, we are using the Random Hamiltonian,
  // and the tau + bond to insert need to be chosen. Otherwise, we are using 
  // the TVModel Hamiltonian, and the tau + bond have already been chosen.
  if (tauToInsert.first < 0) {
    tauToInsert = chooseTauToInsert(regionData, groupNum);
    newBond = createBondToInsert(lattice);
  }

  // Insert into the bond with retries for duplicate taus
  std::pair<double, int> tauCandidate = tauToInsert; //TODO: !!!!!!!!!!!!!!!!!!!!PUT THIS INTO THE CONFIGURATION CLASS. TEST IT WELL.
  double tol = configuration.getTolerance();
  int maxAttempts = 100;
  for (int attempts = 0; attempts < maxAttempts; ++attempts) {
    try {
      configuration.addBond(tauCandidate, newBond);
      return;
    } catch(const std::exception& ex) {
      std::pair<double, int> tauUp = 
          std::pair<double, int>(tauCandidate.first + tol, tauCandidate.second);
      if (tauUp.first < regionData.upper) {
        tauCandidate = tauUp;
        continue;
      }
      std::pair<double, int> tauDown = tauCandidate;
      tauDown.first -= tol;
      if (tauDown.first > regionData.lower) {
        tauCandidate = tauDown;
        continue;
      }
      std::string message = "Could not insert tau=" + 
          std::to_string(tauToInsert.first) + " within " +
          "bounds [" + std::to_string(regionData.lower) + ", " +
          std::to_string(regionData.upper) + "] after " + 
          std::to_string(maxAttempts) + " attempts.";
      std::cout << message << std::endl;
      
      throw std::runtime_error(message);
    }
  }
}


void HamiltonianBase::handleRemoval(Configuration& configuration,
    Configuration::RegionData& regionData, 
    std::pair<double, int> tauToRemove) const {
  // If the tauToRemove is less than 0, we are using the Random Hamiltonian, 
  // and the tauToRemove still needs to be chosen. Otherwise, we are using the
  // TVModel Hamiltonian, and have already computed the tauToRemove. 
  if (tauToRemove.first < 0) {
    tauToRemove = chooseTauToRemove(regionData);
  }
  // If tauToRemove is still negative, there are no bonds in region to delete.
  if (tauToRemove.first >= 0) {
    configuration.delBond(tauToRemove);
  }

}


double HamiltonianBase::getAcceptProb(const Configuration& configuration,
    consts::BondActionType actionType, double tauGroupWidth, 
    int numBondsInRegion, std::pair<double, int> tauToInsRem, 
    const Bond& newBond) const {
  double prob; 
  if (actionType == consts::BondActionType::INSERTION) {
    prob = tauGroupWidth / (getBondSelectionProb() * numBondsInRegion);
  } else if (actionType == consts::BondActionType::REMOVAL) {
    prob = getBondSelectionProb() * numBondsInRegion / tauGroupWidth;
  } 
  prob *= getWeightFactor(configuration, actionType, tauToInsRem, newBond);



  std::cout << "Tau to Ins/Rem: " << tauToInsRem.first << std::endl;
  std::cout << "New Bond: " << newBond << std::endl;
  std::cout << "Num Bonds In Region: " << numBondsInRegion << std::endl;
  std::cout << "Accept Probability: " << prob << std::endl << std::endl << std::endl;



  return prob > 1.0 ? 1.0 : prob;
}


double HamiltonianBase::getBondSelectionProb() const {
  // TODO: This returns the probability of a uniformly random lattice site 
  // selection being the correct selection to move you from configuration i to 
  // j. For now, this assumes each allowed bond length in bondLengthProps is 
  // equally likely even though this is not physically realistic. It also 
  // assumes each spatial site starting point is equally realistic. It also 
  // goes without saying that this function works for only one dimensional 
  // simple cubic lattices.

  // int numUniqueBondSites = 0;
  // for (const auto& [bondLength, bondProb] : bondTypeProps) {
  //   if (bondProb < 0.001) continue;
  //   if (boundType == consts::BoundType::PERIODIC) {
  //     numUniqueBondSites += numSites;
  //   } else if (boundType == consts::BoundType::OPEN) {
  //     numUniqueBondSites += numSites - bondLength + 1;
  //   }
  // }
  // return 1.0 / numUniqueBondSites;
  
  const LatticeBase& lattice = getConstLattice();
  return 1.0 / lattice.getNumUniqueBonds();
}


Bond HamiltonianBase::createBondToInsert(const LatticeBase* lattice) const {
  int bondSize = chooseWeightedRandInt(bondTypeProps); //TODO: EVENTUALLY THE BONDTYPEPROPS WILL NOT BE AN INPUT PARAMETER
  
  
  
  const SiteBase* siteA = &(lattice->chooseRandSite());
  std::vector<const SiteBase*> nearestNeighbors = lattice->getNearestNeighbors(*siteA);
  //TODO: !!!!!!!!!!!!!!!!!!!!!What if there are no nearest neighbors? (Up against OPEN boundary)
  if (nearestNeighbors.size() == 0) {
    return Bond({});
  }
  int neighborInd = chooseUnifRandIntWithBounds(0, nearestNeighbors.size());
  const SiteBase* siteB = nearestNeighbors[neighborInd];
  
  std::set<const SiteBase*> bondSites;
  bondSites.insert(siteA);
  bondSites.insert(siteB);
  Bond newBond(bondSites);



  // int latticeBondStart;
  // int numSites = lattice->getNumSites(consts::DirsType::X);
  // if (lattice->getBoundType(consts::DirsType::X) == consts::BoundType::OPEN) {
  //   latticeBondStart = chooseUnifRandIntWithBounds(0, numSites - bondSize + 1);
  // } else { // Periodic boundary
  //   latticeBondStart = chooseUnifRandIntWithBounds(0, numSites);
  // }
  // std::set<int> bondSites;
  // for (int i = latticeBondStart; i < latticeBondStart + bondSize; i++) {
  //   bondSites.insert(i % numSites);
  // }
  // Bond newBond(bondSites);



  

  // // Ensure bondSize is less than or equal to Nsites
  // if (bondSize > lattice->getNumSites(consts::DirsType::X)) {
  //   throw std::runtime_error("bondSize=" + std::to_string(bondSize) + "is " +
  //       "larger than Nsites=" + std::to_string(lattice->getNumSites(consts::DirsType::X)));
  // }

  return newBond;
}


std::pair<double, int> HamiltonianBase::chooseTauToInsert(
    Configuration::RegionData& regionData, int groupNum) const {
  return std::pair<double, int>
      (chooseUnifRandDoubWithBounds(regionData.lower, regionData.upper), 
      groupNum);
}


std::pair<double, int> HamiltonianBase::chooseTauToRemove(
    Configuration::RegionData& regionData) const {
  // If there are no bonds in the region, return a default tau
  if (regionData.nBondsInRegion == 0) {
    return std::pair<double, int>({-1.0, -1});
  }

  auto it = regionData.itLow;
  int indToDelete = chooseUnifRandIntWithBounds(0, regionData.nBondsInRegion);
  std::advance(it, indToDelete);
  return *it;
}