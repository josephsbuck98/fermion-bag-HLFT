#include "TVModel.hpp"


TVModel::TVModel(InputParser::ParsedInput input) {

}


consts::BondActionType TVModel::applyUpdate(Configuration& configuration, 
    int groupNum, Configuration::RegionData regionData, 
    const LatticeBase* lattice) const {
  return consts::BondActionType::INSERTION;
}