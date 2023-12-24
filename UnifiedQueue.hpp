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
    //1 bit unprocessedSign, 10 bits activeStart_, 1bit unprocessedSign, 10 bits unprocessedStart_, 1 bit freeSign, 10 bits freeStart_
    std::atomic<uint32_t> marker_; //test with this datatype
public:
    UnifiedQueue(uint16_t capacity=1024){
        if(capacity > 1024){
            throw std::invalid_argument("Capacity should be less than 1024");
        }
        queue_.resize(capacity); 
        //init condition is 0,0,0
        marker_.store(0, std::memory_order_relaxed);
    }

    bool getUnprocessedSign(){
        return (marker_.load(std::memory_order_relaxed) & 0x80000000) >> 31;
    }

    bool getFreeSign(){
        return (marker_.load(std::memory_order_relaxed) & 0x40000000) >> 30;
    }

    //getActiveStart
    uint32_t getActiveStart(){
        return (marker_.load(std::memory_order_relaxed) & 0x3FFC0000) >> 18;
    }

    //getUnprocessedStart
    uint32_t getUnprocessedStart(){
        return (marker_.load(std::memory_order_relaxed) & 0x0003FFC0) >> 6;
    }

    //getFreeStart
    uint32_t getFreeStart(){
        return (marker_.load(std::memory_order_relaxed) & 0x0000003F);
    }

    //setfreeSign
    void setFreeSign(bool sign){
        if(sign){
            marker_ |= 0x40000000;
        }else{
            marker_ &= 0xBFFFFFFF;
        }
    }

    //setunprocessedSign
    void setUnprocessedSign(bool sign){
        if(sign){
            marker_ |= 0x80000000;
        }else{
            marker_ &= 0x7FFFFFFF;
        }
    }

    //setActiveStart
    void setActiveStart(uint32_t start){
        marker_ &= 0xC003FFFF;
        marker_ |= (start << 18);
    }

    //setUnprocessedStart
    void setUnprocessedStart(uint32_t start){
        marker_ &= 0xFFFFC03F;
        marker_ |= (start << 6);
    }

    //setFreeStart
    void setFreeStart(uint32_t start){
        marker_ &= 0xFFFFFFC0;
        marker_ |= start;
    }

    //getCapacity
    uint32_t capacity(){
        return queue_.size();
    }

    //preIndex
    uint32_t prevIndex(uint32_t index){
        return (index + capacity() - 1) % capacity();
    }

    //nextIndex
    uint32_t nextIndex(uint32_t index){
        return (index + 1) % capacity();
    }

    //enqueue needs this 4 functions
    uint32_t binarySearch(T element, uint32_t low, uint32_t high);
    uint32_t findInsertPosition(T element, uint32_t low, uint32_t high);
    void shiftElements(uint32_t start, uint32_t end);
    bool enqueue(T element);

    bool isEmpty(){
        uint32_t marker = marker_.load(std::memory_order_relaxed);
        if(!FreeSign(marker) && ActiveStart(marker) == getFreeStart(marker))
            return true;
        return false;
    }

    bool isFull(){
        return getFreeSign();
    }

    uint16_t size(){
        if(isEmpty()){
            return 0;
        }
        if(isFull()){
            return capacity();
        }
        if(getFreeStart() > getActiveStart()){
            return getFreeStart() - getActiveStart();
        if(getFreeStart() < getActiveStart()){
            return capacity() - getActiveStart() + getFreeStart();
        }
    }
    
    T dequeue();

    bool increamentActiveStart(); //for fossil collection

    void debug(){
        std::cout << "activeStart: " << getActiveStart() << std::endl;
        std::cout << "unprocessedSign: " << getUnprocessedSign() << std::endl;
        std::cout << "unprocessedStart: " << getUnprocessedStart() << std::endl;
        std::cout << "freeSign: " << getFreeSign() << std::endl;
        std::cout << "freeStart: " << getFreeStart() << std::endl;

        int i = getUnprocessedStart();
        if(!getUnprocessedSign()){
        do {
            std::cout << queue_[i].receiveTime_ << " ";
            i = nextIndex(i);
        } while (i != getValidIndex(getFreeStart()));
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

    //getValues from Marker
    
    uint32_t ActiveIndex(uint32_t marker){
        return (marker & 0x3FFC0000) >> 18;
    }

    bool UnProcessedSign(uint32_t marker){
        return (marker & 0x80000000) >> 31;
    }

    bool FreeSign(uint32_t marker){
        return (marker & 0x40000000) >> 30;
    }

    uint32_t UnprocessedStart(uint32_t marker){
        return (marker & 0x0003FFC0) >> 6;
    }

    uint32_t FreeStart(uint32_t marker){
        return (marker & 0x0000003F);
    }
};
