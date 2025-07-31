#!/bin/bash

export STUDY_ROOT=$(pwd)
export CPP_ROOT="/home/buck_j/dev/fermion-bag-HLFT/cpp"
export BUILD_ROOT="${CPP_ROOT}/build"

# Build the code
cd $BUILD_ROOT && cmake .. && make -j && cd $STUDY_ROOT

# Copy the executable into the study directory
if [ -f "${BUILD_ROOT}/src/main" ]; then
 cp ${BUILD_ROOT}/src/main ${STUDY_ROOT}/main # Copy into each subdir
 #TODO: else statement builds code or cancels execution???
fi

echo "Generating input deck..."
echo "Finished generating input deck..."
