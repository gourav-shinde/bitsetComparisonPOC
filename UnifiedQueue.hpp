#include <iostream>
#include <atomic>
#include <vector>
#include <cmath>
#include <stdexcept>

// T is the type of the elements in the queue
// comparator is function that compare_s two elements of type T
// and returns true if the first element is smaller than the second
// T: queue_Type
// comparator: compare_ function which return A<B
template <typename T, typename comparator>
class UnifiedQueue
{
private:
    std::vector<T> queue_;
    std::atomic<int> activeStart_;      // start of processed events ,activeStart_ zone
    std::atomic<int> unprocessedStart_; // start of unprocessedStart_ events
    std::atomic<int> freeStart_;        // next available index, start of free space  rename to freespaceStart

    comparator compare_;

public:
    // read atomic documentation and make changes
    UnifiedQueue(size_t capacity = 64) {
        queue_.resize(capacity);
        // change free start to 0
        freeStart_.store(0, std::memory_order_relaxed); // memory order relaxed, same for all 3
        // change active and unprocessed to INT32_MIN
        activeStart_.store(INT32_MIN, std::memory_order_relaxed);
        unprocessedStart_.store(INT32_MIN, std::memory_order_relaxed);
    }
    //adding some comment to test git action
    // check logic
    bool isEmpty() {
        return this->size() == 0;
    }

    // checks if freeStart_ is one index behind activeStart_ zone, if yes then queue is full
    bool isFull() {
        //when freeStart_ is -ve, queue is full
        if (freeStart_.load(std::memory_order_relaxed) < 0) {
            return true;
        }
        return false;
    }

    // returns queue size
    uint64_t capacity() {
        return queue_.size();
    }

    // calculate size of activeStart_ zone to freeStart_ space
    uint64_t size() {
        uint64_t queue_size = 0; // change name

        // Initial case
        if ( activeStart_.load( std::memory_order_relaxed) == INT32_MIN ) {
            return queue_size;
        }
        else if( freeStart_.load(std::memory_order_relaxed) < 0 ){
            return queue_.size(); //when queue is full
        }
        // when there is no rotation in queue
        else if (getValidIndex(freeStart_.load(std::memory_order_relaxed)) > getValidIndex(activeStart_.load(std::memory_order_relaxed))) {
            queue_size = getValidIndex(freeStart_.load(std::memory_order_relaxed)) - getValidIndex(activeStart_.load(std::memory_order_relaxed));
        }
        // rotation i.e fossileStart_ < activeStart_
        else if (getValidIndex(freeStart_.load(std::memory_order_relaxed)) < getValidIndex(activeStart_.load(std::memory_order_relaxed))) {
            queue_size = capacity() - getValidIndex(activeStart_.load(std::memory_order_relaxed)) + getValidIndex(freeStart_.load(std::memory_order_relaxed));
        }
        else if (activeStart_.load(std::memory_order_relaxed) == freeStart_.load(std::memory_order_relaxed)) {
            return 0;//queue is empty , this is when freeStart_ == activeStart_ == unprocessedStart_
        }
        else { // when freeStart_ == activeStart_ (queue is full)
            return queue_.size();
        }
        return queue_size;

        //
    }

    // Print the current state of queue
    void debug() {
        std::cout << "activeStart_: " << activeStart_.load(std::memory_order_relaxed) << " unprocessedStart_: " << unprocessedStart_.load(std::memory_order_relaxed) << " freeStart_: " << freeStart_.load(std::memory_order_relaxed) << " size: " << this->size() << std::endl;

        int i = (unprocessedStart_.load(std::memory_order_relaxed)<0)?-1:unprocessedStart_.load(std::memory_order_relaxed);
        if(i!=-1){
        do {
            std::cout << queue_[i].receiveTime_ << " ";
            i = nextIndex(i);
        } while (i != getValidIndex(abs(freeStart_.load(std::memory_order_relaxed))));
        std::cout << std::endl;
        }
        else{
            std::cout<<"Unprocessed Events Empty"<<std::endl;
        }
        for (auto itr : queue_) {
            std::cout << itr.receiveTime_ << " ";
        }
        std::cout << std::endl;
    }

    // doesnt access indexes directly
    int nextIndex(int idx) {
        return (idx + 1) % capacity();
    }

    // doesnt access indexes directly
    int prevIndex(int idx) {
        return (idx - 1 + capacity()) % capacity();
    }

    // find element smaller than value
    int binarySearch(T value, int low, int high) {
        int mid;

        // THis will never trigger, as this condition is checked in the parent function
        if (isEmpty())
            return freeStart_.load(std::memory_order_relaxed);

        while (low < high) {
            mid = ceil((low + high) / 2);

            if (this->compare_(queue_[mid], value)) {
                low = (mid + 1) % capacity();
            }
            else {
                high = (mid) % capacity(); // very good chance for infinite loop
            }
        }

        return (low) % capacity();
    }

