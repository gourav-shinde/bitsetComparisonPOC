#include"UnifiedQueue.hpp"

//no checks for valid indexes is made here as it should be done by the caller
uint32_t UnifiedQueue::binarySearch(T element, uint32_t low, uint32_t high){
    uint32_t mid;

    // This will never trigger, as this condition is checked in the parent function
    if (isEmpty())
        return getFreeStart();

    while (low < high) {
        mid = ceil((low + high) / 2);

        if (this->compare_(queue_[mid], element)) {
            low = (mid + 1) % capacity();
        }
        else {
            high = (mid) % capacity(); // very good chance for infinite loop
        }
    }

    return (low) % capacity();
}

//no checks for valid indexes is made here as it should be done by the caller
//low: activeStart_
//high: freeStart_
uint32_t UnifiedQueue::findInsertPosition(T element, uint32_t low, uint32_t high){
    
    //This will never trigger, as this condition is checked in the parent function
    if (isEmpty())
        return freeStart_.load(std::memory_order_relaxed);

    // when there is no rotation in queue
    if (low < high) {
        return binarySearch(element, low, high);
    }
    // rotation i.e fossileStart_ < activeStart_
    else {
        if (compare_(element, queue_[capacity() - 1])) {
            return binarySearch(element, low, capacity() - 1);
        }
        else {
            return binarySearch(element, 0, high);
        }
    }
}


//shift elements from start to end by 1 position to the right
//no checks for valid indexes is made here as it should be done by the caller
//Discuss this as it will have ABA problem across threads
void UnifiedQueue::shiftElements(uint32_t start, uint32_t end) {
    uint32_t i = end;
    while (i != start) {
        queue_[i] = queue_[prevIndex(i)];
        i = prevIndex(i);
    }
}


bool UnifiedQueue::enqueue(T element){
    //checks first
    if (isFull()){
        //throw message
        std::cout << "Queue is full" << std::endl;
        return false;
    }
    uint32_t freeStart_ = getFreeStart();
    if(isEmpty()){
        setFreeStart(nextIndex(getFreeStart()));
        queue_[freeStart_] = element;
        return true;
    }
    //get valid freeStart
    
    //increament freeStart
    if(nextIndex(freeStart_) == getActiveStart()){
        //set freeSign_ to 1
        setFreeSign(1);
    }
    setFreeStart(nextIndex(freeStart_));
    

    if(isEmpty()){
        //set activeStart_ to freeStart_
        setActiveStart(freeStart_.load(std::memory_order_relaxed));
    }
}


//this is done
T UnifiedQueue::dequeue(){
    //checks first
    if (isEmpty()){
        //throw message
        std::cout << "Queue is empty" << std::endl;
        return T();
    }
    if(unprocessedSign()){
        //throw message
        std::cout << "unprocessed Queue is empty" << std::endl;
        return T();
    }
    T element = queue_[getUnprocessedStart()];
    
    if(nextIndex(getUnprocessedStart()) == getFreeStart()){
        //set unprocessedSign_ to 1
        setUnprocessedSign(1);
        
    }
    setUnprocessedStart(nextIndex(getUnprocessedStart()));

    return element;
    
}

//this is done 
bool UnifiedQueue::increamentActiveStart(){
    //checks first
    if (isEmpty()){
        //throw message
        std::cout << "Queue is empty" << std::endl;
        return false;
    }
    
    if(getActiveStart() == getFreeStart()){
        std::cout << "Active Zone is Empty" << std::endl;
        return false;
    }
    setActiveStart(nextIndex(getActiveStart()));
    return true;
}