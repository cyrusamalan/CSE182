// Cyrus Amalan, camalan, pa5

#include <stdexcept>
#include "List.h"
using namespace std;


List::Node::Node(ListElement x) {
    data = x;
    next = nullptr;
    prev = nullptr;
}

// Creates new List in the empty state.
List::List(){
    frontDummy = new Node(0);
    backDummy = new Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
}

List::List(const List& L) { 
    frontDummy = new Node(0);
    backDummy = new Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
    Node *new_N = L.frontDummy->next;
    while (new_N != L.backDummy) {
        this->insertAfter(new_N->data);
        moveNext();
        new_N = new_N->next;
    }
    moveFront();
}

List::~List(){
    this->clear();
    delete this->backDummy;
    delete this->frontDummy;
}

// Returns the length of this List.
int List::length() const{
    return this->num_elements;
}

ListElement List::front() const{
    if(this->length() <= 0){
        std::cerr << "pre: length()>0" << "\n";
        cerr.flush();
    }
    return frontDummy->next->data;
}

ListElement List::back() const{
    if(this->length() <= 0){
        std::cerr << "pre: length()>0" << "\n";
        cerr.flush();
    }
    return backDummy->prev->data;
}

int List::position() const{
    return this->pos_cursor;
}

ListElement List::peekNext() const {
   if (this->pos_cursor >= length()) {
      throw length_error("pre: position()<length()");
   }
   return afterCursor->data;
}

ListElement List::peekPrev() const {
   if (this->pos_cursor <= 0) {
      throw length_error("pre: position()>0");
   }
   return beforeCursor->data;
}

void List::clear(){
    this->moveFront();
    while(this->length() > 0){
        this->eraseAfter();
    }
}

void List::moveFront(){
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    pos_cursor = 0;
}

void List::moveBack(){
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
    pos_cursor = num_elements;
}

ListElement List::moveNext(){
    if(this->position() >= this->length()){
        throw length_error("pre: position()<length()\n");
    }
    beforeCursor = beforeCursor->next;
    afterCursor = afterCursor->next;
    pos_cursor++;
    return beforeCursor->data;
}

ListElement List::movePrev(){
    if(this->position() <= 0){
        throw length_error("pre: position()>0\n");
    }
    beforeCursor = beforeCursor->prev;
    afterCursor = afterCursor->prev;
    pos_cursor--;
    return afterCursor->data;
}

void List::insertAfter(ListElement x){
    Node* new_n = new Node(x);
    new_n->prev = beforeCursor;
    new_n->next = afterCursor;
    beforeCursor->next = new_n;
    afterCursor->prev = new_n;
    afterCursor = new_n;
    this->num_elements++;
}

void List::insertBefore(ListElement x) {
    if (pos_cursor < 0) {
        exit(EXIT_FAILURE);
    }
    if (afterCursor == nullptr || beforeCursor == nullptr) {
        exit(EXIT_FAILURE);
    }
    Node *copy = new Node(x);

    copy->prev = beforeCursor;
    beforeCursor->next = copy;
    copy->next = afterCursor;
    afterCursor->prev = copy;
    beforeCursor = copy;

    pos_cursor++;
    num_elements++;
}

void List::setAfter(ListElement x){
    if(position() > length()){
       throw length_error("pre: position()<length()\n");
    }
    afterCursor->next->data = x;
}

void List::setBefore(ListElement x){
    if(position() < 0){
        throw length_error("pre: position()>0\n");
    }
    beforeCursor->prev->data = x;
}

void List::eraseAfter(){
    if(position() > length()){
        throw length_error("pre: position()<length()\n");
    }
    Node* removedNode = afterCursor;
    afterCursor = afterCursor->next;
    beforeCursor->next = afterCursor;
    afterCursor->prev = beforeCursor;
    this->num_elements--;
    delete removedNode;
}

