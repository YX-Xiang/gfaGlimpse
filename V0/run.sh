#!/bin/bash

# INPUT_GFA="../data/test.gfa"
INPUT_GFA="../example/01_dm_pggb.gfa"
# INPUT_GFA="../example/02_dm_mc.gfa"
# INPUT_GFA="/home/yxxiang/data/gfaGlimpse/benchmark/data/03_at_pggb.gfa"
# INPUT_GFA="/home/yxxiang/data/gfaGlimpse/benchmark/data/04_at_mc.gfa"
# INPUT_GFA="/home/yxxiang/data/gfaGlimpse/benchmark/data/05_hs_pggb.gfa"
# INPUT_GFA="/home/yxxiang/data/gfaGlimpse/benchmark/data/07_hs_pggb.gfa"
OUTPUT_PATH="../data"

make clean > build.log 2>&1
make -j >> build.log 2>&1

./gfaGlimpse.exe ${INPUT_GFA} -o ${OUTPUT_PATH} -t 16