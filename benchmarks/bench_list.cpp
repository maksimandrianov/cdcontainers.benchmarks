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
#include <collectc/list.h>
#include <gmodule.h>
}

#include <benchmark/benchmark.h>

#include "benchmarks/utils.hpp"

#include <list>

// Push back benchmarks:
static void BM_PushBack_CppList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    auto list = new std::list<int>();
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      list->push_back(GetRandom());
    }

    state.PauseTiming();
    delete list;
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushBack_CppList));

static void BM_PushBack_CdcList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_list *list = nullptr;
    cdc_list_ctor(&list, nullptr);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_list_push_back(list, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    cdc_list_dtor(list);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushBack_CdcList));

static void BM_PushBack_CcList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    List *list = nullptr;
    list_new(&list);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      list_add_last(list, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    list_destroy(list);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushBack_CcList));

static void BM_PushBack_GList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GList *list = g_list_alloc();
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      list = g_list_append(list, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    g_list_free(list);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushBack_GList));

// Push front benchmarks:
static void BM_PushFront_CppList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    auto list = new std::list<int>();
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      list->push_front(GetRandom());
    }

    state.PauseTiming();
    delete list;
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushFront_CppList));

static void BM_PushFront_CdcList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_list *list = nullptr;
    cdc_list_ctor(&list, nullptr);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_list_push_front(list, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    cdc_list_dtor(list);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushFront_CdcList));

static void BM_PushFront_CcList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    List *list = nullptr;
    list_new(&list);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      list_add_first(list, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    list_destroy(list);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushFront_CcList));

static void BM_PushFront_GList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GList *list = g_list_alloc();
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      list = g_list_prepend(list, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    g_list_free(list);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushFront_GList));

// Insert mid benchmarks:
static void BM_InsertMid_CppList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    auto list = new std::list<int>{1, 2, 3, 4, 5};
    auto it = std::find(std::cbegin(*list), std::cend(*list), 3);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      list->insert(it, GetRandom());
    }

    state.PauseTiming();
    delete list;
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_InsertMid_CppList));

static void BM_InsertMid_CdcList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_list *list = nullptr;
    cdc_list_ctorl(&list, nullptr, CDC_FROM_INT(1), CDC_FROM_INT(2),
                   CDC_FROM_INT(3), CDC_FROM_INT(4), CDC_FROM_INT(5), CDC_END);
    struct cdc_list_iter it = {};
    cdc_list_begin(list, &it);
    while (cdc_list_iter_has_next(&it) &&
           cdc_list_iter_data(&it) != CDC_FROM_INT(3)) {
      cdc_list_iter_next(&it);
    }
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_list_iinsert(&it, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    cdc_list_dtor(list);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_InsertMid_CdcList));

static void BM_InsertMid_CcList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    List *list = nullptr;
    list_new(&list);
    for (int i = 1; i < 6; ++i) {
      list_add_last(list, CDC_FROM_INT(i));
    }
    ListIter iter = {};
    list_iter_init(&iter, list);
    void *val = nullptr;
    while (list_iter_next(&iter, &val) != CC_ITER_END &&
           val != CDC_FROM_INT(3)) {
      /* empty */;
    }
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      list_iter_add(&iter, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    list_destroy(list);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_InsertMid_CcList));

static void BM_InsertMid_GList(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GList *list = g_list_alloc();
    for (int i = 1; i < 6; ++i) {
      list = g_list_append(list, CDC_FROM_INT(i));
    }
    GList *it = g_list_find(list, CDC_FROM_INT(3));
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      list = g_list_insert_before(list, it, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    g_list_free(list);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_InsertMid_GList));

BENCHMARK_MAIN();
