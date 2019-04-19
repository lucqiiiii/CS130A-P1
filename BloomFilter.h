#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include "HashFunctions.h"
#include <cstdint>
#include <string>

class BloomFilter {
  int k; // The number of hash functions
  int m; // The number or bits

  HashFunction** hashfns;
  uint64_t*      bits;

public:
  BloomFilter(int k, int m);
  ~BloomFilter();

  void insert(const std::string& value);
  bool lookup(const std::string& value) const;
};

#endif
