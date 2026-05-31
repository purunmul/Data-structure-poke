#ifndef STACK_H
#define STACK_H

#include <iostream>

// Linked-node stack. Used for movement history and undo.
template <typename T>
class Stack {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& value) : value(value), next(nullptr) {}
    };

    Node* topNode;
    int count;

public:
    Stack() : topNode(nullptr), count(0) {}

    ~Stack() {
        clear();
    }

    Stack(const Stack& other) = delete;
    Stack& operator=(const Stack& other) = delete;

    bool isEmpty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }

    void push(const T& value) {
        // TODO(Student): Implement stack push.
        // Hint: allocate a new Node, link it to the current top, then update topNode.
        (void)value;
        std::cout << "[미구현] Stack::push 함수가 아직 구현되지 않았습니다.\n";
    }

    bool pop(T& output) {
        // TODO(Student): Implement stack pop.
        // Return false if the stack is empty. Otherwise copy the top value into output,
        // unlink the top node, delete it, and return true.
        (void)output;
        std::cout << "[미구현] Stack::pop 함수가 아직 구현되지 않았습니다.\n";
        return false;
    }

    bool peek(T& output) const {
        // TODO(Student): Implement stack peek.
        // Return false if empty. Otherwise copy the top value into output without removing it.
        (void)output;
        return false;
    }

    void clear() {
        // Provided cleanup helper: delete every node and reset topNode/count.
        while (topNode != nullptr) {
            Node* old = topNode;
            topNode = topNode->next;
            delete old;
        }
        count = 0;
    }
};

#endif
