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
#include <gmodule.h>
}

#include <benchmark/benchmark.h>

#include "benchmarks/utils.hpp"

#include <map>
#include <unordered_map>

gboolean GTraverse(gpointer key, gpointer value, gpointer data)
{
  benchmark::DoNotOptimize(value);
  return false;
}

// Insert benchmarks:
template <class Container>
static void BM_Insert_Cpp(benchmark::State &state)
{
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    auto c = new Container;
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      c->emplace(GetRandom(), value);
    }

    state.PauseTiming();
    delete c;
    state.ResumeTiming();
  }
}
S(BENCHMARK_TEMPLATE(BM_Insert_Cpp, std::map<int, void *>));
S(BENCHMARK_TEMPLATE(BM_Insert_Cpp, std::unordered_map<int, void *>));

static void BM_Insert_CcHashTable(benchmark::State &state)
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
S(BENCHMARK(BM_Insert_CcHashTable));

static void BM_Insert_CcTreeTable(benchmark::State &state)
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
S(BENCHMARK(BM_Insert_CcTreeTable));

static void BM_Insert_GTree(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GTree *tree = g_tree_new(CcCmp);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      g_tree_insert(tree, CDC_FROM_INT(GetRandom()), nullptr);
    }

    state.PauseTiming();
    g_tree_destroy(tree);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Insert_GTree));

static void BM_Insert_GHashTable(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GHashTable *table = g_hash_table_new(GHash, IsEquil);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      g_hash_table_insert(table, CDC_FROM_INT(GetRandom()), nullptr);
    }

    state.PauseTiming();
    g_hash_table_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Insert_GHashTable));

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

static void BM_Insert_CdcHashTable(benchmark::State &state)
{
  struct cdc_data_info info = {};
  info.eq = IsEquil;
  info.hash = Hash;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_hash_table *map = nullptr;
    cdc_hash_table_ctor(&map, &info);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_hash_table_insert(map, CDC_FROM_INT(GetRandom()), nullptr, nullptr,
                            nullptr);
    }

    state.PauseTiming();
    cdc_hash_table_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Insert_CdcHashTable));

static void BM_Insert_CdcAvlTree(benchmark::State &state)
{
  struct cdc_data_info info = {};
  info.cmp = Less;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_avl_tree *map = nullptr;
    cdc_avl_tree_ctor(&map, &info);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_avl_tree_insert1(map, CDC_FROM_INT(GetRandom()), nullptr, nullptr,
                           nullptr);
    }

    state.PauseTiming();
    cdc_avl_tree_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Insert_CdcAvlTree));

// Remove benchmarks:
template <class Container>
static void BM_Remove_Cpp(benchmark::State &state)
{
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    auto c = new Container;
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { c->emplace(v, value); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      c->erase(rs.Get());
    }

    state.PauseTiming();
    delete c;
    state.ResumeTiming();
  }
}
S(BENCHMARK_TEMPLATE(BM_Remove_Cpp, std::map<int, void *>));
S(BENCHMARK_TEMPLATE(BM_Remove_Cpp, std::unordered_map<int, void *>));

static void BM_Remove_CcHashTable(benchmark::State &state)
{
  HashTableConf conf;
  hashtable_conf_init(&conf);
  conf.key_compare = IsEquil;
  conf.hash = CcHash;
  for (auto _ : state) {
    state.PauseTiming();
    HashTable *table = nullptr;
    hashtable_new_conf(&conf, &table);
    RandomSet rs(static_cast<size_t>(state.range(0)));
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
S(BENCHMARK(BM_Remove_CcHashTable));

static void BM_Remove_CcTreeTable(benchmark::State &state)
{
  TreeTableConf conf;
  treetable_conf_init(&conf);
  conf.cmp = CcCmp;
  for (auto _ : state) {
    state.PauseTiming();
    TreeTable *table = nullptr;
    treetable_new_conf(&conf, &table);
    RandomSet rs(static_cast<size_t>(state.range(0)));
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
S(BENCHMARK(BM_Remove_CcTreeTable));

static void BM_Remove_GTree(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GTree *tree = g_tree_new(CcCmp);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { g_tree_insert(tree, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      g_tree_remove(tree, CDC_FROM_INT(rs.Get()));
    }

    state.PauseTiming();
    g_tree_destroy(tree);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Remove_GTree));

static void BM_Remove_GHashTable(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GHashTable *table = g_hash_table_new(GHash, IsEquil);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach(
        [&](auto v) { g_hash_table_insert(table, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      g_hash_table_remove(table, CDC_FROM_INT(rs.Get()));
    }

    state.PauseTiming();
    g_hash_table_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Remove_GHashTable));

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
    RandomSet rs(static_cast<size_t>(state.range(0)));
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

static void BM_Remove_CdcHashTable(benchmark::State &state)
{
  struct cdc_data_info info = {};
  info.eq = IsEquil;
  info.hash = Hash;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_hash_table *map = nullptr;
    cdc_hash_table_ctor(&map, &info);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([=](auto v) {
      cdc_hash_table_insert(map, CDC_FROM_INT(v), nullptr, nullptr, nullptr);
    });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_hash_table_erase(map, CDC_FROM_INT(rs.Get()));
    }

    state.PauseTiming();
    cdc_hash_table_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Remove_CdcHashTable));

static void BM_Remove_CdcAvlTree(benchmark::State &state)
{
  struct cdc_data_info info = {};
  info.cmp = Less;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_avl_tree *map = nullptr;
    cdc_avl_tree_ctor(&map, &info);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([=](auto v) {
      cdc_avl_tree_insert1(map, CDC_FROM_INT(v), nullptr, nullptr, nullptr);
    });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_avl_tree_erase(map, CDC_FROM_INT(rs.Get()));
    }

    state.PauseTiming();
    cdc_avl_tree_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Remove_CdcAvlTree));

// Search benchmarks:
template <class Container>
static void BM_Search_Cpp(benchmark::State &state)
{
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    auto c = new Container;
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { c->emplace(v, value); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      benchmark::DoNotOptimize(c->operator[](rs.Get()));
    }

    state.PauseTiming();
    delete c;
    state.ResumeTiming();
  }
}
S(BENCHMARK_TEMPLATE(BM_Search_Cpp, std::map<int, void *>));
S(BENCHMARK_TEMPLATE(BM_Search_Cpp, std::unordered_map<int, void *>));

