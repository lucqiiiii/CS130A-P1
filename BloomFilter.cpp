#include "BloomFilter.h"
#include "string"
#include "vector"
#include "IntegerHashes.h"
#include "StringHashes.h"

BloomFilter::BloomFilter(int k, int m, string strfn, string intfn){
  this -> k = k;
  this -> m = m;
  if(strfn == "jenkins"){
    this -> strfn = new JenkinsHash();
  }
  else if(strfn == "pearson"){
    this -> strfn = new PearsonHash();
  }
  if(intfn == "division"){
    this -> intfn = new DivisionHash();
  }
  else if(intfn == "reciprocal"){
    this -> intfn = new ReciprocalHash();
  }
  else if(intfn == "squareroot"){
    this -> intfn = new SquareRootHash();
  }
  bits = new vector<uint64_t>(m,0);
}

BloomFilter::~BloomFilter(){
  delete []bits;
  delete strfn;
  delete intfn;
}

void BloomFilter::insert(const string& value){
  uint64_t key = intfn -> hash(strfn -> hash(value));
  bits[key/64] |= (1 << (key % 64));
}

bool BloomFilter::lookup(const string& value) const{
  uint64_t key = intfn -> hash(strfn -> hash(value));
  if((bits[key/64] &= (1 << (key % 64))) == 1){
    return true;
  }
  return false;
}


