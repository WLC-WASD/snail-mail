

/*SINGLELY-LINKED-LIST (SLL) */

/*Description:
A templated singely linked list class.
Impleted as a FIFO queue*/

/* Note: Use 'nullptr' not 'NULL' */

template <class T>
class Node{
  template <class t>friend class Sll;
public:
  Node(T);
private:
  T element;
  Node* next;
};

template <class T>
Node<T>::Node(T element){
  this->element = element;
}

template <class T>
class Sll{
public:
  Sll();
  ~Sll();
  void insert(T);
  bool remove(T&);
  T peek();
private:
  Node<T>* head;
};

template <class T>
T Sll<T>::peek(){
  return head->element;
}

template <class T>
Sll<T>::Sll(){
  head = nullptr;
}

template <class T>
Sll<T>::~Sll(){
  T junk;
  while(remove(junk));
}

template <class T>
void Sll<T>::insert(T element){
  Node<T>* n = new Node<T>(element);
  Node<T> *p, *q;
  p = head;
  q = nullptr;
  q = p;
  if(q != nullptr) p = q->next;
 
  if(q == nullptr){ //if empty
    n->next = p;
    head = n;
  }else{
    n->next = q->next;
    q->next = n;
  }
}

template <class T>
bool Sll<T>::remove(T& target){
  if(head == nullptr) return false;
  Node<T> *n = head; //temp
  target = n->element; //return value
  head = n->next; //sets head to nullptr if last element
  delete n;
  return true;
}
