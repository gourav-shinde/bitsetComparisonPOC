#include <iostream>
#include <atomic>
#include <vector>
#include <cmath>




// T is the type of the elements in the queue
// comparator is function that compares two elements of type T 
// and returns true if the first element is smaller than the second
// T: DataType
// comparator: compare function which return A<B
template <typename T,typename comparator>
class UnifiedQueue {
private:
// put postfix _ for private variables
    std::vector<T>  data; //change to queue_
    std::atomic<int> activeStart_; //start of processed events ,activeStart_ zone
    std::atomic<int> unprocessedStart_; //start of unprocessedStart_ events 
    std::atomic<int> fossilStart_; //next available index, start of free space
    size_t capacity; 
    comparator compare;

public:

// read atomic documentation and make changes
    UnifiedQueue(size_t capacity=64) {
        data.resize(capacity);
        fossilStart_ = -1;
        activeStart_ = 0;
        unprocessedStart_ = 0;
        this->capacity = capacity;
    }

    // check logic
    bool isEmpty() {
        return this->getSize() == 0;
    }

    bool isFull() {

        if(nextIndex(fossilStart_)==activeStart_ && fossilStart_!=-1){
            return true;
        }
        return false;
    }

    // calculate size of activeStart_ zone to fossilStart_ space
    int getSize(){
        int size = 0;

        if(fossilStart_==-1){
            return size;
        }
        
        if (fossilStart_ > activeStart_){
            size = fossilStart_ - activeStart_;
        }
        else if(fossilStart_ < activeStart_){
            size = capacity - activeStart_ + fossilStart_;
        }
        return size;
    }
    
    void debug(){
        std::cout<<"activeStart_: "<<activeStart_<<" unprocessedStart_: "<<unprocessedStart_<<" fossilStart_: "<<fossilStart_<<" size: "<<this->getSize()<<std::endl;
        
        for (int i = activeStart_; i != fossilStart_ && fossilStart_ > -1 ; i=nextIndex(i) ) {
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
            return fossilStart_;

        while (low < high) {
            mid = ceil( ( low + high ) / 2 );

            if (this->compare(data[mid], value)){
                low = ( mid + 1 ) % capacity;
            }
            else {
                high = ( mid ) % capacity;  //very good chance for infinite loop
            }
        }

        return (low) % capacity;
    }

    int findInsertPosition(T value){
        int low = activeStart_;
        int high = fossilStart_;
        int mid;

        if (isEmpty())
            return fossilStart_;

        if(activeStart_<fossilStart_){
            return binarySearch(value,activeStart_,fossilStart_);
        }
        else{
            if(compare(value,data[capacity-1])){
                return binarySearch(value,activeStart_,capacity-1);
            }
            else{
                return binarySearch(value,0,fossilStart_);
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
        if (isFull() && fossilStart_ != -1) {
            std::cout << "Queue is full, can't insert element." << std::endl;
            return;
        }
        
        // checking for rollback
        if (isEmpty()) {
            fossilStart_ = activeStart_ = 0;
            data[fossilStart_] = value;
        } else {
            int insertPos = findInsertPosition(value);
            shiftElements(insertPos, fossilStart_);
            data[insertPos] = value;    
        }
        fossilStart_ = nextIndex(fossilStart_);
    }

    // dequeue from activeStart_ zone
    T dequeue() {
        if (isEmpty()) {
            std::cout << "Queue is empty, can't remove element." << std::endl;
            return T();
        }
        T value = data[activeStart_];
        activeStart_ = nextIndex(activeStart_);
        return value;
    }

    // get Indexes
    int getactiveStart_Index()   {
        return activeStart_;
    }

    int getUnprocessedStart_Index()   {
        return unprocessedStart_;
    }

    int getfossileStart_Index()   {
        return fossilStart_;
    }


    // set Indexes
    void setactiveStart_Index(int index)   {
        activeStart_ = index;
    }

    void setUnprocessedStart_Index(int index)   {
        unprocessedStart_ = index;
    }

    void setfossileStart_Index(int index)   {
        fossilStart_ = index;
    }

    // increament fossilStart_ index
    int increamentfossileStart_Index()   {
        if(nextIndex(fossilStart_)==activeStart_){
            std::cout << "Queue is full, can't insert element." << std::endl;
            return -1;
        }
        fossilStart_ = nextIndex(fossilStart_);
        return fossilStart_;
    }
    
};


