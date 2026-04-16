#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import argparse
import yaml
import sys

###############################################################################
# This script is for debugging a negative determinant issue in the tV Model. 
# The most likely source of the negative determinant is not a bug at all, but a
# result of the extremely ill-conditioned matrices that arise when you have 
# over 5 to 6 bonds in the configuration per pair of lattice points. For 
# example if you have a 2 by 2 isolated lattice, and the number of bonds in 
# your configuration reaches 5*2*2=20 or higher, the product of matrices will 
# be very ill conditioned and extremely sensitive to even machine precision 
# variations in the data stored in the matrix. This is true for the naive, 
# brute force method for calculating the weights which does not use a Green
# function.
###############################################################################


def read_input(data_dir):
  message = f"Reading dimensionality from {data_dir}INPUT"
  print("*" * len(message))
  print(message)
  print("*" * len(message))
  print("\n\n")

  with open(data_dir + "INPUT", "r") as f:
    in_params = yaml.safe_load(f)
  
  t = in_params["hamiltonian"]["t"]
  V = in_params["hamiltonian"]["V"]

  dims = {}
  if in_params["lattice"]["lims"].get("x"):
    dims["x"] = in_params["lattice"]["lims"]["x"]["nsites"]
  if in_params["lattice"]["lims"].get("y"):
    dims["y"] = in_params["lattice"]["lims"]["y"]["nsites"]
  if in_params["lattice"]["lims"].get("z"):
    dims["z"] = in_params["lattice"]["lims"]["z"]["nsites"]

  return dims, t, V


def read_config_from_RESTART(data_dir):
  message = f"Reading configuration from {data_dir}RESTART"
  print("*" * len(message))
  print(message)
  print("*" * len(message))
  print("\n\n")

  with open(data_dir + "RESTART") as f:
    lines = f.readlines()
  
  bond_data = {}
  for i, line in enumerate(lines):
    if line.strip() == "[BONDS]":
      for j in range(i + 1, len(lines) - 1, 2):
        try:
          tau = float(lines[j].strip())
          # Split the next line into a list of integers (or strings)
          digits = [int(d) for d in lines[j+1].split()]
          bond_data[tau] = digits
        except (ValueError, IndexError):
          # Stop if we hit a line that doesn't match the expected format
          break
      return bond_data
  
  return {}


def read_config_from_OUTPUTTXT():
  message = f"Reading configuration from output.txt"
  print("*" * len(message))
  print(message)
  print("*" * len(message))
  print("\n\n")

  with open("output.txt") as f:
    lines = f.readlines()
  
  bond_data = {}
  for i, line in enumerate(lines):
    if line.strip() == "[BONDS]":
      for j in range(i + 1, len(lines) - 1, 2):
        try:
          tau = float(lines[j].strip())
          # Split the next line into a list of integers (or strings)
          digits = [int(d) for d in lines[j+1].split()]
          bond_data[tau] = digits
        except (ValueError, IndexError):
          # Stop if we hit a line that doesn't match the expected format
          break
      return bond_data
  
  return {}


def read_detArg_from_OUTPUTTXT():
  message = f"Reading detArg from output.txt"
  print("*" * len(message))
  print(message)
  print("*" * len(message))
  print("\n\n")

  data = []
  capturing = False
  with open("output.txt", 'r') as f:
    for line in f:
      stripped = line.strip()

      # Start capturing after "detArg:"
      if stripped.startswith("detArg:"):
        capturing = True
        continue  # skip the "detArg:" line itself

      # Stop capturing when "tau:" is reached
      if stripped.startswith("tau:"):
        break

      if capturing and stripped:
        row = [float(x) for x in stripped.split()]
        data.append(row)
  return np.array(data)


def compute_cosh_sinh(t, V):
  cosh2alpha = (1 + (V / (2 * t)) * (V / (2 * t))) / (1 - (V / (2 * t)) * (V / (2 * t)))
  sinh2alpha = V / t / (1 - (V / (2 * t)) * (V / (2 * t)))
  return cosh2alpha, sinh2alpha


def map_bond_inds_to_mat_ind(xi, yi, zi, dims):
  return xi + dims.get("x", 0) * (yi + dims.get("y", 0) * zi)


def compute_submat(mat_dims, dims, cosh2alpha, sinh2alpha, bond):
  mat_ind_site_1 = map_bond_inds_to_mat_ind(bond[0], bond[1], bond[2], dims)
  mat_ind_site_2 = map_bond_inds_to_mat_ind(bond[3], bond[4], bond[5], dims)
  
  submat = np.eye(mat_dims)
  submat[mat_ind_site_1, mat_ind_site_1] = cosh2alpha
  submat[mat_ind_site_2, mat_ind_site_2] = cosh2alpha
  submat[mat_ind_site_1, mat_ind_site_2] = sinh2alpha
  submat[mat_ind_site_2, mat_ind_site_1] = sinh2alpha

  return submat


