// DoublyLinkedList.hpp

#ifndef DOUBLYLINKEDLIST_HPP
#define DOUBLYLINKEDLIST_HPP

#include "EmptyException.hpp"
#include "IteratorException.hpp"


template <typename ValueType>
class DoublyLinkedList
{
public:
    class Iterator;
    class ConstIterator;

private:
    struct Node;


public:
    // Initializes this list to be empty.
    DoublyLinkedList() noexcept;

    // Initializes this list as a copy of an existing one.
    DoublyLinkedList(const DoublyLinkedList& list);

    // Initializes this list from an expiring one.
    DoublyLinkedList(DoublyLinkedList&& list) noexcept;


    // Destroys the contents of this list.
    virtual ~DoublyLinkedList() noexcept;


    // Replaces the contents of this list with a copy of the contents
    // of an existing one.
    DoublyLinkedList& operator=(const DoublyLinkedList& list);

    // Replaces the contents of this list with the contents of an
    // expiring one.
    DoublyLinkedList& operator=(DoublyLinkedList&& list) noexcept;


    // addToStart() adds a value to the start of the list, meaning that
    // it will now be the first value, with all subsequent elements still
    // being in the list (after the new value) in the same order.
    void addToStart(const ValueType& value);

    // addToEnd() adds a value to the end of the list, meaning that
    // it will now be the last value, with all subsequent elements still
    // being in the list (before the new value) in the same order.
    void addToEnd(const ValueType& value);


    // removeFromStart() removes a value from the start of the list, meaning
    // that the list will now contain all of the values *in the same order*
    // that it did before, *except* that the first one will be gone.
    // In the event that the list is empty, an EmptyException will be thrown.
    void removeFromStart();

    // removeFromEnd() removes a value from the end of the list, meaning
    // that the list will now contain all of the values *in the same order*
    // that it did before, *except* that the last one will be gone.
    // In the event that the list is empty, an EmptyException will be thrown.
    void removeFromEnd();


    // first() returns the value at the start of the list.  In the event that
    // the list is empty, an EmptyException will be thrown.  There are two
    // variants of this member function: one for a const DoublyLinkedList and
    // another for a non-const one.
    const ValueType& first() const;
    ValueType& first();


    // last() returns the value at the end of the list.  In the event that
    // the list is empty, an EmptyException will be thrown.  There are two
    // variants of this member function: one for a const DoublyLinkedList and
    // another for a non-const one.
    const ValueType& last() const;
    ValueType& last();


    // isEmpty() returns true if the list has no values in it, false
    // otherwise.
    bool isEmpty() const noexcept;


    // size() returns the number of values in the list.
    unsigned int size() const noexcept;



    // Iterators

    // iterator() creates a new Iterator over this list.  It will
    // initially be referring to the first value in the list, unless the
    // list is empty, in which case it will be considered both "past start"
    // and "past end".
    Iterator iterator();


    // constIterator() creates a new ConstIterator over this list.  It will
    // initially be referring to the first value in the list, unless the
    // list is empty, in which case it will be considered both "past start"
    // and "past end".
    ConstIterator constIterator() const;


public:
    // The IteratorBase class is the base class for our two kinds of
    // iterators.  Because there are so many similarities between them,
    // we write those similarities in a base class, then inherit from
    // that base class to specify only the differences.
    class IteratorBase
    {
    public:
        // Initializes a newly-constructed IteratorBase to operate on
        // the given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        IteratorBase(const DoublyLinkedList& list) noexcept;


        // moveToNext() moves this iterator forward to the next value in
        // the list.  If the iterator is refrering to the last value, it
        // moves to the "past end" position.  If it is already at the
        // "past end" position, an IteratorException will be thrown.
        void moveToNext();


        // moveToPrevious() moves this iterator backward to the previous
        // value in the list.  If the iterator is refrering to the first
        // value, it moves to the "past start" position.  If it is already
        // at the "past start" position, an IteratorException will be thrown.
        void moveToPrevious();


        // isPastStart() returns true if this iterator is in the "past
        // start" position, false otherwise.
        bool isPastStart() const noexcept;


        // isPastEnd() returns true if this iterator is in the "past end"
        // position, false otherwise.
        bool isPastEnd() const noexcept;
    
