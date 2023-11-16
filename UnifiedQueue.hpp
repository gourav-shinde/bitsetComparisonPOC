#include <iostream>
#include <atomic>
#include <vector>
#include <cmath>




// T is the type of the elements in the queue
// comparator is function that compare_s two elements of type T 
// and returns true if the first element is smaller than the second
// T: queue_Type
// comparator: compare_ function which return A<B
template <typename T,typename comparator>
class UnifiedQueue {
private:
// put postfix _ for private variables
    std::vector<T>  queue_; //change to queue_
    std::atomic<int> activeStart_; //start of processed events ,activeStart_ zone
    std::atomic<int> unprocessedStart_; //start of unprocessedStart_ events 
    std::atomic<int> fossilStart_; //next available index, start of free space
    size_t capacity_; 
    comparator compare_;

public:

// read atomic documentation and make changes
    UnifiedQueue(size_t capacity=64) {
        queue_.resize(capacity);
        fossilStart_.store(-1);
        activeStart_.store(0);
        unprocessedStart_.store(0);
        this->capacity_ = capacity;
    }

    // check logic
    bool isEmpty() {
        return this->getSize() == 0;
    }

    // checks if fossilStart_ is one index behind activeStart_ zone, if yes then queue is full
    bool isFull() {
        if(nextIndex(fossilStart_.load())==activeStart_.load() && fossilStart_.load()!=-1){
            return true;
        }
        return false;
    }

    // calculate size of activeStart_ zone to fossilStart_ space
    int getSize(){
        int size = 0;

        // Initial case
        if(fossilStart_==-1){
            return size;
        }
        // when there is no rotation in queue
        if (fossilStart_ > activeStart_){
            size = fossilStart_ - activeStart_;
        }
        // rotation i.e fossileStart_ < activeStart_
        else if(fossilStart_ < activeStart_){
            size = capacity_ - activeStart_ + fossilStart_;
        }
        return size;
    }
    
    // Print the current state of queue
    void debug(){
        std::cout<<"activeStart_: "<<activeStart_.load()<<" unprocessedStart_: "<<unprocessedStart_.load()<<" fossilStart_: "<<fossilStart_.load()<<" size: "<<this->getSize()<<std::endl;
        
        for (int i = activeStart_.load(); i != fossilStart_.load() && fossilStart_.load() > -1 ; i=nextIndex(i) ) {
            std::cout<<queue_[i].receiveTime_<<" ";
        }
        std::cout<<std::endl;

        for(auto itr:queue_){
            std::cout<<itr.receiveTime_<<" ";
        }
        std::cout<<std::endl;
    }

    // doesnt access indexes directly
    int nextIndex(int idx) {
        return (idx + 1) % capacity_;
    }

    // doesnt access indexes directly
    int prevIndex(int idx) {
        return (idx - 1 + capacity_) % capacity_;
    }
  
    // find element smaller than value
    int binarySearch(T value, int low, int high) {
        int mid;
        
        // THis will never trigger, as this condition is checked in the parent function
        if (isEmpty())
            return fossilStart_.load();

        while (low < high) {
            mid = ceil( ( low + high ) / 2 );

            if (this->compare_(queue_[mid], value)){
                low = ( mid + 1 ) % capacity_;
            }
            else {
                high = ( mid ) % capacity_;  //very good chance for infinite loop
            }
        }

        return (low) % capacity_;
    }

    // find insert position for value depending whether queue is rotated or not
    int findInsertPosition(T value){
        int low = activeStart_.load();
        int high = fossilStart_.load();
        int mid;

        if (isEmpty())
            return fossilStart_.load();

        // when there is no rotation in queue
        if(activeStart_.load() < fossilStart_.load()){
            return binarySearch(value,activeStart_.load(),fossilStart_.load());
        }
        // rotation i.e fossileStart_ < activeStart_
        else{
            if(compare_(value,queue_[capacity_-1])){
                return binarySearch(value,activeStart_.load(),capacity_-1);
            }
            else{
                return binarySearch(value,0,fossilStart_.load());
            }
        }

    }

    // shift elements to right
    void shiftElements(int start, int end) {
        int i = end;
        while (i != start) {
            queue_[i] = queue_[prevIndex(i)];
            i = prevIndex(i);
        }
    }

    // enqueue the value
    void enqueue(Event value) {
        if (isFull() && fossilStart_ != -1) {
            std::cout << "Queue is full, can't insert element." << std::endl;
            return;
        }
        
        // checking for rollback
        if (isEmpty()) {
            fossilStart_.store(0);
            activeStart_.store(0);
            queue_[fossilStart_.load()] = value;
        } else {
            int insertPos = findInsertPosition(value);
            shiftElements(insertPos, fossilStart_.load());
            queue_[insertPos] = value;    
        }
        fossilStart_.store(nextIndex(fossilStart_.load()));
    }

    // dequeue from activeStart_ zone
    T dequeue() {
        if (isEmpty()) {
            std::cout << "Queue is empty, can't remove element." << std::endl;
            return T();
        }
        T value = queue_[activeStart_.load()];
        activeStart_ = nextIndex(activeStart_.load());
        return value;
    }

    // get Indexes
    int getactiveStart_Index()   {
        return activeStart_.load();
    }

    int getUnprocessedStart_Index()   {
        return unprocessedStart_.load();
    }

    int getfossileStart_Index()   {
        return fossilStart_.load();
    }


    // set Indexes
    void setactiveStart_Index(int index)   {
        activeStart_.store(index);
    }

    void setUnprocessedStart_Index(int index)   {
        unprocessedStart_.store(index);
    }

    void setfossileStart_Index(int index)   {
        fossilStart_.store(index);
    }

    // increament fossilStart_ index
    int increamentfossileStart_Index()   {
        if(nextIndex(fossilStart_.load())==activeStart_.load()){
            std::cout << "Queue is full, can't insert element." << std::endl;
            return -1;
        }
        fossilStart_.store(nextIndex(fossilStart_.load()));
        return fossilStart_.load();
    }
    
};


