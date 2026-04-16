#!/usr/bin/env bash

base_dir=$(pwd)
build_dir="/home/buckj/fbHLFT/build/src/main"



plot_path="/home/buck_j/wf/huffman/runs/test_io/plot.py"
ANALYSIS_PATH="/home/buck_j/DATA"




# Define BC types
bc_types=("open" "periodic")
bc_types_abb=("O" "P")

# Define values of beta to study
betas=("0.1" "1" "10")

# Define the lattice sizes for each dimensionality
oneD_sizes=("5" "10" "50")
twoD_sizes=("2" "4" "8")
threeD_sizes=("2" "4")

# Define the template for the input file
INPUT_TEMPLATE="$(cat <<'EOF'
control:
  equil_sweeps_patience: 3
  equil_sweeps_tol: 0.1
  scale_num_updates: 1
  max_sweeps: 4000
  num_time_groups: 5
  random_seed: 1

output:
  out_sweeps_patience: 10
  out_dir: DATA
  write_bonds_per_type: true
  restarts: true

lattice:
  type: LATTYPE
  dims: DIMS
  a: 3.77945                  # in Bohr radii. 2.00 A.
  b: 3.77945
  c: 3.77945
  alpha: 1.57079632679        # Input in radians
  beta: 1.57079632679
  gamma: 1.57079632679

  lims:
    x:
      bc_type: BCX           # Options are periodic and open
      min: 0.00
      nsites: XNSITES
COMMENT1D    y:
COMMENT1D      bc_type: BCY
COMMENT1D      min: 0.00
COMMENT1D      nsites: YNSITES
COMMENT1D  COMMENT2D  z:
COMMENT1D  COMMENT2D    bc_type: BCZ
COMMENT1D  COMMENT2D    min: 0.00
COMMENT1D  COMMENT2D    nsites: ZNSITES

hamiltonian:
  model: random
  accept_prob: 0.5     #TODO: REMOVE ACCEPT_PROB
  insert_prob: 0.5
  bond_type_props:
    1: 0
    2: 1
    3: 0
    4: 0
    5: 0

configuration:
  float_tol: 1e-5
  beta: BETA
EOF
)"

# Define the slurm submission script
SLURM_TEMPLATE="$(cat <<'EOF'
#!/usr/bin/env bash
#SBATCH --job-name NAME
#SBATCH --output=slurm.out
#SBATCH --partition=small
#SBATCH --ntasks=1
#SBATCH --time=23:00:00
#SBATCH --mem=2G

./main input.yaml > output.txt
rm main
EOF
)"


# Perform HC simulations
mkdir -p HC
cd HC
echo "Starting Honeycomb simulations..."

for beta in "${betas[@]}"; do
  mkdir -p "beta${beta}"
  cd "beta${beta}"
  echo "  beta=${beta}"

  for bcx in "${bc_types[@]}"; do
  for bcy in "${bc_types[@]}"; do

#TODO: CHECK FOR DUPLICATE BCS

    mkdir -p "bc${bcx:0:1}${bcy:0:1}"
    cd "bc${bcx:0:1}${bcy:0:1}"
    echo "    bc=${bcx:0:1}${bcy:0:1}"

    for size in "${twoD_sizes[@]}"; do
      mkdir -p "size${size}${size}"
      cd "size${size}${size}"
      echo "      size=${size}${size}"

      INPUT_DECK="${INPUT_TEMPLATE//COMMENT1D/}"
      INPUT_DECK="${INPUT_DECK//COMMENT2D/#}"
      INPUT_DECK="${INPUT_DECK//DIMS/2}"
      INPUT_DECK="${INPUT_DECK//BETA/$beta}"
      INPUT_DECK="${INPUT_DECK//BCX/$bcx}"
      INPUT_DECK="${INPUT_DECK//BCY/$bcy}"
      INPUT_DECK="${INPUT_DECK//XNSITES/$size}"
      INPUT_DECK="${INPUT_DECK//YNSITES/$size}"
      INPUT_DECK="${INPUT_DECK//LATTYPE/honeycomb}"

      printf '%s\n' "$INPUT_DECK" > input.yaml
      cp "$build_dir" .

      SLURM_SCRIPT="${SLURM_TEMPLATE//NAME/bc${bcx:0:1}${bcy:0:1}b${beta}s${size}}-2D"
      printf '%s\n' "$SLURM_SCRIPT" > submit.slurm
      sbatch submit.slurm
      #./main input.yaml > output.out
      #python3 $plot_path DATA
      #rm main

      cd ..
    done
    cd ..
  done
  done
  cd ..
done
cd ..
cd "$base_dir"

# Perform 1D simulations
mkdir -p oneD
cd oneD
echo "Starting oneD simulations..."

for beta in "${betas[@]}"; do
  mkdir -p "beta${beta}"
  cd "beta${beta}"
  echo "  beta=${beta}"

  for bc in "${bc_types[@]}"; do
    mkdir -p "bc${bc:0:1}"
    cd "bc${bc:0:1}"
    echo "    bc=${bc:0:1}"

    for size in "${oneD_sizes[@]}"; do
      mkdir -p "size${size}"
      cd "size${size}"
      echo "      size=${size}"

      INPUT_DECK="${INPUT_TEMPLATE//COMMENT1D/#}"
      INPUT_DECK="${INPUT_DECK//DIMS/1}"
      INPUT_DECK="${INPUT_DECK//BETA/$beta}"
      INPUT_DECK="${INPUT_DECK//BCX/$bc}"
      INPUT_DECK="${INPUT_DECK//XNSITES/$size}"
      INPUT_DECK="${INPUT_DECK//LATTYPE/simple-cubic}"
      
      printf '%s\n' "$INPUT_DECK" > input.yaml
      cp "$build_dir" .

      SLURM_SCRIPT="${SLURM_TEMPLATE//NAME/bc${bc:0:1}b${beta}s${size}-1D}"
      printf '%s\n' "$SLURM_SCRIPT" > submit.slurm
      sbatch submit.slurm
      #./main input.yaml > output.out
      #python3 $plot_path DATA
      #rm main

      cd ..
    done
    cd ..
  done
  cd ..
