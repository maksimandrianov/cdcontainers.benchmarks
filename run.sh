#!/usr/bin/env bash

set -e

BASE_DIR=$(dirname "$0")
cd ${BASE_DIR}
BASE_DIR=$(pwd)
echo "BASEDIR: ${BASE_DIR}"

BUILD_DIR="$BASE_DIR/build-benchmarks"
echo "BUILD_DIR: ${BUILD_DIR}"

# Try to install dependencies.
mkdir -p ${BUILD_DIR} && \
    cd ${BUILD_DIR} && \
    cmake .. && \
    make all -j4

cd ${BASE_DIR}
pip install -r "${BASE_DIR}/requirements.txt"

# Run benchmarks.
for f in ${BUILD_DIR}/bench_* ; do
    if [[ -x ${f} ]]; then
        BENCHMARK=${f}
        OUT_FILENAME="${BUILD_DIR}/_$(basename ${BENCHMARK}).json"
        ${BENCHMARK} --benchmark_format=json >${OUT_FILENAME}
        "${BASE_DIR}/plot.py" ${OUT_FILENAME}
     fi
done

