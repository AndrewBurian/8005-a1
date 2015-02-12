#!/bin/bash

# ensure we have the right number of args
if [ $# -ne 4 ]
then
	echo "Usage: $0 <(variable)[ops|workers]> <constant> <var_min> <var_max>"
	echo "Ex:"
	echo "$0 workers 5 1 100       Executes 5 ops on a variable number of workers from 1 to 100"
	echo "$0 ops 5 100 500         Executes 100 to 500 ops on 5 workers"
	exit	
fi

# make the csv file
# don't overwrite existing files
filename="performance.csv"
if [ -e $filename ]
then
	i=1
	while [ -e performance_$i.csv ]
	do
		i=$(($i + 1))
	done
	filename="performance_$i.csv"
fi

echo "Writing file: $filename"
touch $filename


## Variable ops
##############
if [ $1 == "ops" ]
then

	echo "Speed with $2 workers" >> $filename
	echo "ops,threads,processes" >> $filename

	# execute time in the loop
	for i in $(seq $3 $4)
	do
		echo -n "$i," >> $filename
		echo -n `/usr/bin/time -f %E ../bin/a1 -t -w $2 -c $i 2>&1` >> $filename
		echo -n "," >> $filename
		echo -n `/usr/bin/time -f %E ../bin/a1 -p -w $2 -c $i 2>&1` >> $filename
		echo "" >> $filename
	done
fi

## Variable Workers
##############

if [ $1 == "workers" ]
then
	
	echo "Speed with $2 operations" >> $filename
	echo "workers,threads,processes" >> $filename

	# execute time in the loop
	for i in $(seq $3 $4)
	do
		echo -n "$i," >> $filename
		echo -n `/usr/bin/time -f %E ../bin/a1 -t -w $i -c $2 2>&1` >> $filename
		echo -n "," >> $filename
		echo -n `/usr/bin/time -f %E ../bin/a1 -p -w $i -c $2 2>&1` >> $filename
		echo "" >> $filename
	done
fi

# cleanup
rm worker*
