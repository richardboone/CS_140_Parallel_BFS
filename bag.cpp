#include "bag.h"
#include <iostream>

using namespace std;

Bag::Bag() {
  count = 0;
  bagsize = 20;
  data = new int[bagsize];
}

Bag::Bag(int size) {
  count = 0;
  bagsize = size;
  data = new int[bagsize];
}

Bag::Bag(Bag& copy) {
  /*
  memcpy(data, copy, copy.size());
  count = copy.size();
  bagsize = copy.bagsize;
  */
  data = new int[copy.bagsize]; 
  for(int i = 0; i < copy.count; i++) 
    data[i] = copy.data[i]; 

  count = copy.count; 
  bagsize = copy.bagsize; 
}

Bag::~Bag() {
  if (data != NULL)
    delete [] data;
}

void Bag::insert(int n) {
  if (count == bagsize)
    resize();

  data[count] = n;
  count++;
}

void Bag::remove(int n) {
  if(howmany(n) == 0)
    return;
 
  int index = 0;
  while(data[index] != n) 
    index++; 
  for(int i = index; i < count; i++) 
    data[i] = data[i+1];
 
  count--; 
}

int Bag::getElement(int index) {
  return data[index];
}

int Bag::size() {
  return count;
}

void Bag::clear() {
  count = 0;
}

bool Bag::inbag(int n) {
  for (int i = 0; i < count; i++) {
    if (data[i] == n) {
      return true;
    }
  }
  return false;
}

int Bag::howmany(int n) {
  int num = 0;
  for (int i = 0; i < count; i++) {
    if (data[i] == n) {
      num++;
    }
  }
  return num;
}

void Bag::resize() {
  int newsize = bagsize + bagsize;
  int *newdata;

  newdata = new int[newsize];  
  /*
  memcpy(newdata, olddata, oldata.size());
  delete [] data;
  data = newdata;
  bagsize = newsize;
  */
  for(int i = 0; i < bagsize; i++) 
    newdata[i] = data[i]; 

  delete [] data; 

  data = newdata; 
  bagsize = newsize;
}

Bag Bag::add(Bag in) {
  /*
  Bag temp(this.size() + in.size());
  memcpy(temp, this, this->size());
  memcpy(temp[this->size()-1], in.data, in.size());
  temp = this->size() + in.size();
  return temp;
  */

  Bag temp(size() + in.size());
  int i = 0;
  while(i < count) {
    temp.insert(data[i]);
    i++; 
  } 
  i = 0; 
  while(i < in.count) { 
    temp.insert(in.data[i]);
    i++; 
  } 
  return temp; 
}

bool Bag::isEmpty() {
  return (count == 0);
}

Bag Bag::operator+(Bag in) {
  Bag temp(size() + in.size());
  temp.count = count + in.count;
  int i = 0;
  while(i < count) {
    temp.insert(data[i]);
    i++;
  }
  i = 0;
  while(i < in.count) {
    temp.insert(in.data[i]);
    i++;
  }
  return temp;
}

Bag& Bag::operator=(Bag in) {
  if (data != NULL) {
    //cout << "deleting" << endl;
    delete [] data;
    //cout << "deleted" << endl;
  }
  count = 0;
  //cout << "assigning new" << endl;
  if (in.count != 0) {
    data = new int[in.count];
    //cout << "assigned" << endl;
    for(int i = 0; i < in.count; i++) { 
      data[i] = in.data[i];
      count++;
    }
  }
  //this->insert(in.data[i]);
    
  cout << "bagsize: " << size() << endl;
  return *this;
}
