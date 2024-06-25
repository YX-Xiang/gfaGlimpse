#!/bin/bash

INPUT_GFA="./data/test.gfa"
# INPUT_GFA="/home/yxxiang/data/gfaGlimpse/benchmark/data/01_dm_pggb.gfa"

make clean
make -j

./gfaGlimpse.exe ${INPUT_GFA}