// SkipListSet.hpp

#ifndef SKIPLISTSET_HPP
#define SKIPLISTSET_HPP

#include <memory>
#include <random>
#include "Set.hpp"



// SkipListKind indicates a kind of key: a normal one, the special key
// -INF, or the special key +INF.

enum class SkipListKind
{
    Normal,
    NegInf,
    PosInf
};




// A SkipListKey represents a single key in a skip list.  It is possible
// to compare these keys using < or == operators.

template <typename ElementType>
class SkipListKey
{
public:
    SkipListKey(SkipListKind kind, const ElementType& element);

    bool operator==(const SkipListKey& other) const;
    bool operator<(const SkipListKey& other) const;

private:
    SkipListKind kind;
    ElementType element;
};

template <typename ElementType>
SkipListKey<ElementType>::SkipListKey(SkipListKind kind, const ElementType& element)
    : kind{kind}, element{element}
{
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const SkipListKey& other) const
{
    return kind == other.kind
        && (kind != SkipListKind::Normal || element == other.element);
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const SkipListKey& other) const
{
    switch (kind)
    {
    case SkipListKind::NegInf:
        return other.kind != SkipListKind::NegInf;

    case SkipListKind::PosInf:
        return false;

    default: // SkipListKind::Normal
        return other.kind == SkipListKind::PosInf
            || (other.kind == SkipListKind::Normal && element < other.element);
    }
}



// The SkipListLevelTester class represents the ability to decide whether
// a key placed on one level of the skip list should also occupy the next
// level. This is the "coin flip," so to speak.

template <typename ElementType>
class SkipListLevelTester
{
public:
    virtual ~SkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) = 0;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() = 0;
};


template <typename ElementType>
class RandomSkipListLevelTester : public SkipListLevelTester<ElementType>
{
public:
    RandomSkipListLevelTester();
    virtual ~RandomSkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) override;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() override;

private:
    std::default_random_engine engine;
    std::bernoulli_distribution distribution;
};


template <typename ElementType>
RandomSkipListLevelTester<ElementType>::RandomSkipListLevelTester()
    : engine{std::random_device{}()}, distribution{0.5}
{
}


template <typename ElementType>
bool RandomSkipListLevelTester<ElementType>::shouldOccupyNextLevel(const ElementType& element)
{
    return distribution(engine);
}


template <typename ElementType>
std::unique_ptr<SkipListLevelTester<ElementType>> RandomSkipListLevelTester<ElementType>::clone()
{
    return std::unique_ptr<SkipListLevelTester<ElementType>>{
        new RandomSkipListLevelTester<ElementType>};
}




