#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

class Message {
public:
    explicit Message(const std::string& content) : content(content) {}
    std::string getContent() const { return content; }

private:
    std::string content;
};

class MessageQueue {
public:
    Message pop() {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty()) {
            cond_.wait(mlock);
        }
        auto item = queue_.front();
        queue_.pop();
        return item;
    }

    void push(const Message& item) {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }

private:
    std::queue<Message> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};



