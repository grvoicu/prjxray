#!/bin/bash

. ../../utils/genheader.sh

echo '`define SEED 32'"'h$(echo $1 | md5sum | cut -c1-8)" > setseed.vh

vivado -mode batch -source ../generate.tcl

for i in {0..9}; do
	../../../tools/bitread -F $XRAY_ROI_FRAMES -o design_$i.bits -z -y design_$i.bit
done

for i in {0..9}; do
	python3 ../generate.py $i
done