template <typename ElementType>
class SkipListSet : public Set<ElementType>
{
public:
    // Initializes an SkipListSet to be empty, with or without a
    // "level tester" object that will decide, whenever a "coin flip"
    // is needed, whether a key should occupy the next level above.
    SkipListSet();
    explicit SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester);

    // Cleans up the SkipListSet so that it leaks no memory.
    virtual ~SkipListSet() noexcept;

    // Initializes a new SkipListSet to be a copy of an existing one.
    SkipListSet(const SkipListSet& s);

    // Initializes a new SkipListSet whose contents are moved from an
    // expiring one.
    SkipListSet(SkipListSet&& s) noexcept;

    // Assigns an existing SkipListSet into another.
    SkipListSet& operator=(const SkipListSet& s);

    // Assigns an expiring SkipListSet into another.
    SkipListSet& operator=(SkipListSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a SkipListSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect. This function runs in an expected time
    // of O(log n)
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in an expected time of O(log n)
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // levelCount() returns the number of levels in the skip list.
    unsigned int levelCount() const noexcept;


    // elementsOnLevel() returns the number of elements that are stored
    // on the given level of the skip list.  Level 0 is the bottom level;
    // level 1 is the one above level 0; and so on.  If the given level
    // doesn't exist, this function returns 0.
    unsigned int elementsOnLevel(unsigned int level) const noexcept;


    // isElementOnLevel() returns true if the given element is on the
    // given level, false otherwise.  Level 0 is the bottom level; level 1
    // is the one above level 0; and so on.  If the given level doesn't
    // exist, this function returns false.
    bool isElementOnLevel(const ElementType& element, unsigned int level) const;

private:
    // A unique pointer to a SkipListLevelTester object that randomizes a choice
    std::unique_ptr<SkipListLevelTester<ElementType>> levelTester;

    // A Node struct that creates a node object containing the key and the nodes below it and
    // next to it. It points to these nodes to create a linked-list of node objects or keys.
    struct Node
    {
        SkipListKey<ElementType>* value;
        Node* below;
        Node* next;

        Node(SkipListKey<ElementType>* v, Node* b, Node* n)
            : value(v), below(b), next(n)
        {
        }
    };

    // A LNode struct that creates a node object that represents the level. It contains an
    // unsigned int representing the current level and points to its linked-list of keys.
    // It also points to the level below it to connect it to the linked-list of levels.
    struct LNode
    {
        unsigned int value;
        LNode* below;
        Node* next;

        LNode(unsigned int v, LNode* b, Node* n)
            : value(v), below(b), next(n)
        {
        }
    };

    // The search() function searches for the element passed in as a parameter in the SkipList.
    // If it is found, it will return nothing, but if it is not found, it will return the node
    // where the element can be inserted next.
    Node* search(const ElementType& element, int minLevel) const;

    // The linkLevels() function copies the linked-list of levels from one SkipList to another.
    // It is a helper function for the copy constructor and copy assignment operator.
    void linkLevels(const SkipListSet& s, LNode*& list);

    // The linkNext() function copies the linked-list of keys for each level from one 
    // Skiplist to another. It is a helper function for copy construction and assignment.
    void linkNext(const SkipListSet& s, LNode*& list);

    // The linkBelow() function links all nodes to its match below it. It is a helper function
    // for the copy constructor and copy assignment operator.
    void linkBelow(const SkipListSet& s, LNode*& list);

    // The updateTail() function updates the tail pointer to point to the last node in te SkipList.
    void updateTail();

    // The createNewLevel() function creates a new node representing the next level above the
    // current level. It is a helper function for the add() function.
    void createNewLevel(const ElementType& element, Node*& current);

    // The insertNewNode() function creates a new node and inserts it into the appropriate
    // spot in the SkipList. It is a helper function for the add() function.
    Node* insertNewNode(Node*& n, Node*& current, const ElementType& element);

    // The getLevel() function returns the node of the level specified by the parameter.
    LNode* getLevel(unsigned int level) const;

    // The deleteSkipList() function deletes and deallocates all memory in the SkipList. It
    // calls the deleteNode() function to help with its deallocation.
    void deleteSkipList(LNode*& list) noexcept;

    // The deleteNode() function deletes and deallocates all memory in the SkipList. It is a 
    // helper function of deleteSkipList() as it focuses on deleting only the nodes each level
    // node points to.
    void deleteNode(Node*& node) noexcept;

    int elements; // an integer that holds the size of the SkipList

    LNode* head; // a pointer to the top level of the SkipList
    Node* tail; // a pointer to the end node of the top level of SkipList
};



template <typename ElementType>
SkipListSet<ElementType>::SkipListSet()
    : SkipListSet{std::make_unique<RandomSkipListLevelTester<ElementType>>()}
{
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester)
    : levelTester{std::move(levelTester)}
{
    try
    {
        ElementType temp;
        head = nullptr;
        head = new LNode(0, head, nullptr);
        head->next = new Node(new SkipListKey<ElementType>(SkipListKind::NegInf, temp), 
            nullptr, new Node(new SkipListKey<ElementType>(SkipListKind::PosInf, temp), 
            nullptr, nullptr));
        updateTail();
        elements = 0;
    }
    catch(...)
    {
        deleteSkipList(head);
        throw;
    }
}


