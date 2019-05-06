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
    this -> intfn = new DivisionHash*[k];
    for(int i = 0; i < k; i++){
      intfn[i] = new DivisionHash(i,m);
    }
  }
  else if(intfn == "reciprocal"){
    this -> intfn = new ReciprocalHash*[k];
    for(int i = 0; i < k; i++){
      intfn[i] = new ReciprocalHash(i,m);
    }
  }
  else if(intfn == "squareroot"){
    this -> intfn = new SquareRootHash*[k];
    for(int i = 0; i < k; i++){
      intfn[i] = new SquareRootHash(i,m);
    }
  }
  bits = new vector<uint64_t>(m,0);
}

BloomFilter::~BloomFilter(){
  delete []bits;
  delete strfn;
  delete [](*intfn);
  delete intfn;
}

void BloomFilter::insert(const string& value){
  uint64_t key;
  for(int i = 0; i < k; i++){
    key = intfn[i] -> hash(strfn -> hash(value));
    bits[key/64] |= (1 << (key % 64));
  }
}

bool BloomFilter::lookup(const string& value) const{
  uint64_t key;
  for(int i = 0; i < k; i++){
    key = intfn[i] -> hash(strfn -> hash(value));
    if((bits[key/64] &= (1 << (key % 64))) != 1){
      return false;
    }
  }
  return true;
}


