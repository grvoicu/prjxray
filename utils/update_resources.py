#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright (C) 2017-2021  The Project X-Ray Authors.
#
# Use of this source code is governed by a ISC-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/ISC
#
# SPDX-License-Identifier: ISC
import argparse
import yaml
import subprocess
import os
import re
import tempfile
import json
from prjxray.util import OpenSafeFile, db_root_arg, get_parts, set_part_resources


def main():
    """Tool to update the used resources by the fuzzers for each available part.

    Example:
        prjxray$ ./utils/update_resources.py artix7 --db-root database/artix7/
    """
    parser = argparse.ArgumentParser(
        description="Saves all resource information for a family.")

    parser.add_argument(
        'family',
        help="Name of the device family.",
        choices=['artix7', 'kintex7', 'zynq7', 'spartan7', 'spartan3'])
    db_root_arg(parser)

    args = parser.parse_args()
    env = os.environ.copy()
    cwd = os.path.dirname(os.path.abspath(__file__))
    resource_path = os.path.join(
        os.getenv('XRAY_DIR'), 'settings', args.family)
    information = {}

    parts = get_parts(args.db_root)
    processed_parts = dict()
    for part in parts.keys():
        # Skip parts which differ only in the speedgrade, as they have the same pins
        fields = part.split("-")
        common_part = fields[0]
        if common_part in processed_parts:
            information[part] = processed_parts[common_part]
            continue

        print("Find pins for {}".format(part))
        env['XRAY_PART'] = part
        _, tmp_file = tempfile.mkstemp()
        # Asks with get_package_pins and different filters for pins with
        # specific properties.
        if (args.family in ['spartan3']):
            #TODO: Use info from partgen -v|-p spartan3e to generate this
            # clk_pins: less CLK xc3s1200efg320.pkg | grep CLK | awk 'ORS=" " { print $3}'
            # data_pins: less xc3s1200efg320.pkg | awk '{print $2,$3,$6}' | grep PAD | awk 'ORS=" " {print $2}'
            pins_json = {
                "clk_pins": "U10 T10 B8 B9 D9 C9 A10 B10 E10 D10 J14 J15 J16 J17 K14 K15 K12 K13 U16 P10 R10 V9 U9 M9 N9 K5 K6 K4 K3 J2 J1 J4 J5",
                "data_pins": "A3 D3 F4 F5 G1 J7 J6 K2 K7 M1 N1 N2 R4 R1 U1 V2 V4 V3 T7 R7 U8 V8 U10 T10 T11 U11 V14 U14 V16 R17 P15 N17 L14 L13 K17 K18 H18 G18 E18 E17 D18 B18 C15 B15 A15 C12 D12 F10 G10 B8 B9 C8 D8 A5 B5 H13 B3 C3 B4 A4 C4 D5 C5 E6 D6 A6 B6 A7 E7 F7 D7 C7 A8 F8 E8 F9 E9 G9 D9 C9 A10 B10 B11 E10 D10 D11 C11 A11 F11 E11 A12 E12 F12 D13 B13 A13 A14 B14 E13 C14 D14 A16 B16 C17 C18 D16 D17 E16 E15 F14 F15 G13 G14 F17 F18 G16 G15 H15 H14 H17 H16 J13 J12 J14 J15 J16 J17 K14 K15 K12 K13 L17 L18 L15 L16 M18 N18 M16 M15 P18 P17 M13 M14 N14 N15 P16 R16 R15 T18 R18 T17 U18 U16 T16 U15 V15 T15 R14 T14 R13 P13 P12 N12 U13 R12 T12 V13 V12 R11 N11 P11 N10 M10 V11 P10 R10 V9 U9 R9 M9 N9 T8 R8 P9 N8 P8 V7 P7 N7 U6 V6 V5 P6 R6 T5 R5 U5 T4 U4 T3 U3 T1 T2 R2 R3 P4 P3 P1 P2 N5 N4 M6 M5 M3 M4 L5 L6 L4 L3 L2 L1 K5 K6 K4 K3 J2 J1 J4 J5 H1 H2 H3 H4 H5 H6 G5 G6 G4 G3 F2 F1 E3 E4 E1 E2 D4 D2 D1 C2 C1",
            }
        else:
            command = "env TMP_FILE={} {} -mode batch -source update_resources.tcl".format(
                tmp_file, env['XRAY_VIVADO'])
            result = subprocess.run(
                command.split(' '),
                check=True,
                env=env,
                cwd=cwd,
                stdout=subprocess.PIPE)

            with OpenSafeFile(tmp_file, "r") as fp:
                pins_json = json.load(fp)

            os.remove(tmp_file)

        clk_pins = pins_json["clk_pins"].split()
        data_pins = pins_json["data_pins"].split()
        pins = {
            0: clk_pins[0],
            1: data_pins[0],
            2: data_pins[int(len(data_pins) / 2)],
            3: data_pins[-1]
        }
        information[part] = {'pins': pins}
        processed_parts[common_part] = {'pins': pins}

    # Overwrites the <family>/resources.yaml file completely with new data
    set_part_resources(resource_path, information)


if __name__ == '__main__':
    main()
