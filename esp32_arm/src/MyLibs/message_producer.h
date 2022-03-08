#include "message_queue.h"

// This is a local message producer
// Normally, this producer 
//     1. Take message from rabbitMQ or MQTT
//     2. Act as a message producer, put the message to local message queue.
class MessageProducer{
    public:
        // virtual void AppendMessageToLocalMQ(const char* message, int length);
        void LinkLocalMessageQueue(MessageQueue* mq){this->__messageQueue = mq;};
    protected:
        // GcodeQueue* _commandQueue;
        MessageQueue* __messageQueue;

    private:


};