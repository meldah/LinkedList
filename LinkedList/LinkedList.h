#pragma once

#include <assert.h>

///
/// A single-linked list
///
template <class T>
class LinkedList {
public:
    ///
    /// Represents one box in a linked list
    ///
    class Box {
    public:
        T Data;     /// Data stored within the element
        Box* pNext; /// Pointer to the next element in the chain

    public:
        Box(const T & Data, Box *pNext = nullptr)
            : Data(Data), pNext(pNext)
        {
        }

        bool HasSuccessor() const
        {
            return pNext != nullptr;
        }
    };



    class Iterator {
    private:
        Box *pStart;   /// Pointer to the first box in the linked list
        Box *pCurrent; /// Current position of the iterator
    
    public:
        Iterator(Box *pStart)
            : pStart(pStart), pCurrent(pStart)
        {
        }

        ///
        /// Retrieves the value of the current element
        ///
        T const & GetCurrent() const
        {
            return pCurrent->Data;
        }

        ///
        /// Changes the value of the current element
        ///
        void SetCurrent(T const & Value)
        {
            pCurrent->Data = Value;
        }

        ///
        /// Moves the iterator one step forward
        ///
        void MoveNext()
        {
            if(pCurrent)
                pCurrent = pCurrent->pNext;
        }

        ///
        /// Returns the iterator to its initial state
        ///
        void Rewind()
        {
            pCurrent = pStart;
        }

        ///
        /// Checks whether the iterator has moved past the end of the list.
        ///
        bool EndReached() const
        {
            return pCurrent == nullptr;
        }
    };

    
private:
    Box *pFirst; /// Pointer to the first box in the linked list
    Box *pLast;  /// Pointer to the last box in the linked list
    size_t Size;    /// Number of elements in the linked list


public:
    ///
    /// Creates an empty list
    ///
    LinkedList()
    {
        ZeroVariables();
    }

    ///
    /// Creates a new list and copies in it the contents of another list
    ///
    LinkedList(LinkedList const & Other)
    {
        ZeroVariables();
        Append(Other);
    }
    
    ///
    /// Copies the contents of one list to another
    ///
    LinkedList& operator= (const LinkedList & Other)
    {
        if (this != &Other)
        {
            Clear();
            Append(Other);
        }

        return *this;
    }

    ///
    /// Destroys a list
    ///
    ~LinkedList()
    {
        Clear();
    }

private:
    void ZeroVariables()
    {
        pFirst = nullptr;
        pLast = nullptr;
        Size = 0;
    }
    
public:
    ///
    /// Checks whether the list is empty or not
    ///
    bool IsEmpty() const
    {
        return Size == 0;
    }

    ///
    /// Returns the number of elements in the list
    ///
    size_t GetSize() const
    {
        return Size;
    }

    ///
    /// Removes all elements of a list
    ///
    void Clear()
    {
        DestroyChain(pFirst);
        ZeroVariables();
    }

    ///
    /// Copies and appends the contents of another list to this one
    ///

    bool Append(LinkedList const & Other)
    {

        if (Other.IsEmpty())
            return true;

        Box *pNewChainStart, *pNewChainEnd;
        CloneChain(Other.pFirst, pNewChainStart, pNewChainEnd);

        if (!pNewChainStart)
            return false;

        if (Size == 0)
            pFirst = pNewChainStart;
        else
            pLast->pNext = pNewChainStart;

        pLast = pNewChainEnd;            

        Size += Other.Size;

        return pNewChainStart != nullptr;
    }


    ///
    /// Prepends a new element at the start of the list
    ///
    void PushFront(const T & Value)
    {
        Box *pNewBox = new Box(Value, pFirst);

        if (IsEmpty())
            pLast = pNewBox;

        pFirst = pNewBox;
        ++Size;
    }


    ///
    /// Appends a new element at the end of a list
    ///