    // find insert position for value depending whether queue is rotated or not
    int findInsertPosition(T value) {
        int low = activeStart_.load(std::memory_order_relaxed);
        int high = freeStart_.load(std::memory_order_relaxed);
        int mid;

        if (isEmpty())
            return freeStart_.load(std::memory_order_relaxed);

        // when there is no rotation in queue
        if (activeStart_.load(std::memory_order_relaxed) < freeStart_.load(std::memory_order_relaxed)) {
            return binarySearch(value, activeStart_.load(std::memory_order_relaxed), freeStart_.load(std::memory_order_relaxed));
        }
        // rotation i.e fossileStart_ < activeStart_
        else {
            if (compare_(value, queue_[capacity() - 1])) {
                return binarySearch(value, activeStart_.load(std::memory_order_relaxed), capacity() - 1);
            }
            else {
                return binarySearch(value, 0, freeStart_.load(std::memory_order_relaxed));
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

    //modulo for index
    int getValidIndex(int index) {
        return abs(index) % capacity();
    }

    // enqueue the value
    void enqueue(T value) {
        // increment freeStart_ index first for ABA. 
        // store freeStart_ index in a variable and use it in the function and then pass it next
        // this changes

        //preprocess freeStart
        //check if freeStart is -ve  or not.
        //this includes doing modulo operation and checking for is queue full or not.

        if (isFull()) {
            std::cout << "Queue is full, can't insert element." << std::endl;
            return;
        }
        freeStart_.store(freeStart_.load(std::memory_order_relaxed) % capacity(), std::memory_order_relaxed);
        unprocessedStart_.store(getValidIndex(unprocessedStart_.load(std::memory_order_relaxed)), std::memory_order_relaxed);
        // checking for rollback
        if (isEmpty()) {
            // make unprocessed and active to 0
            unprocessedStart_.store(0, std::memory_order_relaxed);
            activeStart_.store(0, std::memory_order_relaxed);
            queue_[freeStart_.load(std::memory_order_relaxed)] = value;
        }
        else {
            int insertPos = findInsertPosition(value);
            shiftElements(insertPos, freeStart_.load(std::memory_order_relaxed));
            queue_[insertPos] = value;
        }
        //check if freeStart+1 == activeStart, if then freeStart = -(freeStart++), else freeStart++
        //do modulo on freeStart when doing the insert.
        if (nextIndex(freeStart_.load(std::memory_order_relaxed)) == activeStart_.load(std::memory_order_relaxed)) {
            freeStart_.store(-(freeStart_.load(std::memory_order_relaxed)+1), std::memory_order_relaxed);
        }
        else {
            freeStart_.store((freeStart_.load(std::memory_order_relaxed)+1), std::memory_order_relaxed);
        }
        //not using nextIndex() here as we want to do module at start of the function 
    }

    // dequeue returns start of unprocessed. and increment it
    T dequeue() {
        if (isEmpty()) { //when whole queue is empty
            std::cout << "Queue is empty, can't remove element." << std::endl;
            return T();
        }

        if (unprocessedStart_.load(std::memory_order_relaxed) < 0) {
            std::cout << "Unprocessed Events Empty" << std::endl;
            return T();
        }
        //when activeStart_ is -ve, it means activeStart_ is empty, so with dequeue we are adding a element to it
        activeStart_.store(getValidIndex(activeStart_.load(std::memory_order_relaxed)), std::memory_order_relaxed);

        T value = queue_[unprocessedStart_.load(std::memory_order_relaxed)];
        
        if(nextIndex(unprocessedStart_.load(std::memory_order_relaxed)) == getValidIndex(freeStart_.load(std::memory_order_relaxed))){
          //this means unprocessed list is empty
            unprocessedStart_.store(-(unprocessedStart_.load(std::memory_order_relaxed)+1), std::memory_order_relaxed);
            return value;
        }

        
        unprocessedStart_.store(nextIndex(unprocessedStart_.load(std::memory_order_relaxed)), std::memory_order_relaxed);
        return value;
    }

    // get Indexes
    int getactiveStart_Index() {
        return activeStart_.load(std::memory_order_relaxed);
    }

    int getUnprocessedStart_Index() {
        return unprocessedStart_.load(std::memory_order_relaxed);
    }

    int getfossileStart_Index() {
        return freeStart_.load(std::memory_order_relaxed);
    }

    // set Indexes
    void setactiveStart_Index(int index) {
        activeStart_.store(index, std::memory_order_relaxed);
    }

    void setUnprocessedStart_Index(int index) {
        unprocessedStart_.store(index, std::memory_order_relaxed);
    }

    void setfossileStart_Index(int index) {
        freeStart_.store(index, std::memory_order_relaxed);
    }

    // increament freeStart_ index
    int increamentfreeStart_Index() {
        if (nextIndex(freeStart_.load(std::memory_order_relaxed)) == activeStart_.load(std::memory_order_relaxed)) {
            std::cout << "Queue is full, can't insert element." << std::endl;
            return -1;
        }
        freeStart_.store(nextIndex(freeStart_.load(std::memory_order_relaxed)), std::memory_order_relaxed);
        return freeStart_.load(std::memory_order_relaxed);
    }


    // increament freeStart_ index
    int increamentActiveStart_Index() {
        //check if freeStart -ve, update with abs(freeStart), module operation too
        if(activeStart_.load(std::memory_order_relaxed) == INT32_MIN){
            std::cout << "Cannot do that. activeStart it is inital condition" << std::endl;
            return -1;
        }
        else if(isEmpty()){
            std::cout << "Cannot do that. Whole queue is empty" << std::endl;
            return -1;
        }
        else if (freeStart_.load(std::memory_order_relaxed) < 0) {
            freeStart_.store(getValidIndex(abs(freeStart_.load(std::memory_order_relaxed))), std::memory_order_relaxed);
        }
        else if(activeStart_.load(std::memory_order_relaxed) < 0){
            std::cout << "Cannot do that. activeStart is -ve means its empty" << std::endl;
            return -1;
        }
        else if ((nextIndex(activeStart_.load(std::memory_order_relaxed))) == unprocessedStart_.load(std::memory_order_relaxed)) {
            activeStart_.store(-(activeStart_.load(std::memory_order_relaxed)+1), std::memory_order_relaxed);
            return -1;
        }
        else{

        }
        activeStart_.store(nextIndex(activeStart_.load(std::memory_order_relaxed)), std::memory_order_relaxed);
        return activeStart_.load(std::memory_order_relaxed);
    }

    //increament active start upto a certain timestamp


};
