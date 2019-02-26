#!/bin/bash
path="../logfile/hash_log"
echo "" > $path

for i in 10000 100000 1000000 10000000; do
	echo "Start ${i} Node Test"
	echo 1 > /proc/sys/vm/drop_caches
	echo "Flush Cache"
	
	echo "Sample Num : ${i}" >> $path
	./test 256 $i	>> $path
	echo "" >> $path
done
