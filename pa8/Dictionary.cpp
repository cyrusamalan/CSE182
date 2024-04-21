//Cyrus Amalan, camalan, pa8

#include <stdexcept>

#include "Dictionary.h"

#define RED -1
#define BLACK -2

using namespace std;

Dictionary::Node::Node(keyType k, valType v)
    : key(k), val(v), parent(nullptr), left(nullptr), right(nullptr) {}

Dictionary::Dictionary() : nil(new Node("\0", -1)), root(nil), current(nil), num_pairs(0) {
    nil->color = BLACK;
}

Dictionary::Dictionary(const Dictionary& D) : nil(new Node("\0", -1)), root(nil), current(nil), num_pairs(0) {
    preOrderCopy(D.root, D.nil);
    nil->color = BLACK; 
}

Dictionary::~Dictionary() {
    postOrderDelete(root);
    delete nil;
}


void Dictionary::inOrderString(std::string& s, Node* R) const{
    if(R == nil){
        return;
    }
    inOrderString(s, R->left);
    s += R->key + " : " + std::to_string(R->val) + "\n";
    inOrderString(s, R->right);
}

void Dictionary::preOrderString(std::string& s, Node* R) const {
    if (R == nil) return;
    s += R->key;
    if (R->color == RED) s += " (RED)";
    s += "\n";
    preOrderString(s, R->left);
    preOrderString(s, R->right);
}

