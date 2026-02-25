analyze\_nBonds.py: Acts on a single simulation. Generates and optionally 
displays plots of sweep series data. Also generates histograms of the number of
configurations over nBonds, and outputs mean and std dev of data. Excludes 
"warm-up time" data from calculations.

compile\_nBonds\_data.sh: A common use case of this code is to run a parameter
sweep, such as with sweep\_params.sh. This creates a nested directory 
structure. This script runs analyze\_nBonds.py in each run directory, and 
appends aggregate data for each run in the root directory to an output file in
the root directory.

sweep\_params.sh: This script allows easy specification of parameters to vary,
and handles directory structure creation and job submission. This script takes
in a sweep.config file path. An example config file is stored at _____.
