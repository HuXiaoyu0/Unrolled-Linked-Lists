#ifndef unrolled_h_
#define unrolled_h_
// A simplified implementation of a generic list container class,
// including the iterator, but not the const_iterators.  Three
// separate classes are defined: a Node class, an iterator class, and
// the actual list class.  The underlying list is doubly-linked, but
// there is no dummy head node and the list is not circular.

#include <cassert>

// The number of elements stored in each node
const int NUM_ELEMENTS_PER_NODE = 6;


// -----------------------------------------------------------------
// NODE CLASS
template <class T>

class Node {

public:
  Node() : next_(NULL), prev_(NULL) {num_elements_ = 0;}

  // REPRESENTATION
  T elements_[NUM_ELEMENTS_PER_NODE];
  Node<T>* next_;
  Node<T>* prev_;
  unsigned int num_elements_;
};

// A "forward declaration" of this class is needed
template <class T> class UnrolledLL;

// -----------------------------------------------------------------
// LIST ITERATOR
template <class T>

class list_iterator {

public:
  list_iterator() : ptr_(NULL) {offset_ =0;}
  list_iterator(Node<T>* p, int j) {ptr_ = p; offset_ = j;}
  list_iterator(list_iterator<T> const& old) : ptr_(old.ptr_) {}
  ~list_iterator() {}

  list_iterator<T> & operator=(const list_iterator<T> & old) { 
    ptr_ = old.ptr_; offset_ = old.offset_; return *this; }

  // dereferencing operator gives access to the value at the pointer
  T& operator*()  { return ptr_->elements_[offset_];  }

  // increment & decrement operators
  list_iterator<T> & operator++() { // pre-increment, e.g., ++iter
    offset_++;
	if (offset_ >= ptr_->num_elements_ )
	{ptr_ = ptr_->next_; offset_ = 0;}
    return *this;
  }
  list_iterator<T> operator++(int) { // post-increment, e.g., iter++
    list_iterator<T> temp(*this);
    ++(*this);
    return temp;
  }
  // May not totally right!!!
  list_iterator<T> & operator--() { // pre-decrement, e.g., --iter
    offset_--;
	if (offset_<0)
	{ptr_ = ptr_->prev_; offset_ = ptr_->num_elements_ - 1;}
    return *this;
  }
  list_iterator<T> operator--(int) { // post-decrement, e.g., iter--
    list_iterator<T> temp(*this);
    --(*this);
    return temp;
  }

  friend class UnrolledLL<T>;

  // Comparions operators are straightforward
  bool operator==(const list_iterator<T>& r) const {
    return (ptr_ == r.ptr_ && offset_ == r.offset_); }
  bool operator!=(const list_iterator<T>& r) const {
    return (ptr_ != r.ptr_ || offset_  != r.offset_); }
  //=========================================================
  //this part only used for debug purpose
  int get(){return offset_;}
  int getnum(){return ptr_->num_elements_;}
  //=========================================================
private:
  // REPRESENTATION
  Node<T>* ptr_;    // ptr to node in the list
  int offset_;    //Indicate which element to be access

};

// -----------------------------------------------------------------
// LIST CLASS DECLARATION
// Note that it explicitly maintains the size of the list.
template <class T>

class UnrolledLL {

public:
  UnrolledLL() : head_(NULL), tail_(NULL), size_(0) {}
  UnrolledLL(const UnrolledLL<T>& old) { this->copy_list(old); }
  ~UnrolledLL() { this->destroy_list(); }
  UnrolledLL& operator= (const UnrolledLL<T>& old);

  unsigned int size() const { return size_; }
  bool empty() const { return head_ == NULL; }
  void clear() { this->destroy_list(); }
  void print(std::ostream&);
   
  void push_front(const int& v);
  void pop_front();
  void push_back(const int& v);
  void pop_back();

  const T& front() const { return head_->elements_;  }
  T& front() { return head_->elements_[0]; }
  const T& back() const { return tail_->elements_; }
  T& back() { return tail_->elements_[tail_->num_elements_-1]; }

  typedef list_iterator<T> iterator;
  iterator erase(iterator& itr);
  iterator insert(iterator& itr, T const& v);
  iterator begin() { return iterator(head_,0);}
  iterator end() { return iterator(NULL,0); }

private:
  void copy_list(UnrolledLL<T> const & old);
  void destroy_list();

  //REPRESENTATION
  Node<T>* head_;
  Node<T>* tail_;
  unsigned int size_;
};




// -----------------------------------------------------------------
// LIST CLASS IMPLEMENTATION
template <class T>
UnrolledLL<T>& UnrolledLL<T>::operator= (const UnrolledLL<T>& old) {
  if (&old != this) {
    this->destroy_list();
    this->copy_list(old);
  }
  return *this;
}