template <typename ElementType>
SkipListSet<ElementType>::~SkipListSet() noexcept
{
    deleteSkipList(head);
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(const SkipListSet& s)
{
    try
    {
        linkLevels(s, head); // copy each level node
        linkNext(s, head);   // copy each key node
        linkBelow(s,head);   // copy each link to nodes below

        updateTail();
        elements = s.elements;
        levelTester = s.levelTester->clone();
    }
    catch(...)
    {
        deleteSkipList(head);
        throw;
    }
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(SkipListSet&& s) noexcept
{
    head = s.head;
    tail = s.tail;

    levelTester = s.levelTester->clone();
    elements = s.elements;

    s.head = nullptr;
    s.tail = nullptr;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(const SkipListSet& s)
{
    LNode* temp = nullptr;

    try
    {
        linkLevels(s, temp);  // copy each level node
        linkNext(s, temp);    // copy each key node
        linkBelow(s, temp);   // copy each link to nodes below

        deleteSkipList(head); // delete head for replacement since allocation was successful
        head = temp;
        temp = nullptr;
        delete temp;

        updateTail();
        elements = s.elements;
        levelTester = s.levelTester->clone();
    }
    catch(...)
    {
        deleteSkipList(temp);
        throw;
    }

    return *this;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(SkipListSet&& s) noexcept
{
    LNode* temp_list = head; // store the old list
    Node* temp_node = tail;  // store the old node

    head = s.head;           // swap for new list
    tail = s.tail;           // swap for new node

    levelTester = s.levelTester->clone();
    elements = s.elements;

    s.head = temp_list;      // swap for old list to delete
    s.tail = temp_node;      // swap for new node to delete

    return *this;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void SkipListSet<ElementType>::add(const ElementType& element)
{
    Node* current = search(element, 0); // check to see if there is a space in the list to add
    Node* temp = nullptr;

    try
    {
        temp = new Node(new SkipListKey<ElementType>(SkipListKind::Normal, element),
                        nullptr, current->next);
        current->next = temp; // add the new node into the bottom level
        elements++;           // increment the number of elements/size
    }
    catch(...)
    {
        delete temp;
        throw;
    }
   
    int currentLevel = 0;
    while (levelTester->shouldOccupyNextLevel(element) != 0) // while the levelTester returns 1
    {
        currentLevel++;
        if (currentLevel > head->value)                      // if a new level needs to be created
        {
            createNewLevel(element, temp);
            temp = head->next->next;      
        }
        else                                                 // if level already exists
        {
            Node* n = search(element, currentLevel);
            temp = insertNewNode(n, temp, element);
        }
    }
} 


template <typename ElementType>
bool SkipListSet<ElementType>::contains(const ElementType& element) const
{

    if (search(element, 0) == nullptr)
    {
        return true;
    }

    return false;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::size() const noexcept
{
    return elements;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::levelCount() const noexcept
{
    return head->value + 1;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::elementsOnLevel(unsigned int level) const noexcept
{
    LNode* list;
    list = getLevel(level);

    if (list == nullptr)
    {
        return 0;
    }

    int counter = 0;
    for (Node* n = list->next; n != nullptr; n = n->next)
    {
        counter++;
    }

    return counter - 2;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isElementOnLevel(const ElementType& element, unsigned int level) const
{
    LNode* list;
    list = getLevel(level);

    if (list == nullptr)
    {
        return false;
    }


    SkipListKey<ElementType> newKey(SkipListKind::Normal, element);

    for(Node* p = list->next; p != nullptr; p = p->next)
    {
        if (newKey == *p->value)
        {
            return true;
        }
    }

    return false;
}


template <typename ElementType>
typename SkipListSet<ElementType>::Node* SkipListSet<ElementType>::search(const ElementType& element,int minLevel) const
{
    Node* current = head->next;
    int level = head->value;

    SkipListKey<ElementType> newKey(SkipListKind::Normal, element);

    for (Node* p = current; p != nullptr; )
    {
        if (*p->value == newKey) // if found then return nullptr or nothing
        {
            return nullptr;
        }
        else if (p->next != nullptr && newKey < *current->next->value)
        {
            if (level - 1 <  minLevel) // if the minimum level is reached, then return current key
            {
                return current;
            }
            p = p->below;
            current = p;
            level--;
        }
        else
        {
            p = p->next;         // if next key is less, move to the next key
            current = p;
        }
    }   

    return current;
}


template <typename ElementType>
void SkipListSet<ElementType>::linkLevels(const SkipListSet& s, LNode*& list)
{
    LNode* current = nullptr;
    LNode* temp = nullptr; // temporary variable used to ensure successful allocation first
    
    for (LNode* p = s.head; p != nullptr; p = p->below)
    {
        if (current != nullptr) // add a new level if this is not the first level
        {
            try
            {
                temp = new LNode(p->value, nullptr, nullptr);
                current->below = temp;
                current = current->below;
            }
            catch(...)
            {
                deleteSkipList(temp);
                throw;
            }
        }
        else // add a new level if this is the first level added
        {
            try
            {
                temp = new LNode(p->value, nullptr, nullptr);
                list = temp;
                current = list;
            }
            catch(...)
            {
                deleteSkipList(temp);
                throw;
            }
        }

        temp = nullptr; // reset the temporary pointer
    }
    
}


template <typename ElementType>
void SkipListSet<ElementType>::linkNext(const SkipListSet& s, LNode*& list)
{
    LNode* current = list;
    Node* curr;
    Node* temp = nullptr; // temporary variable to ensure successful allocation first
 
    for (LNode* p = s.head; p != nullptr; p = p->below)
    {
        curr = nullptr;

        for (Node* n = p->next; n != nullptr; n = n->next)
        {
            if (curr != nullptr) // add a new key node if this not the first key node in the list
            {
                try
                {
                    temp = new Node(new SkipListKey(*n->value), nullptr, nullptr);
                    curr->next = temp;
                    curr = curr->next;
                }
                catch(...)
                {
                    deleteNode(temp);
                    throw;
                }
            }
            else // add a new key node if this is the first key node in the list 
            {
                try
                {
                    temp = new Node(new SkipListKey(*n->value), nullptr, nullptr);
                    current->next = temp;
                    curr = current->next;
                }
                catch(...)
                {
                    deleteNode(temp);
                    throw;
                }
            }
            
            temp = nullptr; // reset the temporary pointer
        }

        current = current->below; // go on to the level below to copy its list
    }
}


template <typename ElementType>
void SkipListSet<ElementType>::linkBelow(const SkipListSet& s, LNode*& list)
{
    LNode* current = list;
    Node* curr;

    while (current->below != nullptr)
    {
        curr = current->next;

        for (Node* p = current->below->next; p != nullptr; p = p->next)
        {
            if (*curr->value == *p->value) // if the node below it is the same, then link them
            {
                curr->below = p;
                curr = curr->next;
            }
        }

        current = current->below;
    }
}


template <typename ElementType>
void SkipListSet<ElementType>::updateTail()
{
    for (Node* p = head->next; p != nullptr; p = p->next)
    {
        if (p->next == nullptr)
        {
            tail = p;
        }
    }
}


template <typename ElementType>
void SkipListSet<ElementType>::createNewLevel(const ElementType& element, Node*& current)
{
    head = new LNode(head->value + 1, head, nullptr);
    head->next = new Node(new SkipListKey<ElementType>(SkipListKind::NegInf, ElementType()),
                          head->below->next, nullptr);
    head->next->next = new Node(new SkipListKey<ElementType>(SkipListKind::Normal, element),
                                current, nullptr);
    head->next->next->next = new Node(new SkipListKey<ElementType>(SkipListKind::PosInf, 
                                      ElementType()), tail, nullptr);
    updateTail();
}


template <typename ElementType>
typename SkipListSet<ElementType>::Node* SkipListSet<ElementType>::insertNewNode(Node*& n, Node*& current, const ElementType& element)
{
     SkipListKey<ElementType> newKey(SkipListKind::Normal, element);
     
     n->next = new Node(new SkipListKey<ElementType>(SkipListKind::Normal, element), 
                        current, n->next);

     return n->next;
}

template <typename ElementType>
typename SkipListSet<ElementType>::LNode* SkipListSet<ElementType>::getLevel(unsigned int level) const
{
    for (LNode* p = head; p != nullptr; p = p->below)
    {
        if (p->value == level)
        {
            return p;
        }
    }

    return nullptr;
}


template <typename ElementType>
void SkipListSet<ElementType>::deleteSkipList(LNode*& list) noexcept
{
    LNode* temp;

    for (LNode* p = list; p != nullptr; )
    {
        deleteNode(p->next);

        temp = p;
        p = p->below;
        delete temp;
    }
}


template <typename ElementType>
void SkipListSet<ElementType>::deleteNode(Node*& node) noexcept
{
    Node* t;

    for (Node* n = node; n != nullptr; )
    {
        t = n;
        delete n->value;
        n = n->next;
        delete t;
    }
}



#endif // SKIPLISTSET_HPP

