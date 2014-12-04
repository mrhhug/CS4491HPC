#!/bin/bash

#SBATCH -J assn9       # Job name
#SBATCH -o assn9.o%j   # Name of stdout output file(%j expands to jobId)
#SBATCH -e assn9.o%j   # Name of stderr output file(%j expands to jobId)
#SBATCH -p development           # Submit to the 'normal' or 'development' queue
#SBATCH -N 1                # Total number of nodes requested (16 cores/node)
#SBATCH -n 1                # Total number of mpi tasks requested
#SBATCH -t1:00         # Run time (hh:mm:ss) - 1.5 hours

#SBATCH -A TG-ASC140030 	# <-- Allocation name to charge job against!
export PRINTPI="1"
time ibrun -np 16 a.out