void Dictionary::preOrderCopy(Node* R, Node* N) {
    if (R != N) {
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

void Dictionary::postOrderDelete(Node* R) {
    if (R != nil) {
        postOrderDelete(R->right);
        postOrderDelete(R->left);
        delete R;
    }
}

Dictionary::Node* Dictionary::search(Node* R, keyType k)const{
    while(k != R->key && R != nil){
        if(R->key > k){
            R = R->left;
        }else{
            R = R->right;
        }
        
    }
    return R;
}

Dictionary::Node* Dictionary::findMin(Node* R){
    while(R->left != nil){
        R = R->left;
    }
    return R;
}

Dictionary::Node* Dictionary::findMax(Node* R){
    while(R->right != nil){
        R = R->right;
    }
    return R;
}

Dictionary::Node* Dictionary::findPrev(Node* N){
    if(N->left != nil){
        return findMax(N->left);
    }
    Node* NodeParent = N->parent;
    while (N == NodeParent->left && NodeParent != nil){
        N = NodeParent;
        NodeParent = NodeParent->parent;
    }
    return NodeParent;
}

Dictionary::Node* Dictionary::findNext(Node* N){
    if(N->right != nil){
        return findMin(N->right);
    }
    Node* NodeParent = N->parent;
    while (N == NodeParent->right && NodeParent != nil){
        N = NodeParent;
        NodeParent = NodeParent->parent;
    }
    return NodeParent;
}


void Dictionary::LeftRotate(Node* N) {
    Node* rotate = N->right;
    if (rotate != nil) {
        N->right = rotate->left;
        if (rotate->left != nil) rotate->left->parent = N;
        rotate->parent = N->parent;
    }
    if (N->parent == nil) {
        root = rotate;
    } else if (N == N->parent->left) {
        N->parent->left = rotate;
    } else {
        N->parent->right = rotate;
    }

    if (rotate != nil) {
        rotate->left = N;
    }
    N->parent = rotate;
}

void Dictionary::RightRotate(Node* N) {
    Node* Y = N->left;

    // Make left child be right child
    N->left = Y->right;
    if (Y->right != nil) {
        Y->right->parent = N;
    }

    // Link Y to the parent of N
    Y->parent = N->parent;
    if (N->parent == nil) { // N is the root
        root = Y;
    } else { // N was not the root
        if (N == N->parent->right) { // Right child is N
            N->parent->right = Y;
        } else { // Left child is N
            N->parent->left = Y;
        }
    }

    // N is on the right
    Y->right = N;
    N->parent = Y;
}

void Dictionary::RB_InsertFixUp(Node* N) {
    Node *Uncle, *Grandparent;

    while (N->parent->color == RED && N != root) {
        Grandparent = N->parent->parent;
        if (N->parent == Grandparent->left) {
            Uncle = Grandparent->right;
            if (Uncle->color == RED) {
                N->parent->color = BLACK;
                Uncle->color = BLACK;
                Grandparent->color = RED;
                N = Grandparent;
            } else {
                if (N == N->parent->right) {
                    N = N->parent;
                    LeftRotate(N);
                }
                N->parent->color = BLACK;
                Grandparent->color = RED;
                RightRotate(Grandparent);
            }
        } else {
            Uncle = Grandparent->left;
            if (Uncle->color == RED) {
                N->parent->color = BLACK;
                Uncle->color = BLACK;
                Grandparent->color = RED;
                N = Grandparent;
            } else {
                if (N == N->parent->left) {
                    N = N->parent;
                    RightRotate(N);
                }
                N->parent->color = BLACK;
                Grandparent->color = RED;
                LeftRotate(Grandparent);
            }
        }
    }

    root->color = BLACK;
}

void Dictionary::RB_Transplant(Node* u, Node* v) {
    if (u->parent == nil) {
        root = v;
    } else {
        if (u != u->parent->left) {
            u->parent->right = v;
        } else {
            u->parent->left = v;
        }
    }
    v->parent = u->parent;
}

void Dictionary::RB_DeleteFixUp(Node* N) {
    Node* Sibling;

    while (N != root && (!N || N->color == BLACK)) { 
        bool isLeftChild = (N == N->parent->left);
        Sibling = isLeftChild ? N->parent->right : N->parent->left;

        if (Sibling->color == RED) {
            Sibling->color = BLACK;
            N->parent->color = RED;
            if (isLeftChild) {
                LeftRotate(N->parent);
                Sibling = N->parent->right;
            } else {
                RightRotate(N->parent);
                Sibling = N->parent->left;
            }
        }
        if ((Sibling->right->color == BLACK || !Sibling->right) && (Sibling->left->color == BLACK || !Sibling->left)) {
            Sibling->color = RED;
            N = N->parent;
        } else {
            if (isLeftChild && (!Sibling->right || Sibling->right->color == BLACK)) {
                if (Sibling->left) Sibling->left->color = BLACK;
                Sibling->color = RED;
                RightRotate(Sibling);
                Sibling = N->parent->right;
            } else if ((isLeftChild || Sibling->left) && Sibling->left->color != BLACK) {
                if (Sibling->right) Sibling->right->color = BLACK;
                Sibling->color = RED;
                LeftRotate(Sibling);
                Sibling = N->parent->left;
            }

            Sibling->color = N->parent->color;
            N->parent->color = BLACK;
            if (isLeftChild) {
                if (Sibling->right) Sibling->right->color = BLACK;
                LeftRotate(N->parent);
            } else {
                if (Sibling->left) Sibling->left->color = BLACK;
                RightRotate(N->parent);
            }
            N = root;
        }
    }

    if (N) N->color = BLACK; // Make sure N is not NULL
}

void Dictionary::RB_Delete(Node* N) {
    Node* newNode;
    Node* newSuccessor = N;
    int newColor = newSuccessor->color;

    if (N->left == nil || N->right == nil) {
        if (N->left == nil) {
            newNode = N->right;
        } else {
            newNode = N->left;
        }
        RB_Transplant(N, newNode);
    }
    else {
        newSuccessor = findMin(N->right);
        newColor = newSuccessor->color;
        newNode = newSuccessor->right;

        if (newSuccessor->parent != N) {
            RB_Transplant(newSuccessor, newNode);
            newSuccessor->right = N->right;
            newSuccessor->right->parent = newSuccessor;
        }

        RB_Transplant(N, newSuccessor);
        newSuccessor->left = N->left;
        newSuccessor->left->parent = newSuccessor;
        newSuccessor->color = N->color;
    }

    if (newColor == BLACK) {
        RB_DeleteFixUp(newNode);
    }
    delete N;
}

int Dictionary::size() const {
    return num_pairs;
}

bool Dictionary::contains(keyType k) const {
    return search(root, k) != nil;
}

valType& Dictionary::getValue(keyType k) const {
    Node* N = search(root, k);
    return (N == nil) ? throw logic_error( "key does not exist") : N->val;
}

bool Dictionary::hasCurrent() const {
    return current != nil;
}

keyType Dictionary::currentKey() const{
    if(hasCurrent() == false){
        throw logic_error("key undefined");
    }
    return current->key;
}

valType& Dictionary::currentVal() const{
    if(hasCurrent() == false){
        throw logic_error("key undefined");
    }
    return current->val;
}

void Dictionary::clear(){
    postOrderDelete(root);
    current = nil;
    root = nil;
    num_pairs = 0;
}

void Dictionary::setValue(keyType k, valType v) {
    Node* parent = nil;
    Node* current = root;

    while (current != nil) {
        parent = current;
        if (k < current->key) {
            current = current->left;
        } else if (k > current->key) {
            current = current->right;
        } else {
            // update the value
            current->val = v;
            return;
        }
    }

    Node* newNode = new Node(k, v);
    newNode->parent = parent;
    newNode->left = nil;
    newNode->right = nil;
    newNode->color = RED;

    if (parent == nil) {
        root = newNode;
    } else if (k < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    num_pairs++;
    RB_InsertFixUp(newNode);
}

void Dictionary::remove(keyType k) {
    Node* N = search(root, k);
    if (N == nil) {
        throw logic_error("key undefined");
    }
    
    if (N == current) {
        current = nil;
    }
    
    RB_Delete(N);
    num_pairs--;
}


void Dictionary::begin() {
    current = (num_pairs > 0) ? findMin(root) : nil;
}


void Dictionary::end() {
    current = (num_pairs > 0) ? findMax(root) : nil;
}

void Dictionary::next() {
    if (!hasCurrent()) {
        throw logic_error("current undefined");
    }
    
    Node* nextNode = findNext(current);
    current = (nextNode != nil) ? nextNode : nil;
}

void Dictionary::prev() {
    if (!hasCurrent()) {
        throw logic_error("current undefined");
    }
    
    Node* prevNode = findPrev(current);
    current = (prevNode != nil) ? prevNode : nil;
}

std::string Dictionary::to_string() const {
    std::string str;
    inOrderString(str, root);
    return str;
}

std::string Dictionary::pre_string() const {
    std::string str;
    preOrderString(str, root);
    return str;
}

bool Dictionary::equals(const Dictionary& D) const {
    if (num_pairs != D.num_pairs) {
        return false;
    }
    
    std::string strA = to_string();
    std::string strB = D.to_string();
    
    return strA == strB;
}

ostream& operator<<(ostream& stream, Dictionary& D) {
    stream << D.to_string();
    return stream;
}

bool operator==(const Dictionary& A, const Dictionary& B) {
    return A.equals(B);
}

Dictionary& Dictionary::operator=(const Dictionary& D) {
    if (this == &D) {
        return *this;
    }
    
    clear();
    preOrderCopy(D.root, D.nil);
    
    return *this;
}


