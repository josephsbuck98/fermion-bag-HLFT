import numpy as np
import pandas as pd
import math
import matplotlib.pyplot as plt
import yaml
import os
import sys
plt.rcParams["font.family"] = "serif"


def plot_sweep_series(df):
    plt.figure()

    plt.plot(df["numInserts"], label="numInserts")
    plt.plot(df["numRemoves"], label="numRemoves")
    plt.plot(df["numRejects"], label="numRejects")
    plt.plot(df["numBonds"], label="numBonds")

    plt.xlabel("Sweep Number")

    plt.legend()
    plt.grid()


def plot_nBonds_hist(df, parsed_INPUT, plot_analytical, outdir_name):
    plt.figure()

    hist_range = get_hist_range(df)

    counts, _, _ = plt.hist(df["numBonds"], bins=40, range=hist_range, density=True)

    plt.xlabel("Number of Bonds")
    plt.ylabel("Proportion of Configurations")

    plt.grid()

    if plot_analytical: 
        analytical_dist, overflow, mean_nbonds_thr = get_analytical_dist(parsed_INPUT, outdir_name, hist_range[1])
        if overflow:
            fact = np.amax(counts) / np.amax(analytical_dist)
            analytical_dist *= fact
        plt.plot(analytical_dist)
    plt.xlim(hist_range[0], hist_range[1])

    return mean_nbonds_thr


def get_hist_range(df):
    num_bonds = df["numBonds"]
    num_bonds_mean = np.mean(num_bonds)
    num_bonds_std_dev = np.std(num_bonds)
    return (num_bonds_mean - 3 * num_bonds_std_dev, num_bonds_mean + 3 * num_bonds_std_dev)


def get_analytical_dist(parsed_INPUT, outdir_name, upper):
    beta = parsed_INPUT["beta"]
    num_unique_bonds = get_num_unique_bonds(parsed_INPUT)
    
    with np.errstate(over='raise'):
        try:
            curr_val = 1 / np.exp(beta * num_unique_bonds)
            overflow = False
        except FloatingPointError:
            curr_val = 1e-323
            overflow = True

    analytical_dist = [curr_val]
    for k in range(1, math.floor(upper)):
        curr_val *= beta * num_unique_bonds / k
        analytical_dist.append(curr_val)

    mean_nbonds_thr = beta * num_unique_bonds

    return np.array(analytical_dist), overflow, mean_nbonds_thr


def parse_INPUT(INPUT_path):
    with open(INPUT_path, "r") as f:
        data = yaml.safe_load(f)
    parsed_INPUT = dict()
    parsed_INPUT["beta"] = data["configuration"]["beta"]
    parsed_INPUT["dims"] = data["lattice"]["dims"]
    lat_lims = data["lattice"]["lims"]
    parsed_INPUT["xNSites"] = lat_lims["x"]["nsites"]
    parsed_INPUT["xBCType"] = lat_lims["x"]["bc_type"]
    try:
        parsed_INPUT["yNSites"] = lat_lims["y"]["nsites"]
        parsed_INPUT["yBCType"] = lat_lims["y"]["bc_type"]
    except KeyError:
        parsed_INPUT["yNSites"] = 1
        parsed_INPUT["yBCType"] = "open"
    try:
        parsed_INPUT["zNSites"] = lat_lims["z"]["nsites"]
        parsed_INPUT["zBCType"] = lat_lims["z"]["bc_type"]
    except:
        parsed_INPUT["zNSites"] = 1
        parsed_INPUT["zBCType"] = "open"
    parsed_INPUT["latticeType"] = data["lattice"]["type"]
    hamil = data["hamiltonian"]
    parsed_INPUT["hamilType"] = hamil["model"]
    parsed_INPUT["t"] = hamil["t"] if hamil["t"] else 0
    parsed_INPUT["V"] = hamil["V"] if hamil["V"] else 0
    return parsed_INPUT


def get_num_unique_bonds(data):
    if data["latticeType"] == "honeycomb":
        print("HONEYCOMB LATTICE")
        num_cells = data["xNSites"] * data["yNSites"]
        num_bonds = num_cells * 3
        if data["xBCType"] == "open":
            num_bonds -= data["xNSites"] * 2
        if data["yBCType"] == "open":
            num_bonds -= data["yNSites"] * 2
        return num_bonds

    xNSites, yNSites, zNSites = data["xNSites"], 1, 1
    xBCType, yBCType, zBCType = data["xBCType"], "open", "open"
    
    num_sites = get_num_sites(data)

    num_bonds = num_sites
    dims = 1
    if data["dims"] != 1:
        yNSites = data["yNSites"]
        yBCType = data["yBCType"]
        if data["dims"] == 3:
            num_bonds *= 3
            dims = 3
            zNSites = data["zNSites"]
            zBCType = data["zBCType"]
        else:
            num_bonds *= 2
            dims = 2

    if data["xBCType"] == "open":
        num_bonds -= ((2 - 1) * yNSites * zNSites)
    if dims != 1:
        if yBCType == "open":
            num_bonds -= ((2 - 1) * xNSites * zNSites)
        if dims == 3:
            if zBCType == "open":
                num_bonds -= ((2 - 1) * xNSites * yNSites)

    return num_bonds


