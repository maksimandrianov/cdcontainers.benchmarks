#!/usr/bin/env python
import collections
import json
import sys

from matplotlib import pyplot as plt


def main():
    assert len(sys.argv) == 2, "plot.py benchmark.json"
    filename = sys.argv[1]

    with open(filename) as f:
        raw = json.load(f)
        benchmarks = raw["benchmarks"]
        grouped_benchmarks = collections.defaultdict(
            lambda: collections.defaultdict(dict)
        )
        for bench in benchmarks:
            first, last = bench["name"].rsplit("/", maxsplit=1)
            if "threads" in last:
                bench["name"] = first
                
            name, count = bench["name"].rsplit("/", maxsplit=1)
            name = name.split("_", maxsplit=2)[-1]
            count = int(count)
            operation = bench["name"].split("_", maxsplit=2)[1]
            cpu_time = float(bench["cpu_time"])
            grouped_benchmarks[operation][name][count] = cpu_time

    for operation, v in grouped_benchmarks.items():
        for name, times in v.items():
            plt.plot(list(times.keys()), list(times.values()),
                     marker='o', label=name)

        plt.title(operation)
        plt.ylabel("Time (ns)")
        plt.xlabel("N")
        plt.legend()
        plt.show()


if __name__ == "__main__":
    main()
