//Huihong Zheng
//csci335 Hw#4
//Double Hashing implementation
#ifndef DoubleHashing_H
#define DoubleHashing_H

#include <vector>
#include <algorithm>
#include <functional>



template <typename HashedObj>
class  HashTableDouble {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit  HashTableDouble(size_t size = 101) : array_(NextPrime(size))
    { MakeEmpty(); }
    
  double CurrentElementSize()
  {
  	return current_size_;      
  }    
  
  double TheHashTableSize()
  {
  	int StoreTable = array_.size();            
  	return StoreTable;        
  }   
  
  double TotalCollisionsProbing()
  {
  	return TotalCollisions;          
  }   
  
  void ToMakeEmptyCollision() 
  {
  	TotalCollisions =0;     
  }   
  
  void SetRValue( size_t  RValue)
  {
  	R = RValue;         
  }    
  
  bool Contains(const HashedObj & x) const 
  {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(HashedObj && x) {
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }


 private:        
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    

  std::vector<HashEntry> array_;
  size_t current_size_;
  mutable size_t TotalCollisions;                 
  size_t R;                       

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  size_t FindPos(const HashedObj & x) const 
  {
    size_t offset = 1;        
    size_t current_pos = InternalHash(x);     
      
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x)                     
    {
      int GetTableSize = array_.size();
      current_pos = (current_pos + offset * TheHashPart2(x))% GetTableSize;          
      TotalCollisions = TotalCollisions +1;        
      offset++;        
    }
    return current_pos;    
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    array_.resize(NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }
  
  size_t InternalHash(const HashedObj & x) const 
  {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }
  
  size_t TheHashPart2(const HashedObj & x) const 
  {
    static std::hash<HashedObj> hf;                
    return (R - (hf(x)% R));        
  }    
};

#endif   
