#include <iostream>
#include <atomic>
#include <vector>
#include <cmath>




// T is the type of the elements in the queue
// comparator is function that compares two elements of type T 
// and returns true if the first element is smaller than the second
template <typename T,typename comparator>
class UnifiedQueue {
private:
    std::vector<T> data;
    std::atomic<int> active; //start of processed events 
    std::atomic<int> unprocessed; //start of processed events 
    std::atomic<int> free; //next available index, start of free space
    int capacity;
    comparator compareFuntion;

public:
    UnifiedQueue(int capacity) {
        data.resize(capacity);
        free = -1;
        active = 0;
        unprocessed = 0;
        this->capacity = capacity;
    }

    bool isEmpty() {
        return this->getSize() == 0;
    }

    bool isFull() {
        if(nextIndex(free)==active){
            return true;
        }
        return false;
    }

    // calculate size of active zone to free space
    int getSize(){
        int size = 0;

        if(free==-1){
            return size;
        }
        
        if (free > active){
            size = free - active;
        }
        else if(free < active){
            size = capacity - active + free;
        }
        return size;
    }
    
    void debug(){
        std::cout<<"active: "<<active<<" unprocessed: "<<unprocessed<<" free: "<<free<<" size: "<<this->getSize()<<std::endl;
        
        for (int i = active; i != free && free > -1 ; i=nextIndex(i) ) {
            std::cout<<data[i].receiveTime_<<" ";
        }
        std::cout<<std::endl;

        for(auto itr:data){
            std::cout<<itr.receiveTime_<<" ";
        }
        std::cout<<std::endl;
    }

    int nextIndex(int idx) {
        return (idx + 1) % capacity;
    }

    int prevIndex(int idx) {
        return (idx - 1 + capacity) % capacity;
    }
  
    int binarySearch(T value, int low, int high) {
        int mid;
        
        if (isEmpty())
            return free;

        while (low < high) {
            mid = ceil( ( low + high ) / 2 );

            if (this->compareFuntion(data[mid], value)){
                low = ( mid + 1 ) % capacity;
            }
            else {
                high = ( mid ) % capacity;  //very good chance for infinite loop
            }
        }

        return (low) % capacity;
    }

    int findInsertPosition2(T value){
        int low = active;
        int high = free;
        int mid;

        if (isEmpty())
            return free;

        if(active<free){
            return binarySearch(value,active,free);
        }
        else{
            if(compareFuntion(value,data[capacity-1])){
                return binarySearch(value,active,capacity-1);
            }
            else{
                return binarySearch(value,0,free);
            }
        }

    }

    void shiftElements(int start, int end) {
        int i = end;
        while (i != start) {
            data[i] = data[prevIndex(i)];
            i = prevIndex(i);
        }
    }

    void enqueue(Event value) {
        if (isFull() && free != -1) {
            std::cout << "Queue is full, can't insert element." << std::endl;
            return;
        }
        
        // checking for rollback
        if (isEmpty()) {
            free = active = 0;
            data[free] = value;
        } else {
            int insertPos = findInsertPosition2(value);
            shiftElements(insertPos, free);
            data[insertPos] = value;    
        }
        free = nextIndex(free);
    }

    // dequeue from active zone
    T dequeue() {
        if (isEmpty()) {
            std::cout << "Queue is empty, can't remove element." << std::endl;
            return T();
        }
        T value = data[active];
        active = nextIndex(active);
        return value;
    }

    // get Indexes
    int getActiveIndex()   {
        return active;
    }

    int getUnprocessedIndex()   {
        return unprocessed;
    }

    int getFreeIndex()   {
        return free;
    }


    // set Indexes
    void setActiveIndex(int index)   {
        active = index;
    }

    void setUnprocessedIndex(int index)   {
        unprocessed = index;
    }

    void setFreeIndex(int index)   {
        free = index;
    }

    // increament free index
    int increamentFreeIndex()   {
        if(nextIndex(free)==active){
            std::cout << "Queue is full, can't insert element." << std::endl;
            return -1;
        }
        free = nextIndex(free);
        return free;
    }
    
};


