#include "TVModel.hpp"


TVModel::TVModel(InputParser::ParsedInput input) {

}


consts::BondActionType TVModel::applyUpdate(Configuration& configuration, const Lattice& lattice, 
    int groupLowerBound, int groupUpperBound, int groupNum, Configuration::RegionData regionData) const {
  return consts::BondActionType::INSERTION;
}