    void PushBack(T const & Value)
    {
        Box *pNewBox = new Box(Value);

        if (IsEmpty())
        {
            pFirst = pLast = pNewBox;
        }
        else
        {
            this->pLast->pNext = pNewBox;
            this->pLast = pNewBox;
        }

        ++Size;
    }


    ///
    /// Removes the first element of a list
    ///
    void PopFront()
    {
        if (IsEmpty())
            return;
		
        Box *pOldHead = pFirst;

        pFirst = pFirst->pNext;

        if (pFirst == nullptr)
            pLast = nullptr;
        --Size;

        delete pOldHead;
    }


    ///
    /// Removes the last element of a list
    ///
    void PopBack()
    {
        if ( ! IsEmpty())
            RemoveAt(Size - 1);
    }

    T& Front()
    {
        if (Size == 0)
            throw std::exception();
        
        return pFirst->Data;
    }

    const T& Front() const
    {
        return const_cast<LinkedList<T>*>(this)->Front();
    }

    T& Back()
    {
        if (Size == 0)
            throw std::exception();

        return pLast->Data;
    }

    const T& Back() const
    {
        return const_cast<LinkedList<T>*>(this)->Back();
    }

    T & At(size_t Index)
    {
        if (Index >= Size)
            throw std::out_of_range("Index exceeds the number of elements in the list");

        return FindBoxAt(Index)->Data;
    }

    const T & At(size_t Index) const
    {
        if (Index >= Size)
            throw std::out_of_range("Index exceeds the number of elements in the list");

        return FindBoxAt(Index)->Data;
    }

    void RemoveAt(size_t Index)
    {

        if (Index >= Size)
            throw std::out_of_range("Index exceeds the number of elements in the list");

        if (Index == 0)
        {
            PopFront();
        }
        else
        {
            assert(this->GetSize() >= 2);

            Box *pBoxBeforeIndex = FindBoxAt(Index - 1);

            assert(pBoxBeforeIndex != nullptr);

            Box *pBoxAtIndex = pBoxBeforeIndex->pNext;

            if (Index == Size - 1)
                pLast = pBoxBeforeIndex;

            pBoxBeforeIndex->pNext = pBoxAtIndex->pNext;

            delete pBoxAtIndex;

            --Size;
        }
    }

    Iterator GetIterator() const
    {
        return Iterator(pFirst);
    }


private:
    Box* FindBoxAt(size_t Index) const
    {
        if (Index < 0 || Index >= this->Size)
            return nullptr;

        Box *p = pFirst;

        for (int i = 0; i < Index; ++i)
            p = p->pNext;

        return p;
    }
   
c void CloneChain(Box* pFirstBox, Box*& pCloneStart, Box*& pCloneEnd)
    {
        pCloneStart = nullptr;
        pCloneEnd = nullptr;

        if (pFirstBox == nullptr)
            return;

        Box* pNewChain = nullptr;

        try
        {
            // Pointer to the start of the new chain
            pNewChain = new Box(pFirstBox->Data); 

            // Pointer to the element that we have to copy next
            Box* pReadFrom = pFirstBox->pNext;

            // Pointer to the element after which we have to copy the new element
            Box* pWriteAfter = pNewChain;

            while (pReadFrom)
            {
                pWriteAfter->pNext = new Box(pReadFrom->Data);
                pWriteAfter = pWriteAfter->pNext;
                pReadFrom = pReadFrom->pNext;
            }

            pCloneStart = pNewChain;
            pCloneEnd = pWriteAfter;
        }
        catch (std::bad_alloc&)
        {
            DestroyChain(pNewChain);
        }
    }


    static void DestroyChain(Box* pFirstBox)
    {
        Box* pCurrent = pFirstBox;
        Box* pOld = pFirstBox;

        while (pCurrent)
        {
            pOld = pCurrent;
            pCurrent = pCurrent->pNext;
            delete pOld;
        }
    }

};

