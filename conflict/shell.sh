#!/bin/bash
path="log"
sample=1000000
echo "" > $path

for i in {200000..2000000..200000}; do
	case=`expr "$i" "/" "100000"`
	echo "Start ${i} Buffer Test"
	echo 1 > /proc/sys/vm/drop_caches
	echo "Flush Cache"
	
	echo "Sample Num : ${case}" >> $path
	./test 256 $sample $i	>> $path
	echo "" >> $path
done
