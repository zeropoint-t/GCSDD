// -------------------------------------HashNode.h------------------------------
// Programmer Names:            Tetsuya Hayashi/Eddie Raskin/Ahmed Nada
// Course Section Number:       CSS502A - Winter 2019
// Creation Date:               03/03/2019
// Date of Last Modification:   03/03/2019
// -----------------------------------------------------------------------------
// HashNode - TEMPLATE - Custom Hash Class
// -----------------------------------------------------------------------------
// Used to store customers in media management system
// -----------------------------------------------------------------------------
#ifndef HASHNODE_h
#define HASHNODE_h

// Hash node class template
template <typename k, typename v>
class HashNode
{
private:
    k _key;
    v _value;
    HashNode* _next;

public:
    HashNode(const k &key, const v &value);

    ~HashNode();

    k getKey() const;

    v getValue() const;

    void setValue(v value);

    HashNode<k,v>* getNext() const;

    void setNext(HashNode *next);
};

template<typename k, typename v>
HashNode<k,v>::HashNode(const k &key, const v &value) :
    _key(key), _value(value), _next(NULL)
{
}

template<typename k, typename v>
HashNode<k,v>::~HashNode()
{
    // cout << "Destroy HashNode" << endl;
}

template<typename k, typename v>
k HashNode<k,v>::getKey() const
{
    return _key;
}

template<typename k, typename v>
v HashNode<k,v>::getValue() const
{
    return _value;
}

template<typename k, typename v>
void HashNode<k,v>::setValue(v value)
{
    _value = value;
}

template<typename k, typename v>
HashNode<k,v>* HashNode<k,v>::getNext() const
{
    return _next;
}

template<typename k, typename v>
void HashNode<k,v>::setNext(HashNode *next)
{
    _next = next;
}

#endif
