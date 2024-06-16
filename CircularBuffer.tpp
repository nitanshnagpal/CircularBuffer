#include "CircularBuffer.h"

template <typename T>
CircularBuffer<T>::CircularBuffer(size_t size)
    : buffer(size), maxSize(size), head(0), tail(0), full(false) {}

template <typename T>
void CircularBuffer<T>::put(const T& item, bool overwrite) {
    std::lock_guard<std::mutex> lock(mtx);
    if (full && !overwrite) {
        throw std::runtime_error("Buffer is full and overwrite is not allowed");
    }
    buffer[head] = item;
    if (full) {
        tail = (tail + 1) % maxSize;
    }
    head = (head + 1) % maxSize;
    full = head == tail;
}

template <typename T>
T CircularBuffer<T>::get() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!full && (head == tail)) {
        throw std::runtime_error("Buffer is empty");
    }
    T item = buffer[tail];
    full = false;
    tail = (tail + 1) % maxSize;
    return item;
}

template <typename T>
T CircularBuffer<T>::peek() const {
    std::lock_guard<std::mutex> lock(mtx);
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    return buffer[tail];
}

template <typename T>
void CircularBuffer<T>::putBulk(const T* items, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        put(items[i]);
    }
}

template <typename T>
void CircularBuffer<T>::getBulk(T* items, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        items[i] = get();
    }
}

template <typename T>
bool CircularBuffer<T>::empty() const {
    std::lock_guard<std::mutex> lock(mtx);
    return (!full && (head == tail));
}

template <typename T>
bool CircularBuffer<T>::isFull() const {
    std::lock_guard<std::mutex> lock(mtx);
    return full;
}

template <typename T>
size_t CircularBuffer<T>::size() const {
    std::lock_guard<std::mutex> lock(mtx);
    size_t size = maxSize;
    if (!full) {
        if (head >= tail) {
            size = head - tail;
        } else {
            size = maxSize + head - tail;
        }
    }
    return size;
}

template <typename T>
size_t CircularBuffer<T>::capacity() const {
    std::lock_guard<std::mutex> lock(mtx);
    return maxSize;
}

template <typename T>
void CircularBuffer<T>::resize(size_t newSize) {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<T> newBuffer(newSize);
    size_t currentSize = size();
    for (size_t i = 0; i < currentSize; ++i) {
        newBuffer[i] = get();
    }
    buffer = std::move(newBuffer);
    maxSize = newSize;
    head = currentSize % maxSize;
    tail = 0;
    full = (head == tail);
}

template <typename T>
void CircularBuffer<T>::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    head = 0;
    tail = 0;
    full = false;
}

template <typename T>
float CircularBuffer<T>::fillLevel() const {
    std::lock_guard<std::mutex> lock(mtx);
    return static_cast<float>(size()) / maxSize;
}
