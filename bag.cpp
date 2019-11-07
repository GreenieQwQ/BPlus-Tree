#ifndef BAG_H
#define BAG_H

#include <iostream>
#include <vector>
using namespace std;


typedef int T;

class Bag {
  public:
   virtual size_t size() const = 0;//returns the number of items in the bag.
   virtual bool empty() const = 0;//returns true if it is empty, returns false otherwise.
   virtual bool contains(const T &x) const = 0;//returns true if x is in the bag, returns false otherwise.
   virtual void insert(const T &x) = 0;//insert x even if x is in the bag
   virtual void erase(const T &x) = 0;//erase x if x is in the bag.
   virtual void display() const = 0;//print all elements to standard output in ascending order, and leave a space after every element.
   virtual T min() const = 0;//returns a reference to the smallest item if it is not empty.
   virtual T max() const = 0; //returns a reference to the biggest item if it is not empty.
   virtual void clear() = 0;//make an empty bag 
   void print()const //don't override this method! 
   {
      cout<<"size:" <<size()<<endl;
      cout<<"elements:";
      display();
      cout<<endl;
   }
};

#endif

class MyBag : public Bag {
public:
    MyBag(): root(nullptr), _size(0) {}
    MyBag(const MyBag& other)
    {
        root = clone(other.root);
        _size = other.size();
    }
private:
    struct Node
    {
        Node *left, *right;
        T data;
        int count;
        Node(T d, Node* l = nullptr, Node* r = nullptr, int c = 1)
        : left(l), right(r), data(d), count(c) {}
    };
private:
    int _size;
    Node* root;
public:
    size_t size() const
    {
        return _size;
    }
    bool empty() const
    {
        return size() == 0;
    }
    bool contains(const T &x) const
    {
        return find(root, x) != nullptr;
    }
    void insert(const T &x)
    {
        Node*& n = find(root, x);
        if( n == nullptr )
            n = new Node(x);
        else
            n->count++;
        _size++;
    }
    void erase(const T &x)
    {
        Node*& n = find(root, x);
        if( n == nullptr )
            return;
        
        n->count--;
        _size--;
        if(n->count == 0)
        {
            if( n->left == nullptr || n->right == nullptr )
            {
                Node* old = n;
                n = n->left != nullptr ? n->left : n->right;
                delete old;
            }
            else
            {
                Node* newNode = findmin(n->right);
                T newData = newNode->data;
                n->count = newNode->count;
                newNode->count = 1;
                erase(newNode->data);
                n->data = newData;
                _size++;;
            }
        }
    }
    void display() const
    {
        display(root);
    }
    T min() const
    {
        return findmin(root)->data;
    }
    T max() const
    {
        return findmax(root)->data;
    }
    void clear()
    {
        clear(root);
        root = nullptr;
        _size = 0;
    }
private:
    Node* find(Node* n, const T &x) const
    {
        if( n == nullptr || n->data == x )
            return n;
        if( x < n->data )
            return find(n->left, x);
        else
            return find(n->right, x);
    }
    Node*& find(Node*& n, const T &x)
    {
        if( n == nullptr || n->data == x )
            return n;
        if( x < n->data )
            return find(n->left, x);
        else
            return find(n->right, x);
    }
    Node* findmin(Node* n) const
    {
        while(n->left != nullptr)
            n = n->left;
        return n;
    }
    Node* findmax(Node* n) const
    {
        while(n->right != nullptr)
            n = n->right;
        return n;
    }
    void display(Node* n) const
    {
        if(n)
        {
            display(n->left);
            for( int i = 0; i < n->count; ++i)
                cout << n->data << " ";
            display(n->right);
        }
    }
    void clear(Node* n)
    {
        if(n)
        {
            clear(n->left);
            clear(n->right);
            delete n;
        }
    }
    Node* clone(Node* n)
    {
        if(n)
            return new Node(n->data, clone(n->left), clone(n->right), n->count);
        else 
            return nullptr;
    }
};
//The type of data stored in MyBag is defined in Bag.h, which is T.  Bag.h中定义了MyBag中存储的数据类型T。