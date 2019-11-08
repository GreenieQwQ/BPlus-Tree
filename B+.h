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
    struct Leaf
    {
        int count;
        data_type data[L];
    };
    struct Node
    {
        int count;
        enum NodeTag { LEAF, NODE };
        union branch
        {
            Node* node[order];
            Leaf* leaf[order];
        };
        key_type key[order-1];
        NodeTag tag;
        Node();
    };

public:
    bool contains( const key_type& x ) const;
    bool insert( const key_type& x );
    bool erase( const key_type& x ); 

private:
    Node* root;
    bool insert( Node*& n, const key_type& x );
    size_t findPos( Node* n, const key_type& x ) const;
    void split( Node*& n );
    Node*& find( Node*& n, const key_type& x );
    Node* find( Node* n, const key_type& x ) const;
    Node* findmin( Node* n ) const;
    Node* findmax( Node* n ) const;
    void rotateLeft( Node*& n );
    void rotateRight( Node*& n );
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
bool B_Tree<data_type, key_type, order, L>::insert( const key_type& x )
{
    insert( root, x );
}   

template<typename data_type, typename key_type, int order, int L>
bool B_Tree<data_type, key_type, order, L>::insert( Node*& n, const key_type& x )
{
    if( n->NodeTag == Node::LEAF )
    {
        
    }
    else if( n->NodeTag == Node::NODE )
    {
        size_t pos = findPos( n, x )
        Node* branch =  n->branch.node[i];
        return insert( branch, x );
    }
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
void B_Tree<data_type, key_type, order, L>::split( Node*& n )
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