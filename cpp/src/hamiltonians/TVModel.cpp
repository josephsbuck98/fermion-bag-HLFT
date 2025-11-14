#include "TVModel.hpp"


TVModel::TVModel(InputParser::ParsedInput input) {

}


consts::BondActionType TVModel::applyUpdate(Configuration& configuration, 
    const Lattice& lattice, int groupNum, 
    Configuration::RegionData regionData, const LatticeBase* latticeNEW) const {
  return consts::BondActionType::INSERTION;
}