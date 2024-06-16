#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <vector>
#include <stdexcept>
#include <mutex>

template <typename T>
class CircularBuffer {
public:
    CircularBuffer(size_t size);
    void put(const T& item, bool overwrite = false);
    T get();
    T peek() const;
    void putBulk(const T* items, size_t count);
    void getBulk(T* items, size_t count);
    bool empty() const;
    bool isFull() const;
    size_t size() const;
    size_t capacity() const;
    void resize(size_t newSize);
    void clear();
    float fillLevel() const;

private:
    std::vector<T> buffer;
    size_t head;
    size_t tail;
    size_t maxSize;
    bool full;
    mutable std::mutex mtx;
};

#include "CircularBuffer.tpp"

#endif // CIRCULARBUFFER_H