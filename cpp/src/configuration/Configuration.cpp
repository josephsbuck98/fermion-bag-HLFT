#include <stdexcept>
#include <cmath>
#include <string>

#include "Configuration.hpp"
#include "LatticeHolder.hpp"

//TODO: Error/special case handling (for example, if getBond is called with a tau that doesn't exist.)
//TODO: Implement delBonds (with an s)

Configuration::Configuration(ConfigurationInput input) {
  tolerance = input.floatTol;
  beta = input.beta;
  bonds = {};
  taus = {};
  tauGroupStarts = generateTauGroupStarts(beta, input.numTimeGroups);
}

void Configuration::validate() const {
  if (taus.size() != bonds.size()) {
    throw std::runtime_error("Configuration: The number of taus was not equal "
        "to the number of bonds in the configuration.");
  }
  for (const auto& [tau, _] : taus) {
    Bond bond = getBond(tau);
    if (bond.getNumSites() == 0) {
      throw std::runtime_error("Configuration: A tau value from taus was not "
          "found in bonds.");
    }
  }
}

void Configuration::setTauGroupStarts(std::vector<double> newTauGroupStarts) {
  tauGroupStarts = newTauGroupStarts; 

  std::vector<std::pair<double, int>> updatedTaus;

  for (const auto& tau : taus) {
    double tauVal = tau.first;

    int groupNum = 0;
    while (true) {
      if (groupNum == tauGroupStarts.size() - 1) break;
      if ((tauVal >= tauGroupStarts[groupNum]) && 
          (tauVal < tauGroupStarts[groupNum + 1])) break;
      groupNum++;
    }

    updatedTaus.push_back({tauVal, groupNum});
  }

  taus.clear();
  taus.insert(updatedTaus.begin(), updatedTaus.end());
}

const std::vector<double>& Configuration::getTauGroupStarts() const {
  return tauGroupStarts;
}

const std::set<std::pair<double, int>>& Configuration::getTaus() const {
  return taus;
}

bool Configuration::setTolerance(double tol) {
  tolerance = tol;
  return true;
}

double Configuration::getTolerance() const {
  return tolerance;
}

std::vector<double> generateTauGroupStarts(double beta, int numTimeGroups) {
  double groupWidth = beta / numTimeGroups;
  std::vector<double> tauGroupStarts(numTimeGroups);
  tauGroupStarts[0] = 0.0;
  for (int i = 1; i < numTimeGroups; i++) {
    tauGroupStarts[i] = i * groupWidth;
  }
  return tauGroupStarts;
}

void Configuration::addBond(std::pair<double, int> tau, Bond& bond) {
  std::pair<double, int> tauTrunc = std::pair<double, int>
      (truncateToTolerance(tau.first), tau.second);
  auto retSetPair = taus.insert(tauTrunc);
  auto retMapPair = bonds.insert({tauTrunc.first, bond});

  std::string eMS = "Insert failed: tau=" + std::to_string(tau.first);
  if (!retSetPair.second) {
    if (!retMapPair.second) {
      throw std::runtime_error(eMS + " already exists in both taus and bonds.");
    } else {
      throw std::runtime_error(eMS + " already exists in taus.");
    }
  } else if (!retMapPair.second) {
    throw std::runtime_error(eMS + " already exists in bonds.");
  }
  bondsPerType[bond.getNumSites()]++;
}

void Configuration::addBonds(std::vector<std::pair<double, int>> newTaus, 
      std::vector<Bond> newBonds) {
  if (newTaus.size() != newBonds.size()) {
    throw std::runtime_error("Lengths of newTaus and newBonds must be the "
                  "same, but were " + std::to_string(newTaus.size()) + " and"
                  " " + std::to_string(newBonds.size()));
  }
  for (int i = 0; i < newBonds.size(); i++) {
    try {
      addBond(newTaus[i], newBonds[i]);
    } catch (std::runtime_error err) {
      std::cout << err.what() << std::endl;
    }
  }
}

void Configuration::delBond(std::pair<double, int> tau) {
  std::pair<double, int> tauTrunc = 
      std::pair<double, int>(truncateToTolerance(tau.first), tau.second);

  int numSites = 0; // Get the number of sites associated with the bond
  auto it = bonds.find(tauTrunc.first);
  if (it != bonds.end()) {
    numSites = it->second.getNumSites();
  }

  size_t retSet = taus.erase(tauTrunc);
  size_t retMap = bonds.erase(tauTrunc.first);
  if (numSites == 0 || retSet == 0 || retMap == 0) {
    std::runtime_error err("Cannot delete element with tau"
              "=" + std::to_string(tau.first) + ". Element does not exist.");
    std::cout << err.what() << std::endl;
    throw err;
  }

  bondsPerType[numSites]--; // Decrement the number of bonds of this size
  if (bondsPerType[numSites] == 0) bondsPerType.erase(numSites);
}

