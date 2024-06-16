#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <sstream>
#include "CircularBuffer.h"

const int PRODUCER_COUNT = 3;
const int CONSUMER_COUNT = 3;
const int ITEM_COUNT = 10;

void producer(CircularBuffer<int>& buffer, int id) {
    for (int i = 0; i < ITEM_COUNT; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
        buffer.put(i + id * 100); // Unique item value for each producer
        std::stringstream msg;
        msg << "Producer " << id << " produced: " << i + id * 100 << std::endl;
        std::cout<<msg.str();
    }
}

void consumer(CircularBuffer<int>& buffer, int id) {
    for (int i = 0; i < ITEM_COUNT; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Simulate work
        std::stringstream msg;
        try {
            int item = buffer.get();
            msg<< "Consumer " << id << " consumed: " << item << std::endl;
            std::cout<<msg.str();
        } catch (const std::runtime_error& e) {
            msg << "Consumer " << id << " failed to consume: " << e.what() << std::endl;
            std::cout<<msg.str();
        }
    }
}

int main() {
    CircularBuffer<int> buffer(5);

    // Create producer threads
    std::vector<std::thread> producers;
    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        producers.emplace_back(producer, std::ref(buffer), i);
    }

    // Create consumer threads
    std::vector<std::thread> consumers;
    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        consumers.emplace_back(consumer, std::ref(buffer), i);
    }

    // Wait for all threads to finish
    for (auto& producer : producers) {
        producer.join();
    }
    for (auto& consumer : consumers) {
        consumer.join();
    }

    return 0;
}
