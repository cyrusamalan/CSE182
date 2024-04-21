//Cyrus Amalan, camalan, pa7

#include <stdexcept>
#include <string>
#include <iostream>

#include "Dictionary.h"

using namespace std;

Dictionary::Node::Node(keyType k, valType v)
    : key(k), val(v), parent(nullptr), left(nullptr), right(nullptr) {}


void Dictionary::inOrderString(std::string& s, Node* R) const{
    if(R == nil){
        return;
    }
    inOrderString(s, R->left);
    s += R->key + " : " + std::to_string(R->val) + "\n";
    inOrderString(s, R->right);
}
void Dictionary::preOrderCopy(Node* R, Node* N) {
    if (R != N) {
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

void Dictionary::preOrderString(string& s, Node* R) const{
    if(R == nil){
        return;
    }
    s.append(R->key + "\n");
    preOrderString(s, R->left);
    preOrderString(s, R->right);
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

Dictionary::Dictionary() : nil(new Node("", 0)), root(nil), current(nil), num_pairs(0) {}


Dictionary::Dictionary(const Dictionary& D)
    : nil(new Node("", 0)), root(nil), current(nil), num_pairs(0) {
    preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary() {
    postOrderDelete(root);
    delete nil;
}

int Dictionary::size() const{
    return num_pairs;
}

bool Dictionary::contains(keyType k) const{
    return search(root, k) != nil;
}

valType& Dictionary::getValue(keyType k) const{
    Node* NodeParent = search(root, k);
    if(NodeParent != nil){
        return NodeParent->val;
    }else{
        throw logic_error("The key is not valid or does not exist");
    }
}

bool Dictionary::hasCurrent() const{
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

void Dictionary::setValue(keyType k, valType v){
    Node* tranverse_N = root;
    Node* previous_N = nil;
    Node* Insert_N = new Node(k, v);
    Insert_N->left = nil;
    Insert_N->right = nil;

    while(tranverse_N != nil){
        previous_N = tranverse_N;
        if(tranverse_N->key < k){
            tranverse_N = tranverse_N->right;
        }else if(tranverse_N->key > k){
            tranverse_N = tranverse_N->left;
        }else{
            //the key is already there
            tranverse_N->val = v;
            delete Insert_N;
            return;
        }
    }

    Insert_N->parent = previous_N;
    if(previous_N == nil){
        root = Insert_N;
    }else if(previous_N->key > k){
        previous_N->left = Insert_N;
    }else{
        previous_N->right = Insert_N;
    }
    num_pairs++;
}

void Dictionary::remove(keyType k){
    if(contains(k) == false){
        throw logic_error("the key is not valid");
    }

    Node* node = search(root, k);
    if(node == current){
        current = nil;
    }
    //CASE 1
    if(!node->right && !node->left){
        if(node == root){
            root = nullptr;
        }else{
            if(node->parent->left == node){
                node->parent->left = nullptr;
            }
            else{
                node->parent->right = nullptr;
            }
        }
    delete node;
    }

    //CASE 2
    else if(!node->left){
        if(node == root){
            root = node->right;
        }else{
            if(node->parent->left == node){
                node->parent->left = node->right;
            }else{
                node->parent->right = node->right;
            }
        }
        node->right->parent = node->parent;
        delete node;
    }

    //CASE 3
    else if(node->right == nil){
        if(node == root){
            root = node->left;
        }else{
            if(node->parent->left == node){
                node->parent->left = node->left;
            }else{
                node->parent->right = node->left;
            }
        }
        node->left->parent = node->parent;
        delete node;
    }
    //FINAL CASE
    else{
        Node* replace = findMin(node->right);
        if(replace->parent != node){
            replace->parent->left = replace->right;
            if(replace->right != nullptr){
                replace->right->parent = replace->parent;
            }
            replace->right = node->right;
            replace->right->parent = replace;
        }
        if(node == root){
            root = replace;
        }else{
            if(node->parent->left == node){
                node->parent->left = replace;
            }else{
                node->parent->right = replace;
            }
        }
        replace->parent = node->parent;
        replace->left = node->left;
        if(node->left != nullptr){
            node->left->parent = replace;
        }
        delete node;
    }
    num_pairs--;
}

void Dictionary::begin(){
    if(root == nil){
        return;
    }else{
        current = findMin(root);
    }
}

void Dictionary::end(){
    if(root == nil){
        return;
    }else{
        current = findMax(root);
    }
}

void Dictionary::next(){
    if(hasCurrent() == false){
        throw logic_error("not defined");
    }
    Node* node = findNext(current);
    current = node;
}

void Dictionary::prev(){
    if(hasCurrent() == false){
        throw logic_error("it is not defined");
    }
    Node* node = findPrev(current);
    current = node;
}

string Dictionary::to_string() const{
    string x;
    inOrderString(x, root);
    return x;
}

string Dictionary::pre_string() const{
    string x;
    preOrderString(x, root);
    return x;
}


bool Dictionary::equals(const Dictionary& D) const{
    if(size() == D.size()){
        string x = to_string();
        string y = D.to_string();
        return x == y;
    }else{
        return false;
    }
}

ostream& operator<<(ostream& stream, Dictionary& D) {
    return stream << D.to_string(); // Directly return the result of stream insertion
}

bool operator==(const Dictionary& A, const Dictionary& B){
    return A.equals(B);
}

Dictionary& Dictionary::operator=(const Dictionary& D) {
    if (this != &D) {
        this->clear();
        this->preOrderCopy(D.root, D.nil);
    }
    return *this;
}













