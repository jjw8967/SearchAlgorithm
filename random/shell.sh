#!/bin/bash

for i in 10000 100000 1000000 10000000; do
	./main 256 $i
done