static void BM_Search_CcHashTable(benchmark::State &state)
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
    RandomSet rs(static_cast<size_t>(state.range(0)));
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
S(BENCHMARK(BM_Search_CcHashTable));

static void BM_Search_CcTreeTable(benchmark::State &state)
{
  TreeTableConf conf;
  treetable_conf_init(&conf);
  conf.cmp = CcCmp;
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    TreeTable *table = nullptr;
    treetable_new_conf(&conf, &table);
    RandomSet rs(static_cast<size_t>(state.range(0)));
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
S(BENCHMARK(BM_Search_CcTreeTable));

static void BM_Search_GTree(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GTree *tree = g_tree_new(CcCmp);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { g_tree_insert(tree, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      benchmark::DoNotOptimize(g_tree_lookup(tree, CDC_FROM_INT(rs.Get())));
    }

    state.PauseTiming();
    g_tree_destroy(tree);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Search_GTree));

static void BM_Search_GHashTable(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GHashTable *table = g_hash_table_new(GHash, IsEquil);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach(
        [&](auto v) { g_hash_table_insert(table, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      benchmark::DoNotOptimize(
          g_hash_table_lookup(table, CDC_FROM_INT(rs.Get())));
    }

    state.PauseTiming();
    g_hash_table_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Search_GHashTable));

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
    RandomSet rs(static_cast<size_t>(state.range(0)));
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

static void BM_Search_CdcHashTable(benchmark::State &state)
{
  struct cdc_data_info info = {};
  info.eq = IsEquil;
  info.hash = Hash;
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_hash_table *map = nullptr;
    cdc_hash_table_ctor(&map, &info);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([=](auto v) {
      cdc_hash_table_insert(map, CDC_FROM_INT(v), nullptr, nullptr, nullptr);
    });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      benchmark::DoNotOptimize(
          cdc_hash_table_get(map, CDC_FROM_INT(rs.Get()), &value));
    }

    state.PauseTiming();
    cdc_hash_table_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Search_CdcHashTable));

static void BM_Search_CdcAvlTree(benchmark::State &state)
{
  struct cdc_data_info info = {};
  info.cmp = Less;
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_avl_tree *map = nullptr;
    cdc_avl_tree_ctor(&map, &info);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([=](auto v) {
      cdc_avl_tree_insert1(map, CDC_FROM_INT(v), nullptr, nullptr, nullptr);
    });
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      benchmark::DoNotOptimize(
          cdc_avl_tree_get(map, CDC_FROM_INT(rs.Get()), &value));
    }

    state.PauseTiming();
    cdc_avl_tree_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_Search_CdcAvlTree));

// Iterator traversal benchmarks:
template <class Container>
static void BM_ItTraversal_Cpp(benchmark::State &state)
{
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    auto c = new Container;
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { c->emplace(v, value); });
    state.ResumeTiming();

    auto end = std::end(*c);
    for (auto it = std::begin(*c); it != end; ++it) {
      benchmark::DoNotOptimize(it->second);
    }

    state.PauseTiming();
    delete c;
    state.ResumeTiming();
  }
}
S(BENCHMARK_TEMPLATE(BM_ItTraversal_Cpp, std::map<int, void *>));
S(BENCHMARK_TEMPLATE(BM_ItTraversal_Cpp, std::unordered_map<int, void *>));