void Configuration::delBonds() {
  bonds = std::map<double, Bond>();
  taus = std::set<std::pair<double, int>>();
  bondsPerType = std::map<int, int>();
}

const Bond& Configuration::getBond(double tau) const {
  auto it = bonds.find(truncateToTolerance(tau));
  if (it != bonds.end()) {
    return it->second;
  } 
  static const Bond nullBond({});
  return nullBond;
}

const std::map<double, Bond>& Configuration::getBonds() const {
  return bonds;
}

int Configuration::getNumBonds() const {
  return taus.size();
}

std::map<int, int> Configuration::getBondsPerType() const {
  return bondsPerType;
}

bool Configuration::setBeta(double b) {
  beta = b;
  return true;
}

double Configuration::getBeta() const {
  return beta;
}

Eigen::MatrixXd Configuration::getHProd(int nDims, double omega, 
    double cosh2alpha, double sinh2alpha, double tau, const Bond& bond) const {
  //TODO: YOU CAN STORE W IN CLEVER WAYS SO THAT YOU DON'T HAVE TO RECLCULATE THE CURRENT W IF THE CONFIGURATION WAS UNCHANGED FROM THE LAST ATTEMPT. BASICALLY, YOU COULD STORE WCURR AND JHUST MAKE SURE TO SET IT EITHER TO WINS OR WREM (OR LEAVE UNCHANGED) BASED ON WHAT WAS ACCEPTED 
  
  Eigen::MatrixXd mat = Eigen::MatrixXd::Identity(nDims, nDims);

  bool computingWkm1 = false;
  bool computingWk = tau < 0;
  if (!computingWk && bond.getIndices().count(-1)) {
    computingWkm1 = true;
  }

  if (this->getNumBonds() == 0 && (computingWk || computingWkm1)) {
    return Eigen::MatrixXd::Zero(nDims, nDims);
  }

  bool completedExtraStep = computingWk; // If computing Wk, extra step is completed by definition

  for (auto& currTau : taus) { //TODO: For now, this is designed for bonds of length 2.
    if (!completedExtraStep) {
      if (computingWkm1) { // Run when computing Wkm1
        if (truncateToTolerance(currTau.first) == truncateToTolerance(tau)) {
          completedExtraStep = true;
          continue;
        }
      } else { // Run when computing Wkp1
        if (currTau.first > tau) {
          multToHProd(mat, bond, cosh2alpha, sinh2alpha);
          completedExtraStep = true;
        }
      }
    }
    multToHProd(mat, getBond(truncateToTolerance(currTau.first)), cosh2alpha, 
        sinh2alpha);
  }
  if (!completedExtraStep && !computingWkm1) { // If the tau to insert is greater than greatest tau, we will get here
    multToHProd(mat, bond, cosh2alpha, sinh2alpha);
  }
  return mat;
}

void Configuration::multToHProd(Eigen::MatrixXd& hProdMat, const Bond& bond,
    double cosh2alpha, double sinh2alpha) const {
  //TODO: MAKE COMPATIBLE WITH SITES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  std::set<int> bondInds = bond.getIndices(); 
  int lenBond = bondInds.size();
  if (lenBond != 2) {
    throw std::runtime_error("Configuration::getHSum(): Bonds with lengths "
        "other than 2 are not currently supported.");
  }
  std::vector<int> bondIndsVec(bondInds.begin(), bondInds.end());
  Eigen::MatrixXd matForBond = Configuration::genMatForBond(hProdMat.rows(), 
      bondIndsVec, cosh2alpha, sinh2alpha); //TODO: Optimize by only considering that you are multiplying by an identity with a 2 by 2 block changed.
  hProdMat = matForBond * hProdMat;
}

Eigen::MatrixXd Configuration::genMatForBond(int nDims, 
    const std::vector<int>& bondIndsVec, double cosh2alpha, 
    double sinh2alpha) const {
  Eigen::MatrixXd mat = Eigen::MatrixXd::Identity(nDims, nDims);
  mat(bondIndsVec[0], bondIndsVec[0]) = cosh2alpha;
  mat(bondIndsVec[1], bondIndsVec[1]) = cosh2alpha;
  mat(bondIndsVec[0], bondIndsVec[1]) = sinh2alpha;
  mat(bondIndsVec[1], bondIndsVec[0]) = sinh2alpha;
  return mat;
}

