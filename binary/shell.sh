#!/bin/bash

path=../logfile/binary_log

echo "" > $path
for i in 10000 100000 1000000 10000000; do
	echo "Sample Num : $i" >> $path
	./test 256 $i >> $path
	echo "" >> $path
done
