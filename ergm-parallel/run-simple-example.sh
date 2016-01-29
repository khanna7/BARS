#!/bin/sh

#SBATCH --ntasks=10
#SBATCH --time=60
#SBATCH --mem-per-cpu=10000

# Always use -n 1 for the Rmpi package. It spawns additional processes dynamically
mpirun -n 1 Rscript simple-ergm-fit-example.R
