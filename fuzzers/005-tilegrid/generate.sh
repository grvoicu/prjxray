#!/bin/bash -x
# Copyright (C) 2017-2020  The Project X-Ray Authors.
#
# Use of this source code is governed by a ISC-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/ISC
#
# SPDX-License-Identifier: ISC

PRJ=$2

export FUZDIR=$PWD
source ${XRAY_GENHEADER}

if [ $XRAY_DATABASE == "spartan3" ]; then
    #TODO: Generate tiles.txt from xdl file (with xdl.exe -report). It was generated from RapidSmith.
	cp ../../spartan3_tiles.txt tiles.txt
    #TODO: Auto-generate pin_func.txt. Currently it is empty.
    cp ../../spartan3_pin_func.txt pin_func.txt
else
    ${XRAY_VIVADO} -mode batch -source $FUZDIR/generate_$PRJ.tcl
    test -z "$(fgrep CRITICAL vivado.log)"
fi

if [ $PRJ != "tiles" ] ; then
    for x in design*.bit; do
	    ${XRAY_BITREAD} -F $XRAY_ROI_FRAMES -o ${x}s -z -y $x
    done

    for x in design_*.bits; do
	    diff -u design.bits $x | grep '^[-+]bit' > ${x%.bits}.delta
    done
    touch deltas
fi