def compute_Hbond_mat(bonds, dims, t, V, extra_tau = -1, extra_bond = -1):
  cosh2alpha, sinh2alpha = compute_cosh_sinh(t, V)
  
  mat_dims = 0
  for key in dims.keys():
    if mat_dims == 0:
      mat_dims += dims[key]
      continue
    mat_dims *= dims[key]
  
  mat = np.eye(mat_dims)

  for tau, bond in sorted(bonds.items()):
    submat = compute_submat(mat_dims, dims, cosh2alpha, 
        sinh2alpha, bond)
    mat = np.dot(submat, mat)
  
  return mat
    

def compute_det(Hbond_mat):
  return np.linalg.det(np.eye(len(Hbond_mat[:, 0])) + Hbond_mat)


def main():
  parser = argparse.ArgumentParser(description="Process tau and bond together.")

  # All args are optional. If -dir is not supplied, "DATA/" is used.
  parser.add_argument("-tau", type=float, help="A single float value")
  parser.add_argument("-bond", type=int, nargs=6, help="Exactly 6 integers")
  parser.add_argument("-dir", type=str, help="The name of the data output "
    "directory. Should be in the directory where this script is being run.")
  parser.add_argument("-from_output", action="store_true", help="Use to read "
    "configuration from crashed output.txt instead of RESTART.")

  args = parser.parse_args()

  # If tau or bond is supplied, both must be supplied
  if (args.tau is None) != (args.bond is None):
    parser.error("If you provide -tau, you must also provide -bond (and vice versa).")
  if (args.dir[-1] != "/"): args.dir += "/"

  # Read in params from input file
  data_dir = args.dir if args.dir else "DATA/"
  dims, t, V = read_input(data_dir)



  # READ IN CONFIGURATIONS AND DETARG
  bonds_OUTPUT = read_config_from_OUTPUTTXT() # Read in configuration from output.txt
  bonds_RESTART = read_config_from_RESTART(args.dir) # Read in configuration from RESTART
  detArg = read_detArg_from_OUTPUTTXT() # Read in detArg from output.txt

  # Insert the extra bond into the RESTART bond
  bonds_OUTPUT[args.tau] = args.bond
  bonds_RESTART[args.tau] = args.bond

  # Compute Hbond matrices
  if args.tau and args.bond:
    Hbond_mat_OUTPUT = compute_Hbond_mat(bonds_OUTPUT, dims, t, V, 
      extra_tau = args.tau, extra_bond = args.bond)
    Hbond_mat_RESTART = compute_Hbond_mat(bonds_RESTART, dims, t, V, 
      extra_tau = args.tau, extra_bond = args.bond)



  
  # Perform element-wise comparison between each matrix
  print("Max proportional diff between OUTPUT arg and RESTART arg:")
  print(np.amax(np.divide(Hbond_mat_OUTPUT - Hbond_mat_RESTART, Hbond_mat_RESTART, where=Hbond_mat_RESTART != 0)))
  print(np.amin(np.divide(Hbond_mat_OUTPUT - Hbond_mat_RESTART, Hbond_mat_RESTART, where=Hbond_mat_RESTART != 0)))
  print("Max proportional diff between OUTPUT arg and detArg:")
  print(np.amax(np.divide(np.eye(len(Hbond_mat_OUTPUT[:, 0])) + Hbond_mat_OUTPUT - detArg, detArg, where=detArg != 0)))
  print(np.amin(np.divide(np.eye(len(Hbond_mat_OUTPUT[:, 0])) + Hbond_mat_OUTPUT - detArg, detArg, where=detArg != 0)))
  print("Max proportional diff between RESTART arg and detArg:")
  print(np.amax(np.divide(np.eye(len(Hbond_mat_OUTPUT[:, 0])) + Hbond_mat_RESTART - detArg, detArg, where=detArg != 0)))
  print(np.amin(np.divide(np.eye(len(Hbond_mat_OUTPUT[:, 0])) + Hbond_mat_RESTART - detArg, detArg, where=detArg != 0)))

    
  # Compute determinants
  det_OUTPUT = compute_det(Hbond_mat_OUTPUT)
  det_RESTART = compute_det(Hbond_mat_RESTART)
  det_detArg = np.linalg.det(detArg)
  print("Determinant from OUTPUT Configuration: ", det_OUTPUT)
  print("Determinant from RESTART Configuration: ", det_RESTART)
  print("Determinant from detArg: ", det_detArg)





if __name__ == "__main__":
  main()