bool Configuration::operator==(const Configuration& other) const {
  if (this->getNumBonds() != other.getNumBonds()) {
    return false;
  }
  for (const auto& pair : bonds) {
    double truncTol = pair.first;
    if (this->getBond(truncateToTolerance(truncTol)) 
          != other.getBond(truncateToTolerance(truncTol))) {      
      return false;
    }
  }
  return true;
}

bool Configuration::operator!=(const Configuration& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Configuration& configuration) {  
  os << "[[CONFIGURATION]]" << std::endl;
  os << "[TOLERANCE]\n" << configuration.getTolerance() << std::endl;
  os << "[BETA]\n" << configuration.getBeta() << std::endl;
  
  std::vector<double> tauGroupStarts = configuration.getTauGroupStarts();
  os << "[TAU_GROUP_STARTS]" << std::endl;
  for (int i = 0; i < tauGroupStarts.size(); i++) {
    if (i % 5 == 0 && i != 0) {
      os << std::endl;
    }
    os << tauGroupStarts[i] << " ";
  }
  os << std::endl; //TODO: HANDLE WHEN THIS OUTPUTS A BLANK LINE

  std::map<int, int> bondsPerType = configuration.getBondsPerType();
  os << "[BONDS_PER_TYPE]" << std::endl;
  if (bondsPerType.size() == 0) {
    os << 1 << " " << 0 << std::endl;
  }
  for (const auto& [key, value] : bondsPerType) {
    os << key << " " << value << std::endl;
  }
  
  std::set<std::pair<double, int>> taus = configuration.getTaus();
  os << "[TAUS]" << std::endl;
  int numTaus = taus.size();
  for (const auto& tau : taus) {
    os << tau.first << " " << tau.second << std::endl;
  }

  std::map<double, Bond> bonds = configuration.getBonds();
  os << "[BONDS]" << std::endl;
  int numBonds = configuration.getNumBonds();
  int i = 0;
  for (const auto& [key, value] : bonds) {
    os << key << std::endl << value;
    if (i != numBonds - 1) os << std::endl; //TODO: MIGHT NOT NEED THIS IF IT'S ADDING A BLANK LINE
    i++;
  }
  
  return os;
}

std::istream& operator>>(std::istream& is, Configuration& configuration) {
  std::streampos pos = is.tellg();
  std::string line;
  while (std::getline(is, line)) { 
    size_t start = line.find("["); size_t end = line.find("]");
    if (start == std::string::npos || end == std::string::npos) {
      continue;
    }
    std::string title = line.substr(start + 1, end - (start + 1));
    if (title == "TOLERANCE") {
      if (std::getline(is, line)) {
        configuration.setTolerance(std::stod(line));
      }
    } else if (title == "BETA") {
      if (std::getline(is, line)) {
        configuration.setBeta(std::stod(line));
      }
    } else {
      while (true) {
        pos = is.tellg();
        if (!std::getline(is, line)) {
          return is;
        }
        if (line.find("[") == std::string::npos) {
          std::stringstream ss(line);

          
          if (title == "TAU_GROUP_STARTS") {
            double num;
            while (ss >> num) {
              configuration.tauGroupStarts.push_back(
                  configuration.truncateToTolerance(num));
            }
          } else if (title == "BONDS_PER_TYPE") {
            int key; int val;
            ss >> key; ss >> val;
            configuration.bondsPerType[key] = val;
          } else if (title == "TAUS") {
            int timeGroupInd; double tauVal;
            ss >> tauVal; ss >> timeGroupInd;
            tauVal = configuration.truncateToTolerance(tauVal);
            configuration.taus.insert(std::pair<double, int>(tauVal, timeGroupInd));

          } else if (title == "BONDS") {
            double tau;
            ss >> tau;
            tau = configuration.truncateToTolerance(tau);

            if (!std::getline(is, line)) {
              return is;
            }

            ss.clear();
            ss.str(line);

            int cs[6];
            for (int i = 0; i < 6; i++) {
              if (!(ss >> cs[i])) {
                throw std::runtime_error("Configuration>>: Invalid bond "
                  "coordinate format.");
              }
            }

            const LatticeBase& lattice = getLattice();
            const SiteBase* siteA = &lattice.getSite(cs[0], cs[1], cs[2]);
            const SiteBase* siteB = &lattice.getSite(cs[3], cs[4], cs[5]);

            std::set<const SiteBase*, Bond::SiteSumLess> sites = {siteA, siteB};

            configuration.bonds.insert({tau, Bond(inds)});
          } else {
            return is;
          }
        } else {
          is.seekg(pos);
          break;
        }
      }
    } 
  }

  return is;
}


double Configuration::truncateToTolerance(double key) const {
  return std::round(key / tolerance) * tolerance;
}