#pragma once

#include <common.h>
#include <spm/system.h>
#include <wii/os.h>

namespace mod {

template<typename> class Stack;

template <typename T>
class StackNode {
friend class Stack<T>;
public:
    StackNode(T elem) : mElement(elem), mPrev(nullptr) {

    }
private:
    T mElement;
    StackNode* mPrev;
};

template <typename T>
class Stack {
public:
    Stack() : mHead(nullptr) {}

    inline bool isEmpty() {
        return mHead == nullptr;
    }

    void push(T elem) {
        StackNode<T>* node = new StackNode(elem);
        node->mPrev = mHead;
        mHead = node;
    }

    T pop() {
        assert(!isEmpty(), "ERROR: Tried to pop an empty stack !");
        StackNode<T>* curHead = mHead;
        T elem = mHead->mElement;
        mHead = mHead->mPrev;
        delete curHead;
        return elem;
    }

    T peek(s32 level = 0) {
        StackNode<T>* node = mHead;
        for (s32 i = 0; i < level; i++) {
            assert(!isEmpty(), "ERROR: Tried to peek at an empty stack !");
            node = node->mPrev;
        }
        return node->mElement;
    }

    void clear() {
        while (!isEmpty()) {
            (void)pop();
        }
    }
private:
    StackNode<T>* mHead;
};

}