//*****************************************
//print function
template <class T>
void UnrolledLL<T>::print(std::ostream&){
	if (this->head_ != NULL){
	typename UnrolledLL<T>::iterator ite = this->begin();
	int j = 0;
	std::cout<<"UnrolledLL, size: "<<size_<<std::endl;
	std::cout<<" node:["<<ite.ptr_->num_elements_<<"]";
	for (; ite != this->end(); ite++){
		std::cout<<" "<<*ite; 
		
		j++;
		if (j == ite.ptr_->num_elements_){
		std::cout<<std::endl;
		if (ite.ptr_->next_ != NULL)
		std::cout<<" node:["<<ite.ptr_->next_->num_elements_<<"]";
		j = 0;
	  }
	}
}
	else
std::cout<<"UnrolledLL, size: "<<size_<<std::endl;
}
//*****************************************

template <class T> 
void UnrolledLL<T>::push_back(const int& v) {
 
  // special case:  initially empty list
  if (head_ == NULL) {
	Node<T>* newp = new Node<T>;
    head_ = tail_ = newp;
	head_->elements_[0] = v;
	head_->num_elements_++;
  }
  // normal case:  at least one node already
  else {
	  if (tail_->num_elements_ < NUM_ELEMENTS_PER_NODE)
		  {tail_->elements_[tail_->num_elements_] = v;
	       tail_->num_elements_++;}
	  else if (tail_->num_elements_ == NUM_ELEMENTS_PER_NODE){
		  Node<T>* newp = new Node<T>;
		  newp -> prev_ = tail_;
		  tail_ -> next_ = newp;
		  tail_ = newp;
		  tail_->elements_[0] = v;
		  tail_->num_elements_++;
	  }
  }
  ++size_;
}

template <class T> 
void UnrolledLL<T>::push_front(const int& v) {
	 // special case:  initially empty list
  if (head_ == NULL) {
	Node<T>* newp = new Node<T>;
    head_ = tail_ = newp;
	head_->elements_[0] = v;
	head_->num_elements_++;
  }
  // normal case:  at least one node already
  else {
	  if (head_->num_elements_ < NUM_ELEMENTS_PER_NODE){
		  for (unsigned int j = 0; j < head_->num_elements_; j++)
		  head_->elements_[head_->num_elements_-j] = head_->elements_[head_->num_elements_-j-1];
		  head_->elements_[0] = v;
		  head_->num_elements_++;
	  }
	  else if (head_->num_elements_ == NUM_ELEMENTS_PER_NODE){
		  Node<T>* newp = new Node<T>;
		  newp -> next_ = head_;
		  head_ -> prev_ = newp;
		  head_ = newp;
		  head_->elements_[0] = v;
		  head_->num_elements_++;
	  }
  }
  ++size_;
}


template <class T> 
void UnrolledLL<T>::pop_back() {
	if (head_ == NULL)
		std::cerr<<"Can not pop back a empty unrolled list"<<std::endl;
	else{
		if (head_ == tail_)
		{if (head_->num_elements_ == 1)//the node will go empty, delete shall be called
		{delete head_;
		head_ = NULL;
		tail_ = NULL;}
		else
		{head_->elements_[head_->num_elements_] = 0;
		head_->num_elements_--;}
		}
		else{
		if (tail_->num_elements_ == 1)
		{Node<T>* temp = tail_;
		tail_ = tail_->prev_;
		tail_->next_ = NULL;
		delete temp;
		}
		else
		{tail_->elements_[tail_->num_elements_] = 0; tail_->num_elements_--;}
		}
	}
--size_;
}


template <class T> 
void UnrolledLL<T>::pop_front() {
	if (head_ == NULL)
		std::cerr<<"Can not pop front a empty unrolled list"<<std::endl;
	else{
		if (head_ == tail_)
		{if (head_->num_elements_ == 1)//the node will go empty, delete shall be called
		{delete head_;
		head_ = NULL;
		tail_ = NULL;}
		else
		{for (int j = 0; j< head_->num_elements_-1; j++) 
			head_->elements_[j] = head_->elements_[j+1];
		    head_->elements_[head_->num_elements_-1] = 0;
			head_->num_elements_--;
			}
		}
		else{
		if (head_->num_elements_ == 1)//the node will go empty, delete shall be called
		{Node<T>* temp = head_;
		head_ = head_->next_;
		delete temp;
		}
		else
		{for (int j = 0; j< head_->num_elements_-1; j++) 
			head_->elements_[j] = head_->elements_[j+1];
		    head_->elements_[head_->num_elements_-1] = 0;
			head_->num_elements_--;}
	}
}
--size_;
}

