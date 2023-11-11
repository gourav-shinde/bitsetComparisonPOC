#include <iostream>
#include <vector>
#include <math.h>
#include "EventClass.hpp"
// unified queue 
// default size 128
class CombinedQueue {
private:
    std::vector<Event> data;
    
    // todo atomic
    // go through folly/boost for DS
    int head; //start of processed events
    int tail; //next available index, start of free space
    int size;
    int capacity;//remove and add .size() function using head and tail
    int first_unprocessedEvent;
    int first_scheduledEvent; //processed Event
    int fossil_collection_index;
    // start of active zone yellow area 0 start
    // start of fossilized zone grey area -1 if no fossilized zone
    // start of unprocessed events start 0

    // when doing fossil collection update start of active area
    //fetching we increament unprocessed event
    // start of active zone index, start of free space index,
    //  start of unprocessed eventset
    // getter/setter of markers

// init values a active,f free,u unprocessed
// a=f=u=0
//when inserting f++
//when after processing Event u++ 
public:
    CombinedQueue(int capacity) {
        data.resize(capacity);
        head = 0;
        tail = -1;
        size = 0;
        this->capacity = capacity;
    }

    bool isEmpty() {
        return size == 0;
    }

    bool isFull() {
        return size == capacity;
    }
    
    void debug(){
        std::cout<<"head: "<<head<<" tail: "<<tail<<" size: "<<size<<" capacity: "<<data.size()<<std::endl;
        for (int i = tail; i != head+1; i=(i+1)%capacity) {
            std::cout<<data[i].receiveTime_<<" ";
        }
        std::cout<<std::endl;
    }

    int nextIndex(int idx) {
        return (idx + 1) % capacity;
    }

    int prevIndex(int idx) {
        return (idx - 1 + capacity) % capacity;
    }
  
    int findInsertPosition(Event value) {
        int low = tail;
        int high = head;
        int mid;
        compareEvent compareEvent;
        if (isEmpty())
            return 0;

        while (low <= high) {
            mid = (low + high) / 2;

            if (compareEvent(data[mid], value)){
                low = (mid + 1) % capacity;
            }
            else if (compareEvent(value, data[mid])){
                high = (mid - 1 + capacity) % capacity;
            }
            else{
                return mid;
            }
        }

        return (low) % capacity;
    }

    void shiftElements(int start, int end) {
        int i = nextIndex(end);
        while (i != start) {
            data[i] = data[prevIndex(i)];
            i = prevIndex(i);
        }
    }

    void enqueue(Event value) {
        if (isFull()) {
            std::cout << "Queue is full, can't insert element." << std::endl;
            return;
        }

        int insertPos = findInsertPosition(value);
        // checking for rollback
        if (isEmpty()) {
            head = tail = 0;
            data[tail] = value;
        } else {
            if (insertPos == head) {
                shiftElements(insertPos, head);
                data[insertPos] = value;
                
            } else {
                shiftElements(insertPos, head);
                data[insertPos] = value;
                
            }
        head = (head + 1) % capacity;
        }

        size++;
    }

    void dequeue() {
        if (isEmpty()) {
            std::cout << "Queue is empty, can't dequeue element." << std::endl;
            return;
        }
        

        tail = (tail + 1) % capacity;
        size--;
    }

    
};


