// The MIT License (MIT)
// Copyright (c) 2019 Maksim Andrianov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
extern "C" {
#include <cdcontainers/cdc.h>
#include <collectc/hashtable.h>
#include <collectc/treetable.h>
}

#include <benchmark/benchmark.h>

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <unordered_map>
#include <unordered_set>

#define S(benchmark)                      \
  benchmark->RangeMultiplier(2)           \
      ->Range(4, 1 << 15)                 \
      ->DenseRange(65536, 1 << 17, 65536) \
      ->Threads(4);

int GetRandom();

struct RemoveSet {
  RemoveSet(size_t size)
  {
    _vec.reserve(size);
    int i = 0;
    std::generate_n(std::back_inserter(_vec), size, [&]() { return ++i; });
    std::random_shuffle(std::begin(_vec), std::end(_vec));
  }

  template <typename Fn>
  void ForEach(Fn &&fn)
  {
    for (auto v : _vec) {
      fn(v);
    }
  }

  int Get()
  {
    int back = _vec.back();
    _vec.pop_back();
    return back;
  }

  std::vector<int> _vec;
};

int GetRandom()
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(),
                                             std::numeric_limits<int>::max());
  return dis(gen);
}

static int Less(const void *l, const void *r)
{
  return CDC_TO_INT(l) < CDC_TO_INT(r);
}

static int IsEquil(const void *lhs, const void *rhs)
{
  return CDC_TO_INT(lhs) == CDC_TO_INT(rhs);
}

static size_t Hash(const void *key) { return cdc_hash_int(CDC_TO_INT(key)); }

static size_t CcHash(const void *key, int /* l */, uint32_t /* seed */)
{
  return cdc_hash_int(CDC_TO_INT(key));
}

static int CcCmp(const void *l, const void *r)
{
  if (CDC_TO_INT(l) < CDC_TO_INT(r)) {
    return -1;
  } else if (CDC_TO_INT(l) == CDC_TO_INT(r)) {
    return 0;
  } else {
    return 1;
  }
}

// Insert benchmarks:
template <class Container>
static void BM_Insert_Cpp(benchmark::State &state)
{
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    Container c;
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      c.emplace(GetRandom(), value);
    }
  }
}
S(BENCHMARK_TEMPLATE(BM_Insert_Cpp, std::map<int, void *>));
S(BENCHMARK_TEMPLATE(BM_Insert_Cpp, std::unordered_map<int, void *>));

static void BM_Insert_CdcMap(benchmark::State &state,
                             const struct cdc_map_table *table)
{
  struct cdc_data_info info = {};
  info.eq = IsEquil;
  info.cmp = Less;
  info.hash = Hash;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_map *map = nullptr;
    cdc_map_ctor(table, &map, &info);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_map_insert(map, CDC_FROM_INT(GetRandom()), nullptr, nullptr, nullptr);
    }

    state.PauseTiming();
    cdc_map_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK_CAPTURE(BM_Insert_CdcMap, hash_table, cdc_map_htable));
S(BENCHMARK_CAPTURE(BM_Insert_CdcMap, avl_tree, cdc_map_avl));
S(BENCHMARK_CAPTURE(BM_Insert_CdcMap, treep, cdc_map_treap));
S(BENCHMARK_CAPTURE(BM_Insert_CdcMap, splay_tree, cdc_map_splay));

static void BM_Insert_CC_HashTable(benchmark::State &state)
{
  HashTableConf conf;
  hashtable_conf_init(&conf);
  conf.key_compare = IsEquil;
  conf.hash = CcHash;
  for (auto _ : state) {
    state.PauseTiming();
    HashTable *table = nullptr;
    hashtable_new_conf(&conf, &table);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      hashtable_add(table, CDC_FROM_INT(GetRandom()), nullptr);
    }

    state.PauseTiming();
    hashtable_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Insert_CC_HashTable))

static void BM_Insert_CC_TreeTable(benchmark::State &state)
{
  TreeTableConf conf;
  treetable_conf_init(&conf);
  conf.cmp = CcCmp;
  for (auto _ : state) {
    state.PauseTiming();
    TreeTable *table;
    treetable_new_conf(&conf, &table);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      treetable_add(table, CDC_FROM_INT(GetRandom()), nullptr);
    }

    state.PauseTiming();
    treetable_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Insert_CC_TreeTable))

// Remove benchmarks:
template <class Container>
static void BM_Remove_Cpp(benchmark::State &state)
{
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    Container c;
    RemoveSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { c.emplace(v, value); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      c.erase(rs.Get());
    }
  }
}
S(BENCHMARK_TEMPLATE(BM_Remove_Cpp, std::map<int, void *>));
S(BENCHMARK_TEMPLATE(BM_Remove_Cpp, std::unordered_map<int, void *>));