    protected:
        // You'll want protected member variables and member functions,
        // which will be accessible to the derived classes.
        
        Node* current; // Pointer to the current node in the iteration
        bool pastStart; // true if iterator is "past start" else false
        bool pastEnd; // true if iterator is "past end" else false
        DoublyLinkedList<ValueType> clist; // copy of list in const form
    };


    class ConstIterator : public IteratorBase
    {
    public:
        // Initializes a newly-constructed ConstIterator to operate on
        // the given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        ConstIterator(const DoublyLinkedList& list) noexcept;


        // value() returns the value that the iterator is currently
        // referring to.  If the iterator is in the "past start" or
        // "past end" positions, an IteratorException will be thrown.
        const ValueType& value() const;
    };


    class Iterator : public IteratorBase
    {
    public:
        // Initializes a newly-constructed Iterator to operate on the
        // given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        Iterator(DoublyLinkedList& list) noexcept;


        // value() returns the value that the iterator is currently
        // referring to.  If the iterator is in the "past start" or
        // "past end" positions, an IteratorException will be thrown.
        ValueType& value() const;


        // insertBefore() inserts a new value into the list before
        // the one to which the iterator currently refers.  If the
        // iterator is in the "past start" position, an IteratorException
        // is thrown.
        void insertBefore(const ValueType& value);


        // insertAfter() inserts a new value into the list after
        // the one to which the iterator currently refers.  If the
        // iterator is in the "past end" position, an IteratorException
        // is thrown.
        void insertAfter(const ValueType& value);


        // remove() removes the value to which this iterator refers,
        // moving the iterator to refer to either the value after it
        // (if moveToNextAfterward is true) or before it (if
        // moveToNextAfterward is false).  If the iterator is in the
        // "past start" or "past end" position, an IteratorException
        // is thrown.
        void remove(bool moveToNextAfterward = true);
    private:
        DoublyLinkedList<ValueType>& ilist; // Reference to list for modification
    };


private:
    // A structure that contains the vital parts of a Node in a
    // doubly-linked list, the value and two pointers: one pointing
    // to the previous node (or nullptr if there isn't one) and
    // one pointing to the next node (or nullptr if there isn't
    // one).
    struct Node
    {
        ValueType value;
        Node* prev;
        Node* next;
    };

    // Deletes each node in the doubly-linked list that head points to.
    void deleteList(Node* list) noexcept;

    // Copies a given doubly-linked list to another doubly-linked list.
    void copyList(const DoublyLinkedList& list, Node*& temp_head, Node*& temp_tail);

    Node* head; // Pointer to the first node in the list
    Node* tail; // Pointer to the last node in the list
};



template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList() noexcept
    : head(nullptr), tail(nullptr)
{
}


template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList(const DoublyLinkedList& list)
{
    head = nullptr;
    tail = nullptr;

    try
    {
        copyList(list, head, tail);
    }

    catch(...)
    {
        deleteList(head);
        throw;
    }
}


