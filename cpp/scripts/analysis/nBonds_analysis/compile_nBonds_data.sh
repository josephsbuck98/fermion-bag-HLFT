#!/bin/bash

base_path="$(pwd)"
if [[ -f "$base_path/agg.dat" ]]; then
    rm "$base_path/agg.dat"
fi

plot_path="/home/buck_j/wf/huffman/fermion-bag-HLFT/cpp/scripts/analysis/nBonds_analysis/analyze_nBonds.py"

# Ensure plot_path is set
if [[ -z "$plot_path" ]]; then
    echo "Error: plot_path is not set."
    exit 1
fi

# Set or import the data directory names
if [ "$#" -lt 1 ]; then
   echo "Data directory name not supplied. Assuming \"DATA/\""
   data_dirname="DATA"
fi
data_dirname="$1"


# Function to recursively walk directories
walk_dirs() {
    local current_dir="$1"

    # Check if DATA directory exists in current directory
    if [[ -d "$current_dir/$data_dirname" ]]; then
        echo "Found $data_dirname in: $current_dir"
        (cd "$current_dir" && python3 "$plot_path" -o $data_dirname)
        #TODO: Copy data in sub-agg.dat files to root agg.dat file.
    fi

    # Loop through subdirectories
    for dir in "$current_dir"/*/; do
        [[ -d "$dir" ]] || continue

        # Skip DATA directories
        if [[ "$(basename "$dir")" == "DATA" ]]; then
            continue
        fi

        walk_dirs "$dir"
    done
}

# Start recursion from current directory
walk_dirs "$(pwd)"
