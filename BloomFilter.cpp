#include "BloomFilter.h"
#include "string"
#include "vector"
#include "IntegerHashes.h"
#include "StringHashes.h"

BloomFilter::BloomFilter(int k, int m, std::string strfn, std::string intfn){
  this -> k = k;
  this -> m = m;
  if(strfn == "jenkins"){
    this -> strfn = new JenkinsHash();
  }
  else if(strfn == "pearson"){
    this -> strfn = new PearsonHash();
  }
  if(intfn == "division"){
    this -> intfns = new IntegerHash*[k];
    for(int i = 0; i < k; i++){
      intfns[i] = new DivisionHash(i,m);
    }
  }
  else if(intfn == "reciprocal"){
    this -> intfns = new IntegerHash*[k];
    for(int i = 0; i < k; i++){
      intfns[i] = new ReciprocalHash(i,m);
    }
  }
  else if(intfn == "squareroot"){
    this -> intfns = new IntegerHash*[k];
    for(int i = 0; i < k; i++){
      intfns[i] = new SquareRootHash(i,m);
    }
  }
  this -> bits = new uint64_t[m];
}

BloomFilter::~BloomFilter(){
  delete []bits;
  delete strfn;
  for(int i = 0; i < k; i++){
    delete intfns[i];
  }
  delete []intfns;
}

void BloomFilter::insert(const std::string& value){
  uint64_t key;
  for(int i = 0; i < k; i++){
    key = intfns[i] -> hash(strfn -> hash(value));
    bits[key/64] |= (uint64_t(1) << (key % 64));
  }
}

bool BloomFilter::lookup(const std::string& value) const{
  uint64_t key;
  for(int i = 0; i < k; i++){
    key = intfns[i] -> hash(strfn -> hash(value));
    if((bits[key/64] & (uint64_t(1) << (key % 64))) != 1){
      return false;
    }
  }
  return true;
}


