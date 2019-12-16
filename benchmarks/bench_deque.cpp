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
#include <collectc/deque.h>
#include <gmodule.h>
}

#include <benchmark/benchmark.h>

#include "benchmarks/utils.hpp"

#include <deque>
#include <iterator>

// Push back benchmarks:
static void BM_PushBack_CppDeque(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    auto deque = new std::deque<int>();
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      deque->push_back(GetRandom());
    }

    state.PauseTiming();
    delete deque;
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushBack_CppDeque));

static void BM_PushBack_CcDeque(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    Deque *deque = nullptr;
    deque_new(&deque);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      deque_add_last(deque, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    deque_destroy(deque);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushBack_CcDeque));

static void BM_PushBack_GQueue(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GQueue *deque = g_queue_new();
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      g_queue_push_tail(deque, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    g_queue_free(deque);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushBack_GQueue));

static void BM_PushBack_CdcDeque(benchmark::State &state,
                                 const struct cdc_sequence_table *table)
{
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_deque *deque = nullptr;
    cdc_deque_ctor(table, &deque, nullptr);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_deque_push_back(deque, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    cdc_deque_dtor(deque);
    state.ResumeTiming();
  }
}
S(BENCHMARK_CAPTURE(BM_PushBack_CdcDeque, circular_array, cdc_seq_carray));
S(BENCHMARK_CAPTURE(BM_PushBack_CdcDeque, list, cdc_seq_list));

static void BM_PushBack_CdcCircularArray(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_circular_array *deque = nullptr;
    cdc_circular_array_ctor(&deque, nullptr);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_circular_array_push_back(deque, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    cdc_circular_array_dtor(deque);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushBack_CdcCircularArray));

// Push front benchmarks:
static void BM_PushFront_CppDeque(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    auto deque = new std::deque<int>();
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      deque->push_front(GetRandom());
    }

    state.PauseTiming();
    delete deque;
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushFront_CppDeque));

static void BM_PushFront_CcDeque(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    Deque *deque = nullptr;
    deque_new(&deque);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      deque_add_first(deque, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    deque_destroy(deque);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushFront_CcDeque));

static void BM_PushFront_GQueue(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GQueue *deque = g_queue_new();
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      g_queue_push_head(deque, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    g_queue_free(deque);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushFront_GQueue));

static void BM_PushFront_CdcDeque(benchmark::State &state,
                                  const struct cdc_sequence_table *table)
{
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_deque *deque = nullptr;
    cdc_deque_ctor(table, &deque, nullptr);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_deque_push_front(deque, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    cdc_deque_dtor(deque);
    state.ResumeTiming();
  }
}
S(BENCHMARK_CAPTURE(BM_PushFront_CdcDeque, circular_array, cdc_seq_carray));
S(BENCHMARK_CAPTURE(BM_PushFront_CdcDeque, list, cdc_seq_list));

static void BM_PushFront_CdcCircularArray(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_circular_array *deque = nullptr;
    cdc_circular_array_ctor(&deque, nullptr);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_circular_array_push_front(deque, CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    cdc_circular_array_dtor(deque);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_PushFront_CdcCircularArray));

// Insert rand pos benchmarks:
static void BM_InsertRandPos_CppDeque(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    auto deque = new std::deque<int>{1, 2, 3, 4, 5};
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      auto it = std::begin(*deque);
      std::advance(it, GetRandomPos(deque->size()));
      deque->insert(it, GetRandom());
    }

    state.PauseTiming();
    delete deque;
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_InsertRandPos_CppDeque));

static void BM_InsertRandPos_CcDeque(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    Deque *deque = nullptr;
    deque_new(&deque);
    for (int i = 1; i < 6; ++i) {
      deque_add_last(deque, CDC_FROM_INT(i));
    }
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      deque_add_at(deque, CDC_FROM_INT(GetRandom()),
                   GetRandomPos(deque_size(deque)));
    }

    state.PauseTiming();
    deque_destroy(deque);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_InsertRandPos_CcDeque));

static void BM_InsertRandPos_GQueue(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    GQueue *deque = g_queue_new();
    for (int i = 1; i < 6; ++i) {
      g_queue_push_head(deque, CDC_FROM_INT(i));
    }
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      g_queue_push_nth(
          deque, CDC_FROM_INT(GetRandom()),
          static_cast<int>(GetRandomPos(g_queue_get_length(deque))));
    }

    state.PauseTiming();
    g_queue_free(deque);
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_InsertRandPos_GQueue));

static void BM_InsertRandPos_CdcDeque(benchmark::State &state,
                                      const struct cdc_sequence_table *table)
{
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_deque *deque = nullptr;
    cdc_deque_ctorl(table, &deque, nullptr, CDC_FROM_INT(1), CDC_FROM_INT(2),
                    CDC_FROM_INT(3), CDC_FROM_INT(4), CDC_FROM_INT(5), CDC_END);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_deque_insert(deque, GetRandomPos(cdc_deque_size(deque)),
                       CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    cdc_deque_dtor(deque);
    deque = nullptr;
    state.ResumeTiming();
  }
}
S(BENCHMARK_CAPTURE(BM_InsertRandPos_CdcDeque, circular_array, cdc_seq_carray));
S(BENCHMARK_CAPTURE(BM_InsertRandPos_CdcDeque, list, cdc_seq_list));

static void BM_InsertRandPos_CdcCircularArray(benchmark::State &state)
{
  for (auto _ : state) {
    state.PauseTiming();
    struct cdc_circular_array *deque = nullptr;
    cdc_circular_array_ctorl(&deque, nullptr, CDC_FROM_INT(1), CDC_FROM_INT(2),
                             CDC_FROM_INT(3), CDC_FROM_INT(4), CDC_FROM_INT(5),
                             CDC_END);
    state.ResumeTiming();

    for (int j = 0; j < state.range(0); ++j) {
      cdc_circular_array_insert(deque,
                                GetRandomPos(cdc_circular_array_size(deque)),
                                CDC_FROM_INT(GetRandom()));
    }

    state.PauseTiming();
    cdc_circular_array_dtor(deque);
    deque = nullptr;
    state.ResumeTiming();
  }
}
S(BENCHMARK(BM_InsertRandPos_CdcCircularArray));

BENCHMARK_MAIN();
