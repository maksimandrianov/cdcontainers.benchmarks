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
#include "benchmarks/utils.hpp"

extern "C" {
#include <cdcontainers/cdc.h>
}

#include <algorithm>
#include <random>

RandomSet::RandomSet(size_t size)
{
  _vec.reserve(size);
  int i = 0;
  std::generate_n(std::back_inserter(_vec), size, [&]() { return ++i; });
  std::random_shuffle(std::begin(_vec), std::end(_vec));
}

int RandomSet::Get()
{
  int back = _vec.back();
  _vec.pop_back();
  return back;
}

int GetRandom()
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(),
                                             std::numeric_limits<int>::max());
  return dis(gen);
}

size_t GetRandomPos(size_t len)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<unsigned int> dis(
      std::numeric_limits<size_t>::max());
  return dis(gen) % len;
}

int IsEquil(const void *lhs, const void *rhs)
{
  return CDC_TO_INT(lhs) == CDC_TO_INT(rhs);
}

int Less(const void *lhs, const void *rhs)
{
  return CDC_TO_INT(lhs) < CDC_TO_INT(rhs);
}

int CcCmp(const void *lhs, const void *rhs)
{
  if (CDC_TO_INT(lhs) < CDC_TO_INT(rhs)) {
    return -1;
  } else if (CDC_TO_INT(lhs) == CDC_TO_INT(rhs)) {
    return 0;
  } else {
    return 1;
  }
}

size_t Hash(const void *key) { return cdc_hash_int(CDC_TO_INT(key)); }

unsigned int GHash(const void *key)
{
  return static_cast<unsigned int>(cdc_hash_int(CDC_TO_INT(key)));
}

size_t CcHash(const void *key, int /* l */, uint32_t /* seed */)
{
  return cdc_hash_int(CDC_TO_INT(key));
}
