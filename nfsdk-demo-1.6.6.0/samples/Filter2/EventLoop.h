#pragma once
#include "MessageQueue.h"

class EventLoop {
public:
    EventLoop(MessageQueue& queue) : queue(queue), running(true) {}

    void run() {
        while (running) {
            Message message = queue.pop();
            if (message.getContent() == "STOP") {
                std::cout << "Processing STOP message\n" ;
                break;
            }
            processMessage(message);
        }
    }

    void stop() {
        running = false;
    }

private:
    void processMessage(const Message& message) {
        std::cout << "Processing message: " << message.getContent() << std::endl;
    }

    MessageQueue& queue;
    bool running;
};