template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList(DoublyLinkedList&& list) noexcept
{
    head = list.head;
    tail = list.tail;
    list.head = nullptr;
    list.tail = nullptr;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::~DoublyLinkedList() noexcept
{
    deleteList(head);
}


template <typename ValueType>
DoublyLinkedList<ValueType>& DoublyLinkedList<ValueType>::operator=(const DoublyLinkedList& list)
{
    Node* temp_head = nullptr;
    Node* temp_tail = nullptr;

    try
    {
        copyList(list, temp_head, temp_tail);
    
        deleteList(head);
        head = temp_head;
        tail = temp_tail;
    }

    catch(...)
    {
        deleteList(temp_head);
        throw;
    }

    return *this;
}


template <typename ValueType>
DoublyLinkedList<ValueType>& DoublyLinkedList<ValueType>::operator=(DoublyLinkedList&& list) noexcept
{
    Node* temp = head;
    head = list.head;
    list.head = temp;

    temp = tail;
    tail = list.tail;
    list.tail = temp;

    temp = nullptr;

    return *this;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::addToStart(const ValueType& value)
{
    Node* new_node = new Node();

    new_node->prev = nullptr;
    new_node->value = value;
    new_node->next = head;

    if (new_node->next != nullptr)
    {
        head->prev = new_node;
    }

    head = new_node;

    if (tail == nullptr)
    {
        tail = new_node;
    }
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::addToEnd(const ValueType& value)
{
    Node* new_node = new Node();

    new_node->next = nullptr;
    new_node->value = value;
    new_node->prev = tail;

    if (new_node->prev != nullptr)
    {
        tail->next = new_node;
    }

    tail = new_node;
    
    if (head == nullptr)
    {
        head = new_node;
    }
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::removeFromStart()
{

    if (head == nullptr & tail == nullptr)
    {
        throw EmptyException();
    }
    
    else if (head == tail)
    {
        delete head;
        head = nullptr;
        tail = nullptr;
    }

    else
    {
        head = head->next;
        delete head->prev;
        head->prev = nullptr;
    }
    
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::removeFromEnd()
{

    if (head == nullptr && tail == nullptr)
    {
        throw EmptyException();
    }

    else if (head == tail)
    {
        delete head;
        head = nullptr;
        tail = nullptr;
    }

    else
    {
        tail = tail->prev;
        delete tail->next;
        tail->next = nullptr;
    }
    
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::first() const
{
    // note that this is an awful thing i'm doing here, but i needed
    // something that would make this code compile.  you're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.

    if (head == nullptr && tail == nullptr)
    {
        throw EmptyException();
    }

    return head->value;
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::first()
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.

    if (head == nullptr && tail == nullptr)
    {
        throw EmptyException();
    }

    return head->value;
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::last() const
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.

    if (head == nullptr && tail == nullptr)
    {
        throw EmptyException();
    }

    return tail->value;
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::last()
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.

    if (head == nullptr && tail == nullptr)
    {
        throw EmptyException();
    }

    return tail->value;
}


template <typename ValueType>
unsigned int DoublyLinkedList<ValueType>::size() const noexcept
{
    int counter = 0;

    for (Node* p = head; p != nullptr; p = p->next)
    {
        counter++;
    }

    return counter;
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::isEmpty() const noexcept
{
    return head == nullptr && tail == nullptr;
}


template <typename ValueType>
typename DoublyLinkedList<ValueType>::Iterator DoublyLinkedList<ValueType>::iterator()
{
    return Iterator{*this};
}


template <typename ValueType>
typename DoublyLinkedList<ValueType>::ConstIterator DoublyLinkedList<ValueType>::constIterator() const
{
    return ConstIterator{*this};
}


template <typename ValueType>
DoublyLinkedList<ValueType>::IteratorBase::IteratorBase(const DoublyLinkedList& list) noexcept
    : clist(list)
{
    current = list.head;

    if (list.head == nullptr && list.tail == nullptr)
    {
        pastStart = true;
        pastEnd = true;
    }
    else
    {
        pastStart = false;
        pastEnd = false;
    }

}


template <typename ValueType>
void DoublyLinkedList<ValueType>::IteratorBase::moveToNext()
{
    if (pastEnd)
    {
        throw IteratorException();
    }

    if (pastStart && clist.size() != 0)
    {
        current = clist.head;
        pastStart = false;
    }
    else
    {
        current = current->next;
    }

    if (current == nullptr)
    {
        pastEnd = true;
    }
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::IteratorBase::moveToPrevious()
{
    if (pastStart)
    {
        throw IteratorException();
    }

    if (pastEnd && clist.size() != 0)
    {
        current = clist.tail;
        pastEnd = false;
    }
    else
    {
        current = current->prev;
    }

    if (current == nullptr)
    {
        pastStart = true;
    }
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::IteratorBase::isPastStart() const noexcept
{
    return pastStart;
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::IteratorBase::isPastEnd() const noexcept
{
    return pastEnd;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::ConstIterator::ConstIterator(const DoublyLinkedList& list) noexcept
    : IteratorBase{list}
{
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::ConstIterator::value() const
{

    if (IteratorBase::pastStart || IteratorBase::pastEnd)
    {
        throw IteratorException();
    }

    return IteratorBase::current->value;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::Iterator::Iterator(DoublyLinkedList& list) noexcept
    : IteratorBase{list}, ilist(list)
{
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::Iterator::value() const
{

    if (IteratorBase::pastStart || IteratorBase::pastEnd)
    {
        throw IteratorException();
    }

    return IteratorBase::current->value;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::insertBefore(const ValueType& value)
{
    Node* new_node = new Node;
    new_node->value = value;

    if (IteratorBase::pastStart)
    {
        throw IteratorException();
    }
    else if (IteratorBase::pastEnd)
    {
        new_node->prev = ilist.tail;
        ilist.tail->next = new_node;
        ilist.tail = new_node;
    }
    else if (IteratorBase::current->prev == nullptr)
    {
        ilist.head = new_node;
        new_node->prev = IteratorBase::current->prev;
        IteratorBase::current->prev = new_node;
    }
    else
    {
        new_node->prev = IteratorBase::current->prev;
        IteratorBase::current->prev->next = new_node;
        IteratorBase::current->prev = new_node;
    }

    new_node->next = IteratorBase::current;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::insertAfter(const ValueType& value)
{   
    Node* new_node = new Node;
    new_node->value = value;

    if (IteratorBase::pastEnd)
    {
        throw IteratorException();
    }
    else if (IteratorBase::pastStart)
    {
        new_node->next = ilist.head;
        ilist.head->prev = new_node;
        ilist.head = new_node;
    }
    else if (IteratorBase::current->next == nullptr)
    {
        ilist.tail = new_node;
        new_node->next = IteratorBase::current->next;
        IteratorBase::current->next = new_node;
    }
    else
    {
        new_node->next = IteratorBase::current->next;
        IteratorBase::current->next->prev = new_node;
        IteratorBase::current->next = new_node;
    }

    new_node->prev = IteratorBase::current;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::remove(bool moveToNextAfterward)
{
    if (IteratorBase::pastEnd || IteratorBase::pastStart)
    {
        throw IteratorException();
    }

    Node* temp = IteratorBase::current;

    if (IteratorBase::current == ilist.head)
    {
        ilist.head = IteratorBase::current->next;
        if (IteratorBase::current->next != nullptr)
        {
            IteratorBase::current->next->prev = nullptr;
        }
        else
        {
            ilist.tail = nullptr;
            IteratorBase::pastEnd = true;
        }
        IteratorBase::current = ilist.head;
        if (moveToNextAfterward == false)
            IteratorBase::pastStart = true;
 
        delete temp;
    }  
    else if (IteratorBase::current == ilist.tail)
    {
        ilist.tail = IteratorBase::current->prev;
        if (IteratorBase::current->prev != nullptr)
        {
            IteratorBase::current->prev->next = nullptr;
        }
        else
        {
            ilist.head = nullptr;
            IteratorBase::pastStart = true;
        }
        
        IteratorBase::current = ilist.head;
        if (moveToNextAfterward == true)
            IteratorBase::pastEnd = true;

        delete temp;
    }
    else if (moveToNextAfterward == true)
    {
        IteratorBase::current->prev->next = IteratorBase::current->next;
        IteratorBase::current->next->prev = IteratorBase::current->prev;
        IteratorBase::current = IteratorBase::current->next;
        delete temp;
    }
    else if (moveToNextAfterward == false)
    {
        IteratorBase::current->prev->next = IteratorBase::current->next;
        IteratorBase::current->next->prev = IteratorBase::current->prev;
        IteratorBase::current = IteratorBase::current->prev;
        delete temp;
    }
}

template <typename ValueType>
void DoublyLinkedList<ValueType>::deleteList(Node* list) noexcept
{
    Node* current = list;
    Node* next;

    while (current != nullptr)
    {
        next = current->next;
        delete current;
        current = next;
    }
}

template <typename ValueType>
void DoublyLinkedList<ValueType>::copyList(const DoublyLinkedList<ValueType>& list, 
                                           Node*& temp_head, Node*& temp_tail)
{
    for (Node* p = list.head; p != nullptr; p = p->next)
    {
        Node* new_node = new Node;
        new_node->value = p->value;
 
        if (temp_head == nullptr)
        {
            temp_head = new_node;
            new_node->prev = nullptr;
        }

        if (temp_tail != nullptr)
        {
            temp_tail->next = new_node;
            new_node->prev = temp_tail;
        }

        new_node->next = nullptr;
        temp_tail = new_node;
    }
}


#endif
