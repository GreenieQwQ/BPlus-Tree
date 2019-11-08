#ifndef BPLUS_H
#define BPLUS_H

#include <cstddef>
#include <iostream>
using namespace std;

template<typename data_type, typename key_type, int order = 4, int L = 4>
class B_Tree
{
public:
    B_Tree();
    B_Tree( const B_Tree& );
private:
    enum State { success, failure, overflow, duplicate };
    struct Leaf
    {
        int count;
        data_type data[L];
    };
    struct Node
    {
        int count;
        enum NodeTag:char { LEAF, NODE };
        union branch
        {
            Node* node[order];
            Leaf* leaf[order];
        };
        key_type key[order-1];
        NodeTag tag;
        Node(): count(0), tag(NODE) {}
    };

public:
    bool contains( const key_type& x ) const;
    bool insert( const data_type& x );
    bool erase( const key_type& x ); 

private:
    Node* root;
    State insert_data( Node*& n, const key_type& key, const data_type& data, key_type& newKey, Node*& newBranch );
    void insert_key( Node* n, const key_type& newkey, Node* newBranch, size_t pos );
    void insert_key( Node* n, const key_type& newkey, Leaf* newBranch, size_t pos );
    size_t findPos( Node* n, const key_type& x ) const;
    void split( Node*& n, Node* pre );
    void split( Leaf*& n, Node* pre );
    void display( Node* n, int indent = 0 ) const;
    void display( Leaf* l, int indent = 0 ) const;
    
private:    
    inline void displayIndent( int indent ) const
    {
        for( int i = 0; i < indent; ++i )
            cout << "\t";
    }
};

template<typename data_type, typename key_type, int order, int L>
bool B_Tree<data_type, key_type, order, L>::insert( const data_type& data )
{
    key_type key = getKey( data );
    key_type newKey;
    Node* newBranch;
    State result = insert_data( root, key, data, newKey, newBranch );
    if( result == overflow )
    {
        Node* newRoot = new Node;
        newRoot->count = 1;
        newRoot->key[0] = newKey;
        newRoot->branch[0] = root;
        newRoot->branch[1] = newBranch;
        root = newRoot;
        result = success; 
    }
    if( result == duplicate )
        return false;
    else
        return true;
}   

template<typename data_type, typename key_type, int order, int L>
typename B_Tree<data_type, key_type, order, L>::State
B_Tree<data_type, key_type, order, L>::insert_data( Node*& n, const key_type& key, const data_type& data, key_type& newKey, Node*& newBranch )
{
    if( n->NodeTag == Node::LEAF )
    {
        size_t pos = findPos( n, x );
        Leaf*& l = n->branch.leaf[pos];
        for( int i = 0; i < l->count; ++i )
            if( data == l->data[i] )
                return duplicate;
        
        key_type newKey;
        Leaf* newBranch;
        State result = insert( l, key, data, newKey, newBranch );
        if( result == overflow )
        {
            if( n->count < order - 1 )
            {
                result = success;
                insert_key( n, newKey, newBranch, pos );
            }
        }
    }
    else if( n->NodeTag == Node::NODE )
    {
        size_t pos = findPos( n, x );
        Node* branch =  n->branch.node[pos];
        return insert( branch, x );
    }
} 

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::insert_key( Node* n, const key_type& key, const data_type& data, size_t pos ) 
{
    for( int i = 0; i < )
}   

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::insert_key( Node* n, const key_type& key, const data_type& data, size_t pos ) 
{
    for( int i = 0; i < )
}   

template<typename data_type, typename key_type, int order, int L>
size_t B_Tree<data_type, key_type, order, L>::findPos( Node* n, const key_type& x ) const
{
    for( int i = 0; i < n->count; ++i )
    {
        if( x < n->key[i] )
            return i;
    }
    return n->count;
}   

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::split( Node*& n, Node* pre)
{
    if( n->count == order - 1 )
    {
        if( n == root )
        {

        }
        else
        {
            
        }
    }
}   

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::split( Leaf*& n, Node* pre )
{
    if( n->count == L )
    {
        if( n == root )
        {
            
        }
        else
        {
            
        }
    }
}   

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::display( Node* n, int indent ) const
{
    if( n->NodeTag == Node::NODE )
    {
        for( int i = n->count - 1; i >= 0; --i )
        {
            display( n->branch.node[i+1], indent + 1 );
            displayIndent(indent);
            cout << n->key[i] << endl;
        }
        display( n->branch.node[0], indent + 1 );
    }
    else if( n->NodeTag == Node::LEAF )
    {
        for( int i = n->count - 1; i >= 0; --i )
        {
            display( n->branch.leaf[i+1], indent + 1 );
            displayIndent(indent);
            cout << n->key[i] << endl;
        }
        display( n->branch.leaf[0], indent + 1 );
    }       
} 

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::display( Leaf* l, int indent ) const
{
    displayIndent(indent);
    
    for( int i = 0; i < l->count; ++i )
        cout << l->data[i] << ' ';

    cout << endl;
}   



#endif