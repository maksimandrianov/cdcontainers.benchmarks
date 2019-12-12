#!/usr/bin/env python
import collections
import json
import os
import sys

from matplotlib import pyplot as plt


def main():
    assert len(sys.argv) == 3, \
        ("plot.py bench_collection1.json:bench_collection1.json "
         "1[display graphs]")

    filenames = filter(None, sys.argv[1].split(":"))
    display_graphs = int(sys.argv[2])

    for filename in filenames:
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

            container = os.path.splitext(os.path.basename(filename))[0].split("_")[-1]
            plt.title(f"{container} {operation}")
            plt.ylabel("Time (ns)")
            plt.xlabel("N")
            plt.legend()
            plt.savefig(f"{os.path.splitext(filename)[0]}_{operation}.svg",
                        format='svg', dpi=1200)
            if display_graphs:
                plt.show()
            else:
                plt.close()


if __name__ == "__main__":
    main()