static void BM_ItTraversal_CcHashTable(benchmark::State &state)
{
  HashTableConf conf;
  hashtable_conf_init(&conf);
  conf.key_compare = IsEquil;
  conf.hash = CcHash;
  for (auto _ : state) {
    state.PauseTiming();
    HashTable *table = nullptr;
    hashtable_new_conf(&conf, &table);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { hashtable_add(table, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    HashTableIter it;
    hashtable_iter_init(&it, table);
    TableEntry *entry;
    while (hashtable_iter_next(&it, &entry) != CC_ITER_END) {
      benchmark::DoNotOptimize(entry->value);
    }

    state.PauseTiming();
    hashtable_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_ItTraversal_CcHashTable));

static void BM_ItTraversal_CcTreeTable(benchmark::State &state)
{
  TreeTableConf conf;
  treetable_conf_init(&conf);
  conf.cmp = CcCmp;
  for (auto _ : state) {
    state.PauseTiming();
    TreeTable *table = nullptr;
    treetable_new_conf(&conf, &table);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { treetable_add(table, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    TreeTableIter it;
    treetable_iter_init(&it, table);
    TreeTableEntry entry;
    while (treetable_iter_next(&it, &entry) != CC_ITER_END) {
      benchmark::DoNotOptimize(entry.value);
    }

    state.PauseTiming();
    treetable_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_ItTraversal_CcTreeTable));

static void BM_ItTraversal_GTree(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GTree *tree = g_tree_new(CcCmp);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([&](auto v) { g_tree_insert(tree, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    g_tree_foreach(tree, GTraverse, nullptr);

    state.PauseTiming();
    g_tree_destroy(tree);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_ItTraversal_GTree));

static void BM_ItTraversal_GHashTable(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GHashTable *table = g_hash_table_new(GHash, IsEquil);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach(
        [&](auto v) { g_hash_table_insert(table, CDC_FROM_INT(v), nullptr); });
    state.ResumeTiming();

    GHashTableIter it;
    g_hash_table_iter_init(&it, table);
    void *key;
    void *value;
    while (g_hash_table_iter_next(&it, &key, &value)) {
      benchmark::DoNotOptimize(value);
    }

    state.PauseTiming();
    g_hash_table_destroy(table);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_ItTraversal_GHashTable));

static void BM_ItTraversal_CdcMap(benchmark::State &state,
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
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([=](auto v) {
      cdc_map_insert(map, CDC_FROM_INT(v), nullptr, nullptr, nullptr);
    });
    state.ResumeTiming();

    cdc_map_iter it;
    cdc_map_iter_ctor(map, &it);
    cdc_map_begin(map, &it);
    while (cdc_map_iter_has_next(&it)) {
      benchmark::DoNotOptimize(cdc_map_iter_value(&it));
      cdc_map_iter_next(&it);
    }
    cdc_map_iter_dtor(&it);

    state.PauseTiming();
    cdc_map_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK_CAPTURE(BM_ItTraversal_CdcMap, hash_table, cdc_map_htable));
S(BENCHMARK_CAPTURE(BM_ItTraversal_CdcMap, avl_tree, cdc_map_avl));
S(BENCHMARK_CAPTURE(BM_ItTraversal_CdcMap, treep, cdc_map_treap));
S(BENCHMARK_CAPTURE(BM_ItTraversal_CdcMap, splay_tree, cdc_map_splay));

static void BM_ItTraversal_CdcHashTable(benchmark::State &state)
{
  struct cdc_data_info info = {};
  info.eq = IsEquil;
  info.hash = Hash;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_hash_table *map = nullptr;
    cdc_hash_table_ctor(&map, &info);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([=](auto v) {
      cdc_hash_table_insert(map, CDC_FROM_INT(v), nullptr, nullptr, nullptr);
    });
    state.ResumeTiming();

    cdc_hash_table_iter it;
    cdc_hash_table_begin(map, &it);
    while (cdc_hash_table_iter_has_next(&it)) {
      benchmark::DoNotOptimize(cdc_hash_table_iter_value(&it));
      cdc_hash_table_iter_next(&it);
    }

    state.PauseTiming();
    cdc_hash_table_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_ItTraversal_CdcHashTable));

static void BM_ItTraversal_CdcAvlTree(benchmark::State &state)
{
  struct cdc_data_info info = {};
  info.cmp = Less;
  void *value = nullptr;
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_avl_tree *map = nullptr;
    cdc_avl_tree_ctor(&map, &info);
    RandomSet rs(static_cast<size_t>(state.range(0)));
    rs.ForEach([=](auto v) {
      cdc_avl_tree_insert1(map, CDC_FROM_INT(v), nullptr, nullptr, nullptr);
    });
    state.ResumeTiming();

    cdc_avl_tree_iter it;
    cdc_avl_tree_begin(map, &it);
    while (cdc_avl_tree_iter_has_next(&it)) {
      benchmark::DoNotOptimize(cdc_avl_tree_iter_value(&it));
      cdc_avl_tree_iter_next(&it);
    }

    state.PauseTiming();
    cdc_avl_tree_dtor(map);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_ItTraversal_CdcAvlTree));

BENCHMARK_MAIN();
