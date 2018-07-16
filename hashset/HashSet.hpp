// HashSet.hpp


#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;
    
    // A struct that creates a node object that connects to a linked-list
    struct Node
    {
        ElementType value;
        Node* next;
        
        Node(ElementType v, Node* n)
            : value(v), next(n)
        {
        }
    };

    // A function that copies a HashSet's array of linked-list
    void copy(Node**& dest, Node** src, int cap, int newcap, HashFunction func);

    // A function that deletes each node in the linked-list that the key points to
    void deleteLists(Node**& hashset, int cap) noexcept;

    int stored; // number of elements currently stored within the HashSet
    int capacity; // the current size of the array

    Node** set; // dynamically-allocated array of pointers to linked-lists
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    try
    {
        set = nullptr;
        set = new Node*[DEFAULT_CAPACITY]();
        capacity = DEFAULT_CAPACITY;
        stored = 0;
    }
    catch(...)
    {
        delete[] set;
        throw;
    }
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    deleteLists(set, capacity);
    delete[] set;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    try
    {
        hashFunction = s.hashFunction;
        capacity = s.capacity;
        stored = s.stored;
        set = nullptr;
        set = new Node*[s.capacity]();
        copy(set, s.set, s.capacity, s.capacity, s.hashFunction);
    }
    catch(...)
    {
        deleteLists(set, capacity);
        delete[] set;
        throw;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    set = s.set;
    hashFunction = s.hashFunction;
    capacity = s.capacity;
    stored = s.stored;
    s.set = nullptr;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    Node** temp = {};

    try
    {
        temp = new Node*[s.capacity]();
        copy(temp, s.set, s.capacity, s.capacity, s.hashFunction);
        deleteLists(set, capacity);
        delete[] set;
        set = temp;
        hashFunction = s.hashFunction;
        capacity = s.capacity;
        stored = s.stored;
        temp = nullptr;
        delete temp;
    }
    catch(...)
    {
        deleteLists(temp, s.capacity);
        delete[] temp;   
        throw;
    }

    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    Node** temp = s.set;

    deleteLists(set, capacity);
    delete[] set;

    set = temp;
    hashFunction = s.hashFunction;
    capacity = s.capacity;
    stored = s.stored;

    s.set = nullptr;
    temp = nullptr;

    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if (this->contains(element) == false)
    {
        int index = hashFunction(element) % capacity;
        set[index] = new Node(element, set[index]);
        stored++;
   
        if ((double)((double)stored/(double)capacity) >= 0.8)
        {
            Node** temp = {};

            try
            {
                temp = new Node*[capacity*2]();
                copy(temp, set, capacity, capacity*2, hashFunction);
                deleteLists(set, capacity);
                delete[] set;
                set = temp;
                capacity = capacity*2;
            }
            catch(...)
            {
                deleteLists(temp, capacity*2);
                delete[] temp;
                throw;
            }  
        }
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    int index = hashFunction(element) % capacity;

    for (Node* p = set[index]; p != nullptr; p = p->next)
    {
        if (element == p->value)
        {
            return true;
        }
    }

    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return stored;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if (index < 0 || index >= capacity)
    {
        return 0;
    }

    int counter = 0;

    for (Node* p = set[index]; p != nullptr; p = p->next)
    {
        counter++;
    }

    return counter;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if (index < 0 || index >= capacity)
    {
        return 0;
    }

    return hashFunction(element) % capacity == index;
}


template <typename ElementType>
void HashSet<ElementType>::copy(Node**& dest, Node** src, int cap, int newcap, HashFunction func)
{
    for (int i = 0; i < cap; ++i)
    {
        if (src[i] != nullptr)
        {
            for (Node* p = src[i]; p != nullptr; p = p->next)
            {
                int index = func(p->value) % (newcap);
                dest[index] = new Node(p->value, dest[index]);
            }
        }
    }
}


template<typename ElementType>
void HashSet<ElementType>::deleteLists(Node**& hashset, int cap) noexcept
{
    if (hashset != nullptr)
    {
        for (int i = 0; i < cap; ++i)
        {
            Node* temp;

            for (Node* p = hashset[i]; p != nullptr; )
            {
                temp = p;
                p = p->next;
                delete temp;
            }
        }
    }
}



#endif // HASHSET_HPP

