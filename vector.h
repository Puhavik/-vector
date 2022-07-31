#ifndef VECTOR_H
#define VECTOR_H
#include<initializer_list>
#include<iostream>
using namespace std;

template<typename T>
class Vector {

// Iterator

public:
  class Iterator;
  class ConstIterator;
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = Vector::Iterator;
  using const_iterator = Vector::ConstIterator;

// Vector

private:
  size_t sz;
  size_t max_sz;
  value_type *values;

public:
  Vector(){
    sz = 0;
    values = new value_type[10];
    max_sz = 10;
  };

  Vector(const Vector& copy){
    values = new value_type[10];
    max_sz = 10;
    sz = 0;
    for(int i=0; i<copy.size(); i++){
      push_back(copy[i]);
    }
  };

  Vector(size_t n) {
    if (n == 0) {
      values = new value_type[10];
      max_sz = 10;
    } else {
      values = new value_type[n];
      max_sz = n;
    }
    sz = 0;
  }

  Vector(initializer_list<value_type> list){
    int to_allocate = list.size();
    values = new value_type[to_allocate];
    max_sz = to_allocate;
    sz = 0;
    for(auto& elem : list ){
      values[sz++]=elem;
      //sz++; inside
    }
  };

  ~Vector(){
    delete[] values;
  };

  size_t size() const{
    return sz;
  };

  bool empty() const{
    return sz==0;
  };

  void clear(){
    sz = 0;
  };

  void reserve(size_t n){
      if (sz < max_sz) { return; }
      size_t to_reserve = n;
      value_type *buffer = new value_type[to_reserve];
      for(int i = 0; i < sz; i++){
        buffer[i] = values[i];
      }
      delete[] values;
      values = buffer;
      max_sz = to_reserve;
  };

  void shrink_to_fit(){
    max_sz = sz;
    value_type *buffer = new value_type[max_sz];
    for(int i = 0; i < sz; i++){
      buffer[i] = values[i];
    }
    delete[] values;
    values = buffer;
  };

  void push_back(value_type x){
    if(sz <= max_sz){
      reserve(max_sz*2);
    }
    values[sz++] = x;
  };

  void pop_back(){
    if(sz == 0){
      throw runtime_error("Leer Vector!");
    } else {
      sz--;
    }
  };

  value_type& operator[](size_t index){
    if(index < 0 || index > sz - 1){
      throw runtime_error ("Out of bounds not const!");
    }
    return values[index];
  };

  const value_type& operator[](size_t index) const{
    if(index < 0 || index > sz - 1){
      throw runtime_error ("Out of bounds!");
    }
    return values[index];
  };

  size_t capacity() const{
    return max_sz;
  };

   ostream& print(ostream& out)const{
     bool first{false};
     out << "[";
     for(int i = 0; i < sz; i++){
       if(!first){
         first=true;
       } else {
         out << ", ";
       }
       out << values[i];
     }
     out << "]" << endl;
     return out;
   };

   Vector& operator=(const Vector& v) {
     max_sz = v.max_sz;
     values = new value_type[max_sz];
     sz = 0;
     for (size_t i = 0; i < v.sz; i++) {
       values[sz++] = v.values[i];
     }
     return *this;
   };

//Iterators

class Iterator {
  public:
   using value_type = Vector::value_type;
   using difference_type = Vector::difference_type;
   using size_type = Vector::size_type;
   using reference = Vector::reference;
   using pointer = Vector::pointer;
   using iterator_category = std::forward_iterator_tag;

  private:
    pointer ptr;


  public:
    Iterator(){
      ptr = nullptr;
    };

    Iterator(pointer myptr){
      ptr = myptr;
    };

    reference operator*() const{
      return *ptr;
    };

    pointer operator->() const{
      return ptr;
    };

    pointer get_ptr(){
      return ptr;
    }

    bool operator==(const const_iterator& const_it) const{
      return ptr==const_it.get_ptr();
    };

    bool operator!=(const const_iterator& const_it) const{
      return ptr!=const_it.get_ptr();
    };

    iterator& operator++(){
      ptr++;
      return *this;
    };

    iterator operator++(int) {
      pointer ptr1{ptr};
      ++ptr;
      return ptr1;
    };

    Vector::difference_type operator-(const const_iterator& it) const{
      return ptr-it.get_ptr();
    };

    operator const_iterator() const{
      return ConstIterator(ptr);
    };
  };

  // ConstIterator

  class ConstIterator{
  public:
    using value_type = Vector::value_type;
    using reference = Vector::const_reference;
    using pointer = Vector::const_pointer;
    using difference_type = Vector::difference_type;
    using iterator_category = std::forward_iterator_tag;

  private:
    pointer ptr;

  public:
    ConstIterator(){
      ptr = nullptr;
    }
    ConstIterator(pointer my_ptr) {
      ptr = my_ptr;
    }

    pointer get_ptr() const {
      return ptr;
    };

    reference operator*() const{

      return *ptr;
    };

    pointer operator->() const{

      return ptr;
    };

    pointer get_ptr(){
      return ptr;
    }

    bool operator==(const const_iterator& const_it) const{
      return ptr==const_it.get_ptr();
    };

    bool operator!=(const const_iterator& const_it) const{
      return ptr!=const_it.get_ptr();
    };

    const_iterator& operator++(){

      ptr++;
      return *this;
    };

    const_iterator operator++(int) {
      pointer ptr1{ptr};
      ++ptr;
      return ptr1;
    };

    Vector::difference_type operator-(const const_iterator& it) const{
      return ptr-it.get_ptr();
    };
  };

  Iterator begin(){
    return Iterator(values);
  };

  Iterator end(){
    return Iterator(values+sz);
  };

  ConstIterator begin() const{
    return ConstIterator(values);
  };

  ConstIterator end() const {
    return ConstIterator(values+sz);
  };



  iterator insert(const_iterator pos, const_reference val) {
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) > sz) {
      throw std::runtime_error("Iterator out of bounds");
    }
    size_type current{static_cast<size_type>(diff)};
    if (sz >= max_sz) {
      reserve(max_sz*2);
    }
    for (auto i {sz}; i-->current;) {
      values[i + 1] = values[i];
    }
    values[current]=val;
    ++sz;
    return iterator{values+current};
  }

  iterator erase(const_iterator pos) {
    auto diff = pos - begin();
    if (diff<0 || static_cast<size_type>(diff)>=sz) {
      throw std::runtime_error("Iterator out of bounds");
    }
    size_type current{static_cast<size_type>(diff)};
    for (auto i{current}; i < sz-1; ++i) {
      values[i] = values[i+1];
    }
    --sz;
    return iterator{values+current};
  }


};

// Output

template <class T>
ostream& operator<<(ostream& out, const Vector<T>& v){
  return v.print(out);
};

#endif
