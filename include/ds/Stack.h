#ifndef STACK_H
#define STACK_H

#include <iostream>

// 연결 리스트 기반 스택: 이동 기록 및 Undo 기능에 사용 -dc (Gun Kim 구현)
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

    // 새로운 데이터를 스택 최상단에 삽입
    void push(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    // 최상단 데이터를 꺼내고 메모리 해제 (성공 시 true)
    bool pop(T& output) {
        if (isEmpty()) {
            return false;
        }
        Node* temp = topNode;
        output = topNode->value;
        topNode = topNode->next;
        delete temp;
        count--;
        return true;
    }

    // 데이터를 꺼내지 않고 값만 확인
    bool peek(T& output) const {
        if (isEmpty()) {
            return false;
        }
        output = topNode->value;
        return true;
    }

    void clear() {
        while (topNode != nullptr) {
            Node* old = topNode;
            topNode = topNode->next;
            delete old;
        }
        count = 0;
    }
};

#endif