void List::eraseBefore(){
    if(position() < 0){
        throw length_error("pre: position()>0\n");
    }
    Node* removedNode = beforeCursor;
    beforeCursor = beforeCursor->prev;
    beforeCursor->next = afterCursor;
    afterCursor->prev = beforeCursor;
    this->pos_cursor--;
    this->num_elements--;
    delete removedNode;
}


int List::findNext(ListElement x){
    Node* curr_Node = afterCursor;
    int i = pos_cursor; //keeping track of the index
    //loop through the list
    while(curr_Node != backDummy){
        if(curr_Node->data == x){
            //change cursor position and the pointers next to it
            beforeCursor = curr_Node;
            afterCursor = curr_Node->next;
            pos_cursor = i + 1; //update the position

            return pos_cursor; //return new updated position
        }
        curr_Node = curr_Node->next; //next node
        ++i; //keeping track of index for next iteration
    }
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
    pos_cursor = num_elements;
    
    return -1; //indicating that element was not found
}

int List::findPrev(ListElement x){
    Node* searchNode = afterCursor->prev;
    int i = pos_cursor - 1;
    //iterate until frontDummy is reached
    while(searchNode != frontDummy){
        if(searchNode->data == x){
            beforeCursor = searchNode->prev;
            afterCursor = searchNode;
            pos_cursor = i;

            return i; //return 'x' position
        }
        searchNode = searchNode->prev;
        i--;
    }
    searchNode = frontDummy->next;
    beforeCursor = frontDummy;
    pos_cursor = 0;
        
    return -1; //indicates that 'x' is not found
}

void List::cleanup() {
   Node *cursor = frontDummy->next;
   int index = 0;
   while (cursor != backDummy) {
      Node *curr = cursor;
      Node *new_N = cursor->next;
      int inner = index + 1;
      while (new_N != backDummy) {
         if (cursor->data == new_N->data) {
               curr->next = new_N->next;
               new_N->next->prev = curr;
               delete new_N;
               new_N = curr->next;
               num_elements--;
               if (inner <= pos_cursor) {
                  pos_cursor--;
               }
         }
         else {
               curr = new_N;
               new_N = curr->next;                
         }
         inner++;
      }
      index++;
      cursor = cursor->next;
   }
}


List List::concat(const List& L) const {
    List newList;
    // Use a for loop for a clearer iteration over the current list
    for (Node* N = this->frontDummy->next; N != this->backDummy; N = N->next) {
        newList.insertAfter(N->data); // Insert data from the current list
    }
    // Use a for loop for iteration over the passed list
    for (Node* M = L.frontDummy->next; M != L.backDummy; M = M->next) {
        newList.insertAfter(M->data); // Insert data from L
    }

    return newList;
}


std::string List::to_string() const {
    std::string s;
    // Start from the first actual element, skip the dummy
    for (Node* N = frontDummy->next; N != backDummy; N = N->next) {
        s += std::to_string(N->data) + " ";
    }
    return s;
}


bool List::equals(const List& R) const {
    if (num_elements != R.num_elements) {
        return false;
    }

    Node* start = this->frontDummy->next; // Start from the first actual element
    Node* M = R.frontDummy->next;

    while (start != this->backDummy && M != R.backDummy) {
        if (start->data != M->data) {
            return false;
        }
        start = start->next;
        M = M->next;
    }

    return true; // If all elements matched
}


std::ostream& operator<<(std::ostream& stream, const List& L) {
    // Iterate directly from the node after frontDummy up to, but not including, backDummy.
    for (List::Node* curr = L.frontDummy->next; curr != L.backDummy; curr = curr->next) {
        stream << curr->data << " ";
    }
    return stream;
}


bool operator==( const List& A, const List& B ) {
   return A.List::equals(B);
} 


List& List::operator=(const List& L) {
    if (this != &L) {
      List copy = L;
      swap(beforeCursor, copy.beforeCursor);
      swap(backDummy, copy.backDummy);
      swap(pos_cursor, copy.pos_cursor);
      swap(frontDummy, copy.frontDummy);
      swap(afterCursor, copy.afterCursor);
      swap(num_elements, copy.num_elements);
    }
    return *this;
}



