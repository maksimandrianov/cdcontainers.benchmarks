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
#pragma once

#include <cstdint>
#include <vector>

#define S(benchmark)                        \
  benchmark->RangeMultiplier(2)             \
      ->Range(1 << 2, 1 << 15)              \
      ->DenseRange(1 << 16, 1 << 20, 65536) \
      ->Threads(4);

class RandomSet
{
 public:
  RandomSet(size_t size);

  template <typename Fn>
  void ForEach(Fn &&fn)
  {
    for (auto v : _vec) {
      fn(v);
    }
  }

  int Get();

 private:
  std::vector<int> _vec;
};

int GetRandom();

size_t GetRandomPos(size_t len);

int IsEquil(const void *lhs, const void *rhs);

int Less(const void *lhs, const void *rhs);
int CcCmp(const void *lhs, const void *rhs);

size_t Hash(const void *key);
unsigned int GHash(const void *key);
size_t CcHash(const void *key, int /* l */, uint32_t /* seed */);
