#!/usr/bin/env bash

set -e

function show_help() {
    echo "./run [-hsd] [-b <collection>]
          -h              show help
          -s              skip building of benchmarks
          -d              display graphs
          -b <collection> run bench_<collection>";
}

function run_benchmark() {
    BUILD_DIR=${1}
    BENCHMARK=${2}
    if [[ -x ${BENCHMARK} ]]; then
        OUT_FILENAME="${BUILD_DIR}/_$(basename ${BENCHMARK}).json"
        ${BENCHMARK} --v=2 --benchmark_format=json >${OUT_FILENAME}
        echo ${OUT_FILENAME}
    fi
    echo ""
}

SKIP_BUILD=0

BENCHMARK=""

DISPLAY_GRAPH=0

OPTIND=1
while getopts "h?sdb:" opt; do
    case "$opt" in
    h|\?)
        show_help
        exit 0
        ;;
    s)  SKIP_BUILD=1
        ;;
    d)  DISPLAY_GRAPH=1
        ;;
    b)  BENCHMARK=$OPTARG
        ;;
    esac
done
shift $((OPTIND - 1))

BASE_DIR=$(dirname "$0")
cd ${BASE_DIR}
BASE_DIR=$(pwd)
echo "BASEDIR: ${BASE_DIR}"

BUILD_DIR="$BASE_DIR/build-benchmarks"
echo "BUILD_DIR: ${BUILD_DIR}"

if [[ ${SKIP_BUILD} == 0 ]]; then
    mkdir -p ${BUILD_DIR} && \
        cd ${BUILD_DIR} && \
        cmake .. && \
        make all -j4

    cd ${BASE_DIR}
    pip install -r "${BASE_DIR}/requirements.txt"
fi

OUT_FILENAMES=""
if [[ ${BENCHMARK} != "" ]]; then
    OUT_FILENAME=$(run_benchmark "${BUILD_DIR}" "${BUILD_DIR}/bench_${BENCHMARK}")
    OUT_FILENAMES="${OUT_FILENAMES}:${OUT_FILENAME}"
else
    for f in ${BUILD_DIR}/bench_* ; do
        OUT_FILENAME=$(run_benchmark "${BUILD_DIR}" "${f}")
        OUT_FILENAMES="${OUT_FILENAMES}:${OUT_FILENAME}"
    done
fi

"${BASE_DIR}/plot.py" "${OUT_FILENAMES}" "${DISPLAY_GRAPH}"
