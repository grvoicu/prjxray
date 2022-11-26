# Copyright (C) 2017-2020  The Project X-Ray Authors.
#
# Use of this source code is governed by a ISC-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/ISC
#
# SPDX-License-Identifier: ISC
N ?= 30
ifeq (${XRAY_ARCH}, Series7)
GENERATE_ARGS?="--oneval 1 --design params.csv --dframe 26 --dword 1"
else
DFRAME = 0
DWORD = 0
endif

include ../fuzzaddr/common.mk