def filter_warmup(df):
    data = df['numBonds'].values
    N = len(data)

    tolerance = 0.01
    min_points = 10
    
    # Compute cumulative mean
    cumsum = np.cumsum(data)
    cumu_mean = cumsum / (np.arange(N) + 1)
    
    # Final mean (assumes the last part is stationary)
    final_mean = cumu_mean[-1]
    
    # Stationarity: where cumulative mean is within tolerance of final mean
    within_tolerance = np.abs(cumu_mean - final_mean) <= tolerance * final_mean
    
    # Ensure minimum points are considered
    candidate_indices = np.where(within_tolerance)[0]
    burn_index = candidate_indices[candidate_indices >= min_points][0]
    
    df_filtered = df.iloc[burn_index:].copy()
    
    return df_filtered, burn_index


def get_num_sites(data):
    num_sites = data["xNSites"]
    if data["dims"] > 1:
        num_sites *= data["yNSites"]
    if data["dims"] > 2:
        num_sites *= data["zNSites"]
    return num_sites
    


def plot_other_data_hist(df):
    plt.figure()

    bins = np.linspace(min(df["numInserts"].min(), df["numRemoves"].min()),
                   max(df["numInserts"].max(), df["numRemoves"].max()), 30)

    plt.hist(df["numInserts"], label="numInserts", alpha=0.8, bins=30)
    plt.hist(df["numRemoves"], label="numRemoves", histtype='step', bins=30)
    plt.hist(df["numRejects"], label="numRejects", alpha=0.8, bins=25)

    plt.xlabel("Number of Inserts/Removes")
    plt.ylabel("Number of Sweeps")

    plt.legend()
    plt.grid()


def main():
    if len(sys.argv) < 3 or len(sys.argv[1]) == 0:
        print(f"Usage: python3 analyzie_nBonds.py -o <outdir name> [-display]")
        return
    outdir_name = "DATA"
    display_plots = False
    for i in range(1, len(sys.argv)):
        if (sys.argv[i] == "-o"):
            i += 1
            outdir_name = sys.argv[i]
        if (sys.argv[i] == "-display"):
            display_plots = True
    if (outdir_name[-1] != "/"): outdir_name += "/"
    if not os.path.exists(outdir_name) or \
            not os.path.exists(outdir_name + "sweeps.dat"):
        print(f"Data does not exist in {outdir_name}/sweeps.dat.")
    
    # Read in data and filter out equilibration period
    df = pd.read_csv(outdir_name + "sweeps.dat", sep="\s+", comment="-")
    df = df.set_index("sweepID")
    df, ind = filter_warmup(df)
    print(f"Equil Ind: {ind}")

    # Parse input file
    INPUT_path = os.getcwd() + "/" + outdir_name + "/INPUT"
    if not os.path.exists(INPUT_path):
        print(f"Path {INPUT_path} does not exist.")
        sys.exit(1)
    parsed_INPUT = parse_INPUT(INPUT_path)

    # Plot sweep series data
    plot_sweep_series(df)

    # Plot number of bonds histogram
    mean_nbonds_thr = plot_nBonds_hist(df, parsed_INPUT, True, outdir_name)

    # Plot number of inserts/removes histogram
    plot_other_data_hist(df)

    # Compile data
    mean_nbonds = np.mean(df['numBonds'])
    sd_nbonds = np.std(df['numBonds'])
    zscore = sd_nbonds / np.sqrt(df.shape[0])

    # Write data
    with open(outdir_name + "agg.dat", "w") as f:
        f.write(f"hamil_type: {parsed_INPUT['hamilType']}\n")
        f.write(f"t: {parsed_INPUT['t']}\n")
        f.write(f"V: {parsed_INPUT['V']}\n")
        f.write(f"latt_type: {parsed_INPUT['latticeType']}\n")
        f.write(f"xNSites: {parsed_INPUT['xNSites']}\n")
        f.write(f"yNSites: {parsed_INPUT['yNSites']}\n")
        f.write(f"zNSites: {parsed_INPUT['zNSites']}\n")
        f.write(f"beta: {parsed_INPUT['beta']}\n")
        f.write(f"mean_nbonds_thr: {mean_nbonds_thr}\n")
        f.write(f"mean_nbonds: {mean_nbonds}\n")
        f.write(f"sd_nbonds: {sd_nbonds}\n")
        f.write(f"std_err: {(mean_nbonds_thr-mean_nbonds)/sd_nbonds}\n")
    print(f"Wrote data to {outdir_name}agg.dat")

    # Display data
    if (display_plots):
        plt.show()



if __name__ == "__main__":
    main()
