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
        // change active and unprocessed to -1
        activeStart_.store(-1, std::memory_order_relaxed);
        unprocessedStart_.store(-1, std::memory_order_relaxed);
    }
    //adding some comment to test git action
    // check logic
    bool isEmpty() {
        return this->size() == 0;
    }

    // checks if freeStart_ is one index behind activeStart_ zone, if yes then queue is full
    bool isFull() {
        
        if (freeStart_.load(std::memory_order_relaxed) == activeStart_.load(std::memory_order_relaxed)) {
            return true;
        }
        return false;
    }

    // returns queue size
    uint64_t capacity(){
        return queue_.size();
    }

    // calculate size of activeStart_ zone to freeStart_ space
    uint64_t size() {
        uint64_t queue_size = 0; // change name

        // Initial case
        if (activeStart_.load(std::memory_order_relaxed) == -1) {
            return queue_size;
        }
        // when there is no rotation in queue
        if (freeStart_.load(std::memory_order_relaxed) > activeStart_.load(std::memory_order_relaxed)) {
            queue_size = freeStart_.load(std::memory_order_relaxed) - activeStart_.load(std::memory_order_relaxed);
        }
        // rotation i.e fossileStart_ < activeStart_
        else if (freeStart_.load(std::memory_order_relaxed) < activeStart_.load(std::memory_order_relaxed)) {
            queue_size = capacity() - activeStart_.load(std::memory_order_relaxed) + freeStart_.load(std::memory_order_relaxed);
        }
        else{ // when freeStart_ == activeStart_ (queue is full)
            return queue_.size();
        }
        return queue_size;
    }

    // Print the current state of queue
    void debug() {
        std::cout << "activeStart_: " << activeStart_.load(std::memory_order_relaxed) << " unprocessedStart_: " << unprocessedStart_.load(std::memory_order_relaxed) << " freeStart_: " << freeStart_.load(std::memory_order_relaxed) << " size: " << this->size() << std::endl;

        int i = unprocessedStart_.load(std::memory_order_relaxed);
        do{
            std::cout << queue_[i].receiveTime_ << " ";
            i = nextIndex(i);
        }while(i!=freeStart_.load(std::memory_order_relaxed));
        std::cout << std::endl;
        
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

    // enqueue the value
    void enqueue(Event value) {
        // increment freeStart_ index first for ABA. 
        // store freeStart_ index in a variable and use it in the function and then pass it next
        // this changes
        if (isFull()) {
            std::cout << "Queue is full, can't insert element." << std::endl;
            return;
        }

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
        freeStart_.store(nextIndex(freeStart_.load(std::memory_order_relaxed)), std::memory_order_relaxed);
    }

    // dequeue returns start of unprocessed. and increment it
    T dequeue() {
        if (isEmpty()) {
            std::cout << "Queue is empty, can't remove element." << std::endl;
            return T();
        }
        T value = queue_[unprocessedStart_.load(std::memory_order_relaxed)];
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
        if (nextIndex(activeStart_.load(std::memory_order_relaxed)) > unprocessedStart_.load(std::memory_order_relaxed)) {
            std::cout << "Cannot do that. activeStart goes in front of unprocessed" << std::endl;
            return -1;
        }
        activeStart_.store(nextIndex(activeStart_.load(std::memory_order_relaxed)), std::memory_order_relaxed);
        return activeStart_.load(std::memory_order_relaxed);
    }

    //increament active start upto a certain timestamp

    
};
