#include <iostream>
#include <atomic>
#include <vector>
#include <cmath>

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

    size_t capacity_;
    comparator compare_;

public:
    // read atomic documentation and make changes
    UnifiedQueue(size_t capacity = 64) {
        queue_.resize(capacity); // use vector.reserve
        // change free start to 0
        freeStart_.store(-1); // memory order relaxed, same for all 3
        // change active and unprocessed to -1
        activeStart_.store(0);
        unprocessedStart_.store(0);
        this->capacity_ = capacity;
    }

    // check logic
    bool isEmpty() {
        return this->size() == 0;
    }

    // checks if freeStart_ is one index behind activeStart_ zone, if yes then queue is full
    bool isFull() {
        // TODO TEST
        if (activeStart_.load() != -1 && nextIndex(freeStart_.load()) == activeStart_.load()) {
            return true;
        }
        return false;
    }

    // calculate size of activeStart_ zone to freeStart_ space
    uint64_t size() {
        uint64_t queue_size = 0; // change name

        // Initial case
        if (freeStart_ == -1) {
            return queue_size;
        }
        // when there is no rotation in queue
        if (freeStart_ > activeStart_) {
            queue_size = freeStart_ - activeStart_;
        }
        // rotation i.e fossileStart_ < activeStart_
        else if (freeStart_ < activeStart_) {
            queue_size = capacity_ - activeStart_ + freeStart_;
        }
        return queue_size;
    }

    // Print the current state of queue
    void debug() {
        std::cout << "activeStart_: " << activeStart_.load() << " unprocessedStart_: " << unprocessedStart_.load() << " freeStart_: " << freeStart_.load() << " size: " << this->size() << std::endl;

        for (int i = unprocessedStart_.load(); i != freeStart_.load() && freeStart_.load() > -1; i = nextIndex(i)) {
            std::cout << queue_[i].receiveTime_ << " ";
        }
        std::cout << std::endl;

        for (auto itr : queue_) {
            std::cout << itr.receiveTime_ << " ";
        }
        std::cout << std::endl;
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
            return freeStart_.load();

        while (low < high) {
            mid = ceil((low + high) / 2);

            if (this->compare_(queue_[mid], value)) {
                low = (mid + 1) % capacity_;
            }
            else {
                high = (mid) % capacity_; // very good chance for infinite loop
            }
        }

        return (low) % capacity_;
    }

    // find insert position for value depending whether queue is rotated or not
    int findInsertPosition(T value) {
        int low = activeStart_.load();
        int high = freeStart_.load();
        int mid;

        if (isEmpty())
            return freeStart_.load();

        // when there is no rotation in queue
        if (activeStart_.load() < freeStart_.load()) {
            return binarySearch(value, activeStart_.load(), freeStart_.load());
        }
        // rotation i.e fossileStart_ < activeStart_
        else {
            if (compare_(value, queue_[capacity_ - 1])) {
                return binarySearch(value, activeStart_.load(), capacity_ - 1);
            }
            else {
                return binarySearch(value, 0, freeStart_.load());
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
        // increment freeStart_ index first

        // this changes
        if (isFull() && freeStart_ != -1) {
            std::cout << "Queue is full, can't insert element." << std::endl;
            return;
        }

        // checking for rollback
        if (isEmpty()) {
            // make unprocessed and active to 0
            freeStart_.store(0);
            activeStart_.store(0);
            queue_[freeStart_.load()] = value;
        }
        else {
            int insertPos = findInsertPosition(value);
            shiftElements(insertPos, freeStart_.load());
            queue_[insertPos] = value;
        }
        freeStart_.store(nextIndex(freeStart_.load()));
    }

    // dequeue returns start of unprocessed. and increment it
    T dequeue() {
        if (isEmpty()) {
            std::cout << "Queue is empty, can't remove element." << std::endl;
            return T();
        }
        T value = queue_[unprocessedStart_.load()];
        unprocessedStart_ = nextIndex(unprocessedStart_.load());
        return value;
    }

    // get Indexes
    int getactiveStart_Index() {
        return activeStart_.load();
    }

    int getUnprocessedStart_Index() {
        return unprocessedStart_.load();
    }

    int getfossileStart_Index() {
        return freeStart_.load();
    }

    // set Indexes
    void setactiveStart_Index(int index) {
        activeStart_.store(index);
    }

    void setUnprocessedStart_Index(int index) {
        unprocessedStart_.store(index);
    }

    void setfossileStart_Index(int index) {
        freeStart_.store(index);
    }

    // increament freeStart_ index
    int increamentfossileStart_Index() {
        if (nextIndex(freeStart_.load()) == activeStart_.load()) {
            std::cout << "Queue is full, can't insert element." << std::endl;
            return -1;
        }
        freeStart_.store(nextIndex(freeStart_.load()));
        return freeStart_.load();
    }

    // retrive index of timestamp for fossile collection
    //should be between active and unprocessed.
    //this increaments the activeStart_ index to this index
    void fossilizeData(uint64_t index) {
        // check if index lies between active and unprocessed
        if (index < activeStart_.load() || index > unprocessedStart_.load()) {
            std::cout << "Index out of range" << std::endl;
            return;
        }
        else {
            activeStart_.store(index);
        }
    }
};
