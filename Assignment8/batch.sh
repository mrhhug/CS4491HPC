#!/bin/bash

#SBATCH -J assn8       # Job name
#SBATCH -o assn8.o%j   # Name of stdout output file(%j expands to jobId)
#SBATCH -e assn8.o%j   # Name of stderr output file(%j expands to jobId)
#SBATCH -p normal           # Submit to the 'normal' or 'development' queue
#SBATCH -N 128                # Total number of nodes requested (16 cores/node)
#SBATCH -n 2048               # Total number of mpi tasks requested
#SBATCH -t 01:00:00         # Run time (hh:mm:ss) - 1.5 hours

#SBATCH -A TG-ASC140030 	# <-- Allocation name to charge job against!

# Run the OpenMP application
thr=("4" "8" "16")
tos=("1600000" "16000000" "160000000" "1600000000")
count=0
echo "_____________________________________________________________"
printf "|      Tosses |%10i| %10i| %10i| %10i|\n" ${tos[0]} ${tos[1]} ${tos[2]} ${tos[3]}
echo "|------------------------------------------------------------|"
for THREADS in "${thr[@]}"
do
	printf "|Threads: %2i |" ${thr[count]}
	count=$((count + 1))
	for TOSSES in "${tos[@]}"
	do 
		./a.out "$THREADS" "$TOSSES"
		printf "|"
	done
	echo ""
done
echo "_____________________________________________________________":q
