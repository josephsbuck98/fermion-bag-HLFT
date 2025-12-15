// Entry point for running simulations
#include <iostream>

#include "Configuration.hpp"
#include "Driver.hpp"
#include "Honeycomb.hpp"
#include "InputParser.hpp"
#include "Output.hpp"
#include "RandomHelpers.hpp"
#include "SimpleCubic.hpp"


#include <climits>


const double OMEGA = 4 / (4 * 1.3 - 1.3 * 1.3 * 1.3);
const double SINH2ALPHA = 4.0 * 1.0 * 1.3 / (4.0 * 1.0 * 1.0 - 1.3 * 1.3);
const double COSH2ALPHA = (4.0 * 1.0 * 1.0 + 1.3 * 1.3) / (4.0 * 1.0 * 1.0 - 1.3 * 1.3);


void testDet(int latSize, int nBonds, std::string filename, int callNum);
std::vector<Bond> createBondVector(LatticeBase* lattice, int nBonds);
Eigen::MatrixXd computeDetArg(int latSize, std::vector<Bond> activeBonds);
void printMat(Eigen::MatrixXd mat, std::ofstream& ofs);


int main(int argc, char* argv[]) {
  globalRNG(5);

  int latSize = 5;
  
  int nBonds = 100;
  
  std::string filename = "det.out";

  testDet(latSize, nBonds, filename, 0);

}

void testDet(int latSize, int nBonds, std::string filename, int callNum) {
  // Initialize the outfile
  fs::path filepath = fs::path(filename);
  if (fs::exists(filepath)) { // If filename exists, tack a 1 before the period
    if (callNum > 10) return;
    auto pos = filename.rfind('.');
    if (pos != std::string::npos) filename.insert(pos, "1");
    return testDet(latSize, nBonds, filename, callNum + 1);
  }
  std::ofstream ofs(filepath);
  if (!ofs) return;

  // Miscellaneous
  ofs << std::string(80, '~') << std::endl;
  ofs << "  -INT_MIN: " << INT_MIN << std::endl;
  ofs << "  -INT_MAX: " << INT_MAX << std::endl;

  // Computing interaction constants
  ofs << std::string(80, '~') << std::endl;
  ofs << "Interaction Constants: " << std::endl;
  ofs << "  -OMEGA: " << OMEGA << std::endl;
  ofs << "  -COSH2ALPHA: " << COSH2ALPHA << std::endl;
  ofs << "  -SINH2ALPHA: " << SINH2ALPHA << std::endl;

  // Create the lattice
  LatticeInput latIn{consts::LatticeType::SIMPLE_CUBIC, consts::DimsType::ONE,
      3.77945, 3.77945, 3.77945, 1.57079632679, 1.57079632679, 1.57079632679,
      consts::BoundType::PERIODIC, consts::BoundType::PERIODIC, consts::BoundType::PERIODIC,
      0.0, 0.0, 0.0, latSize, 1, 1
    };
  std::unique_ptr<LatticeBase> lattice_unqptr = std::make_unique<SimpleCubic>(latIn);
  LatticeBase* lattice = lattice_unqptr.get();

  ofs << std::string(80, '~') << std::endl;
  ofs << "Creating Lattice:" << std::endl;
  ofs << "  -Number of Sites: " << lattice->getNumSites(consts::DirsType::X) << std::endl;

  // Create the bond vector
  std::vector<Bond> bonds = createBondVector(lattice, nBonds);

  ofs << std::string(80, '~') << std::endl;
  ofs << "Creating Bond Vector:" << std::endl;
  ofs << "  -Number of Bonds: " << bonds.size() << std::endl;
  if (false) {
    for (int i = 0; i < nBonds; i++) {
      ofs << "Bond Number: " << i << std::endl << "  ";
      ofs << bonds[i] << std::endl;
    }
  }

  // Compute and report determinants
  ofs << std::string(80, '~') << std::endl;
  ofs << "Computing Determinants: " << std::endl;

  for (int numBondsInConfig = 1; numBondsInConfig <= nBonds; numBondsInConfig++) {
    ofs << "  -(Number of Bonds, Determinant): (" << numBondsInConfig << ", ";
    std::vector<Bond> activeBonds(bonds.begin(), bonds.begin() + numBondsInConfig);
    Eigen::MatrixXd detArg = computeDetArg(latSize, activeBonds);
    double det = detArg.determinant();
    ofs << det << ")" << std::endl;
    if (det < 0) {
      ofs << std::string(80, '~') << std::endl;
      ofs << "DETERMINANT IS NEGATIVE!!!!! Printing detArg and bonds..." << std::endl;
      printMat(detArg, ofs);
      break;
    }
  }
}

std::vector<Bond> createBondVector(LatticeBase* lattice, int nBonds) {
  int latticeBondStart;
  int latSize = lattice->getNumSites(consts::DirsType::X);
  
  std::vector<Bond> bonds;
  bonds.reserve(nBonds);
  for (int i = 0; i < nBonds; i++) {
    if (lattice->getBoundType(consts::DirsType::X) == consts::BoundType::OPEN) {
      latticeBondStart = chooseUnifRandIntWithBounds(0, latSize - 2 + 1);
    } else { // Periodic boundary
      latticeBondStart = chooseUnifRandIntWithBounds(0, latSize);
    }
    std::set<int> bondSites;
    for (int i = latticeBondStart; i < latticeBondStart + 2; i++) {
      bondSites.insert(i % latSize);
    }
    Bond newBond(bondSites);
    bonds.push_back(newBond);
  }
  return bonds;
}

Eigen::MatrixXd computeDetArg(int latSize, std::vector<Bond> activeBonds) {
  Eigen::MatrixXd bondProd = Eigen::MatrixXd::Identity(latSize, latSize);
  for (Bond& bond : activeBonds) {
    std::set<int> siteSet = bond.getIndices();
    std::vector<int> siteVect(siteSet.begin(), siteSet.end());

    Eigen::MatrixXd singleBondMat = Eigen::MatrixXd::Identity(latSize, latSize);
    singleBondMat(siteVect[0], siteVect[0]) = COSH2ALPHA;
    singleBondMat(siteVect[1], siteVect[1]) = COSH2ALPHA;
    singleBondMat(siteVect[0], siteVect[1]) = SINH2ALPHA;
    singleBondMat(siteVect[1], siteVect[0]) = SINH2ALPHA;

    bondProd = singleBondMat * bondProd;
  }
  return Eigen::MatrixXd::Identity(latSize, latSize) + bondProd;
}

void printMat(Eigen::MatrixXd mat, std::ofstream& ofs) {
  ofs << std::fixed << std::setprecision(1);
  for (int i = 0; i < mat.rows(); i++) {
    ofs << "  ";
    for (int j = 0; j < mat.cols(); j++) {
      ofs << mat(i, j) << " ";
    }
    ofs << std::endl;
  }
}