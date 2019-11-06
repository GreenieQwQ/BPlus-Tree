#ifndef BPLUS_H
#define BPLUS_H

template<typename data_type, typename key_type, int order, int L>
class B_Tree
{
public:
    B_Tree();
    B_Tree( const B_Tree& );
private:
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
    struct Leaf
    {
        int count;
        data_type data[L];
    };

public:
    bool contains( const key_type& x ) const;
    bool insert( const key_type& x );
    bool erase( const key_type& x ); 

private:
    Node* root;
    Node*& find( Node*& n, const key_type& x );
    Node* find( Node* n, const key_type& x ) const;
    Node* findmin( Node* n ) const;
    Node* findmax( Node* n ) const;
    void rotateLeft( Node*& n );
    void rotateRight( Node*& n );
};


#endif