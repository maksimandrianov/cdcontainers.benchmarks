# cdcontainers.benchmarks

Benchmarks for [cdcontainers](https://maksimandrianov.github.io/cdcontainers.github.io/) library.

## Equipment
Benchmarks run on the system:
```sh
 $ uname -a
```
```
Linux 4.15.0-72-generic #81-Ubuntu SMP Tue Nov 26 12:20:02 UTC 2019 x86_64 x86_64 x86_64 GNU/Linux
```

```sh
 $ inxi -C
```
```
CPU:       Quad core Intel Core i7-7820HQ (-MT-MCP-) cache: 8192 KB
           clock speeds: max: 3900 MHz 1: 900 MHz 2: 900 MHz 3: 900 MHz 4: 900 MHz 5: 900 MHz 6: 900 MHz
           7: 900 MHz 8: 900 MHz
```


## Deque

Insertion an element to the beginning:

![](https://raw.githubusercontent.com/maksimandrianov/cdcontainers.benchmarks/master/_graphs/_bench_deque_PushFront.svg?sanitize=true)


Adding an element to the end:

![](https://raw.githubusercontent.com/maksimandrianov/cdcontainers.benchmarks/master/_graphs/_bench_deque_PushBack.svg?sanitize=true)


Insertion an element at a random position:

![](https://raw.githubusercontent.com/maksimandrianov/cdcontainers.benchmarks/master/_graphs/_bench_deque_InsertRandPos.svg?sanitize=true)


## List

Insertion an element to the beginning:

![](https://raw.githubusercontent.com/maksimandrianov/cdcontainers.benchmarks/master/_graphs/_bench_list_PushFront.svg?sanitize=true)


Adding an element to the end:

![](https://raw.githubusercontent.com/maksimandrianov/cdcontainers.benchmarks/master/_graphs/_bench_list_PushBack.svg?sanitize=true)


Insertion an element in the middle:

![](https://raw.githubusercontent.com/maksimandrianov/cdcontainers.benchmarks/master/_graphs/_bench_list_InsertMid.svg?sanitize=true)


## Map

Insertion an element:

![](https://raw.githubusercontent.com/maksimandrianov/cdcontainers.benchmarks/master/_graphs/_bench_map_Insert.svg?sanitize=true)

Erasing an element:

![](https://raw.githubusercontent.com/maksimandrianov/cdcontainers.benchmarks/master/_graphs/_bench_map_Remove.svg?sanitize=true)

Search of an elemen with specific key:

![](https://raw.githubusercontent.com/maksimandrianov/cdcontainers.benchmarks/master/_graphs/_bench_map_Search.svg?sanitize=true)

For each loop with iterators:

![](https://raw.githubusercontent.com/maksimandrianov/cdcontainers.benchmarks/master/_graphs/_bench_map_ItTraversal.svg?sanitize=true)