static void BM_Remove_CdcMap(benchmark::State &state,
                             const struct cdc_map_table *table)
{
  struct cdc_data_info info = {};
  info.eq = IsEquil;
  info.cmp = Less;
  info.hash = Hash;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_map *map = nullptr;
    cdc_map_ctor(table, &map, &info);
    RemoveSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([=](auto v) {
      cdc_map_insert(map, CDC_FROM_INT(v), nullptr, nullptr, nullptr);
    });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_map_erase(map, CDC_FROM_INT(rs.Get()));
    }

    state.PauseTiming();
    cdc_map_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK_CAPTURE(BM_Remove_CdcMap, hash_table, cdc_map_htable));
S(BENCHMARK_CAPTURE(BM_Remove_CdcMap, avl_tree, cdc_map_avl));
S(BENCHMARK_CAPTURE(BM_Remove_CdcMap, treep, cdc_map_treap));
S(BENCHMARK_CAPTURE(BM_Remove_CdcMap, splay_tree, cdc_map_splay));

static void BM_Remove_CC_HashTable(benchmark::State &state)
{
  HashTableConf conf;
  hashtable_conf_init(&conf);
  conf.key_compare = IsEquil;
  conf.hash = CcHash;
  for (auto _ : state) {
    state.PauseTiming();
    HashTable *table = nullptr;
    hashtable_new_conf(&conf, &table);
    RemoveSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { hashtable_add(table, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      hashtable_remove(table, CDC_FROM_INT(rs.Get()), nullptr);
    }

    state.PauseTiming();
    hashtable_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Remove_CC_HashTable))

static void BM_Remove_CC_TreeTable(benchmark::State &state)
{
  TreeTableConf conf;
  treetable_conf_init(&conf);
  conf.cmp = CcCmp;
  for (auto _ : state) {
    state.PauseTiming();
    TreeTable *table = nullptr;
    treetable_new_conf(&conf, &table);
    RemoveSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { treetable_add(table, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      treetable_remove(table, CDC_FROM_INT(rs.Get()), nullptr);
    }

    state.PauseTiming();
    treetable_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Remove_CC_TreeTable))

// Search benchmarks:
template <class Container>
static void BM_Search_Cpp(benchmark::State &state)
{
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    Container c;
    RemoveSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { c.emplace(v, value); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      benchmark::DoNotOptimize(c[rs.Get()]);
    }
  }
}
S(BENCHMARK_TEMPLATE(BM_Search_Cpp, std::map<int, void *>));
S(BENCHMARK_TEMPLATE(BM_Search_Cpp, std::unordered_map<int, void *>));

static void BM_Search_CdcMap(benchmark::State &state,
                             const struct cdc_map_table *table)
{
  struct cdc_data_info info = {};
  info.eq = IsEquil;
  info.cmp = Less;
  info.hash = Hash;
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_map *map = nullptr;
    cdc_map_ctor(table, &map, &info);
    RemoveSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([=](auto v) {
      cdc_map_insert(map, CDC_FROM_INT(v), nullptr, nullptr, nullptr);
    });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      benchmark::DoNotOptimize(
          cdc_map_get(map, CDC_FROM_INT(rs.Get()), &value));
    }

    state.PauseTiming();
    cdc_map_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK_CAPTURE(BM_Search_CdcMap, hash_table, cdc_map_htable));
S(BENCHMARK_CAPTURE(BM_Search_CdcMap, avl_tree, cdc_map_avl));
S(BENCHMARK_CAPTURE(BM_Search_CdcMap, treep, cdc_map_treap));
S(BENCHMARK_CAPTURE(BM_Search_CdcMap, splay_tree, cdc_map_splay));

static void BM_Search_CC_HashTable(benchmark::State &state)
{
  HashTableConf conf;
  hashtable_conf_init(&conf);
  conf.key_compare = IsEquil;
  conf.hash = CcHash;
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    HashTable *table = nullptr;
    hashtable_new_conf(&conf, &table);
    RemoveSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { hashtable_add(table, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      benchmark::DoNotOptimize(
          hashtable_get(table, CDC_FROM_INT(rs.Get()), &value));
    }

    state.PauseTiming();
    hashtable_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Search_CC_HashTable))

static void BM_Search_CC_TreeTable(benchmark::State &state)
{
  TreeTableConf conf;
  treetable_conf_init(&conf);
  conf.cmp = CcCmp;
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    TreeTable *table = nullptr;
    treetable_new_conf(&conf, &table);
    RemoveSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { treetable_add(table, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      benchmark::DoNotOptimize(
          treetable_get(table, CDC_FROM_INT(rs.Get()), &value));
    }

    state.PauseTiming();
    treetable_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Search_CC_TreeTable))

BENCHMARK_MAIN();
