#!/bin/bash

# LOG MESSAGES TO A RUN DIRECTORY LOG

# Specify your personal file paths here
export CPP_ROOT="/home/buck_j/dev/fermion-bag-HLFT/cpp"
export RUN_DIR="."

# Build cpp program

# Create folders in run directory for each combo of params

# Create and copy input files into each run directory. Loop over arrays

# Copy executable into each run directory

# Run each combo of params 

echo "Root of CPP code is $CPP_ROOT"
echo "Run directory path is $RUN_DIR"

echo "Generating input deck..." >> log

echo "Finished generating input deck..." >> log

# Run the binary 
"$CPP_ROOT/build/main"