done
cd ..
cd "$base_dir"


# Perform 2D simulations
mkdir -p twoD
cd twoD
echo "Starting twoD simulations..."

for beta in "${betas[@]}"; do
  mkdir -p "beta${beta}"
  cd "beta${beta}"
  echo "  beta=${beta}"

  for bcx in "${bc_types[@]}"; do
  for bcy in "${bc_types[@]}"; do

#TODO: CHECK FOR DUPLICATE BCS

    mkdir -p "bc${bcx:0:1}${bcy:0:1}"
    cd "bc${bcx:0:1}${bcy:0:1}"
    echo "    bc=${bcx:0:1}${bcy:0:1}"

    for size in "${twoD_sizes[@]}"; do
      mkdir -p "size${size}${size}"
      cd "size${size}${size}"
      echo "      size=${size}${size}"

      INPUT_DECK="${INPUT_TEMPLATE//COMMENT1D/}"
      INPUT_DECK="${INPUT_DECK//COMMENT2D/#}"
      INPUT_DECK="${INPUT_DECK//DIMS/2}"
      INPUT_DECK="${INPUT_DECK//BETA/$beta}"
      INPUT_DECK="${INPUT_DECK//BCX/$bcx}"
      INPUT_DECK="${INPUT_DECK//BCY/$bcy}"
      INPUT_DECK="${INPUT_DECK//XNSITES/$size}"
      INPUT_DECK="${INPUT_DECK//YNSITES/$size}"
      INPUT_DECK="${INPUT_DECK//LATTYPE/simple-cubic}"

      printf '%s\n' "$INPUT_DECK" > input.yaml
      cp "$build_dir" .

      SLURM_SCRIPT="${SLURM_TEMPLATE//NAME/bc${bcx:0:1}${bcy:0:1}b${beta}s${size}}-2D"
      printf '%s\n' "$SLURM_SCRIPT" > submit.slurm
      sbatch submit.slurm
      #./main input.yaml > output.out
      #python3 $plot_path DATA
      #rm main

      cd ..
    done
    cd ..
  done
  done
  cd ..
done
cd ..
cd "$base_dir"



# Perform 3D simulations
mkdir -p threeD
cd threeD
echo "Starting threeD simulations..."

for beta in "${betas[@]}"; do
  mkdir -p "beta${beta}"
  cd "beta${beta}"
  echo "  beta=${beta}"

  for bcx in "${bc_types[@]}"; do
  for bcy in "${bc_types[@]}"; do
  for bcz in "${bc_types[@]}"; do
	

#TODO: CHECK FOR DUPLICATE BCS


    mkdir -p "bc${bcx:0:1}${bcy:0:1}${bcz:0:1}"
    cd "bc${bcx:0:1}${bcy:0:1}${bcz:0:1}"
    echo "    bc=${bcx:0:1}${bcy:0:1}${bcz:0:1}"

    for size in "${threeD_sizes[@]}"; do
      mkdir -p "size${size}${size}${size}"
      cd "size${size}${size}${size}"
      echo "      size=${size}${size}${size}"

      INPUT_DECK="${INPUT_TEMPLATE//COMMENT1D/}"
      INPUT_DECK="${INPUT_DECK//COMMENT2D/}"
      INPUT_DECK="${INPUT_DECK//DIMS/3}"
      INPUT_DECK="${INPUT_DECK//BETA/$beta}"
      INPUT_DECK="${INPUT_DECK//BCX/$bcx}"
      INPUT_DECK="${INPUT_DECK//BCY/$bcy}"
      INPUT_DECK="${INPUT_DECK//BCZ/$bcz}"
      INPUT_DECK="${INPUT_DECK//XNSITES/$size}"
      INPUT_DECK="${INPUT_DECK//YNSITES/$size}"
      INPUT_DECK="${INPUT_DECK//ZNSITES/$size}"
      INPUT_DECK="${INPUT_DECK//LATTYPE/simple-cubic}"

      printf '%s\n' "$INPUT_DECK" > input.yaml
      cp "$build_dir" .

      SLURM_SCRIPT="${SLURM_TEMPLATE//NAME/bc${bcx:0:1}${bcy:0:1}${bcz:0:1}b${beta}s${size}-3D}"
      printf '%s\n' "$SLURM_SCRIPT" > submit.slurm
      sbatch submit.slurm
      #./main input.yaml > output.out
      #python3 $plot_path DATA
      #rm main

      cd ..
    done
    cd ..
  done
  done
  done
  cd ..
done
cd ..
cd "$base_dir"








## Remove previous data
#rm -r $out_dir
#rm main

## Build the code
#cd $build_dir
#make -j
#cd $run_dir

## Copy the code executable to the run directory
#cp "$build_dir/src/main" .

## Run the code
#./main INPUT > output.txt