//erase a element and return to the iterator of that position
template <class T> 
typename UnrolledLL<T>::iterator UnrolledLL<T>::erase(iterator& itr) {
	assert (size_ > 0);
    size_--;
  
  if (itr.ptr_->num_elements_ == 1){//the node will go empty, delete shall be called
  if (head_ == tail_){
	  delete head_;
  head_ = NULL;
  tail_ = NULL;
  return iterator(NULL, 0);}
  else if (itr.ptr_->prev_ == NULL){
	  head_ = head_->next_;
	  itr.ptr_->next_->prev_ = NULL;
	  delete itr.ptr_;
	  return iterator(head_,0);
  }
  else if (itr.ptr_->next_ == NULL){
	  tail_ = tail_->prev_;
	  itr.ptr_->prev_->next_ = NULL;
	  delete itr.ptr_;
	  return iterator(NULL, 0);
  }
  else{
	  Node<T>* tmp = itr.ptr_->next_;
	  itr.ptr_->prev_->next_ = itr.ptr_->next_;
	  itr.ptr_->next_->prev_ = itr.ptr_->prev_;
	  delete itr.ptr_;
	  return iterator(tmp, 0);
  }

  }
  else{//normal case
	  for (int j = itr.offset_; j<itr.ptr_->num_elements_-1;j++)
		  itr.ptr_->elements_[j] = itr.ptr_->elements_[j+1];
	  itr.ptr_->elements_[itr.ptr_->num_elements_-1] = 0;
	  itr.ptr_->num_elements_--;
  }
  if (itr.offset_ >= itr.ptr_->num_elements_)
	  return iterator (itr.ptr_->next_, 0);
  else
      return iterator(itr.ptr_, itr.offset_);
}

// insert BEFORE the node indicated by the iterator and return an iterator to the new node
template <class T> 
typename UnrolledLL<T>::iterator UnrolledLL<T>::insert(iterator& itr, T const& v) {
  ++size_ ;
  if (itr.offset_ ==0){
	  //under this ccondition you have to decide which node to go, 
	  //the previous one or the current one
	  if (itr.ptr_->prev_ ==NULL && itr.ptr_->num_elements_ == NUM_ELEMENTS_PER_NODE)//overflows
	  {Node<T>* p = new Node<T>;
	  p->next_ = head_;
	  head_->prev_ = p;
	  head_ = p;
	  p->elements_[0] = v;
	  p->num_elements_++;
	  return iterator(p,0);
      }
	  else if (itr.ptr_->prev_ !=NULL && itr.ptr_->num_elements_ == NUM_ELEMENTS_PER_NODE){//overflows
		  Node<T>* p = new Node<T>;
		  itr.ptr_->prev_->next_ = p;
          p -> prev_ = itr.ptr_ -> prev_;
          p -> next_ = itr.ptr_;
          itr.ptr_ -> prev_ = p;
		  p->elements_[0] = v;
		  p->num_elements_++;
		  return iterator(p,0);
		  }
	  else{//normal case
		  itr.ptr_->num_elements_++;
		  for (int j = itr.ptr_->num_elements_-1; j>=1; j--)
			  itr.ptr_->elements_[j] = itr.ptr_->elements_[j-1];
		  itr.ptr_->elements_[0] = v;

		  
		  return iterator(itr.ptr_, 0);
		  }	
  }
  else{
	  //under this ccondition you have to decide which node to go, 
	  //the next one or the current one
	  if (itr.ptr_->num_elements_ == NUM_ELEMENTS_PER_NODE){//overflows
		  Node<T>* p = new Node<T>;
          if (itr.ptr_->next_ == NULL){
		  itr.ptr_->next_ = p;
		  p->prev_ = itr.ptr_;
		  tail_ = p;
		  p->elements_[0] = itr.ptr_->elements_[NUM_ELEMENTS_PER_NODE];
		  p->num_elements_++;
		  itr.ptr_->elements_[itr.offset_] = v;
		  for (int j =itr.offset_+1; j<NUM_ELEMENTS_PER_NODE-1; j++)
			  itr.ptr_->elements_[j]=itr.ptr_->elements_[j-1];
		  return iterator(itr.ptr_, itr.offset_);
		  }
		  else{
			  for (int j = itr.offset_; j<NUM_ELEMENTS_PER_NODE; j++){//normal case
				  p->elements_[j-itr.offset_] = itr.ptr_->elements_[j];
				  p->num_elements_++;
				  itr.ptr_->elements_[j] = 0;
				  itr.ptr_->num_elements_--;
			  }
			  itr.ptr_->elements_[itr.offset_] = v;
			  itr.ptr_->num_elements_++;
			  
			  p->next_ = itr.ptr_->next_;
			  p->prev_ = itr.ptr_;
			  itr.ptr_->next_->prev_ = p;
			  itr.ptr_->next_ = p;
			  
			  return iterator(itr.ptr_, itr.offset_);
		  }
	  }
	  else{
	  
	  itr.ptr_->num_elements_++;
	  for (int j =itr.ptr_->num_elements_-1; j>itr.offset_; j--)
			  itr.ptr_->elements_[j]=itr.ptr_->elements_[j-1];
	  itr.ptr_->elements_[itr.offset_] = v;
	  return iterator(itr.ptr_, itr.offset_);
	  }
  }
}

//clear all the elements in the structure and free the memory
template <class T>
void UnrolledLL<T>::destroy_list() {
	Node<T>* temp;
	if (head_ != NULL){
	temp = this->head_;
	this->head_ = this->head_->next_;
	delete temp;
	destroy_list();
	}
	delete this->head_;
	size_ = 0;
}

#endif
