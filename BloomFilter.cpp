#include "BloomFilter.h"
#include "string"
#include "vector"


BloomFilter::BloomFilter(int k, int m, string strfn, string intfn){
  this -> k = k;
  this -> m = m;
  this -> strfn = strfn;
  this -> intfn = intfn;
  bits = new vector<uint64_t>[m];
  for(int i = 0; i < m; i++){
    bits[i] = 0;
  }
}

BloomFilter::~BloomFilter(){
  delete []bits;

}

void BloomFilter::insert(const string& value){
  int key = intfn::hash(strfn::hash(value));
  bits[key] = 1;
  
}

bool BloomFilter::lookup(const string& value) const{


}


