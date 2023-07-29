#include<iostream>
#include<bitset>
#include<string>
#include"randomGenerator.cpp"
#include"currentComparison.cpp"

// 1. bitset is a fixed size sequence of N bits
struct Event{
    std::bitset<64> mSendTime;
    std::bitset<64> mReceiveTime;
    std::bitset<64> mSendName;
    std::bitset<64> mReceiveName;
    std::bitset<8> mGeneration;

    Event(std::uint64_t sendTime, std::uint64_t receiveTime, std::string sendName, std::string receiveName, std::uint8_t generation) :
        mSendTime(sendTime),
        mReceiveTime(receiveTime),
        mSendName(std::hash<std::string>{}(sendName)),
        mReceiveName(std::hash<std::string>{}(receiveName)),
        mGeneration(generation) {}
};

//overload <= operators as they are not supported by bitsets
template<std::size_t N>
bool operator<=(const std::bitset<N>& lhs, const std::bitset<N>& rhs){
    return lhs.to_ullong() <= rhs.to_ullong();
}
template<std::size_t N>
bool operator>=(const std::bitset<N>& lhs, const std::bitset<N>& rhs){
    return lhs.to_ullong() >= rhs.to_ullong();
}

template<std::size_t N>
bool operator<(const std::bitset<N>& lhs, const std::bitset<N>& rhs){
    return lhs.to_ullong() < rhs.to_ullong();
}

// compare two events
bool compareEvents(const Event& lhs, const Event& rhs){
    if(lhs.mSendTime < rhs.mSendTime){
        return true;
    }
    else if(lhs.mSendTime != rhs.mSendTime){
        if(lhs.mReceiveTime < rhs.mReceiveTime){
            return true;
        }
        else if(lhs.mReceiveTime != rhs.mReceiveTime){
            if(lhs.mSendName < rhs.mSendName){
                return true;
            }
            else if(lhs.mSendName != rhs.mSendName){
                if(lhs.mReceiveName < rhs.mReceiveName){
                    return true;
                }
                else if(lhs.mReceiveName != rhs.mReceiveName){
                    if(lhs.mGeneration < rhs.mGeneration){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


std::int32_t main(){
    RandomGenerator randomGenerator;
    std::uint64_t sendTime = randomGenerator.generateRandomLongLongInt();
    std::uint64_t receiveTime = randomGenerator.generateRandomLongLongInt();
    std::string sendName = randomGenerator.generateRandomString();
    std::string receiveName = randomGenerator.generateRandomString();
    std::uint8_t generation = randomGenerator.generateRandomUint8();
    Event event1(sendTime, receiveTime, sendName, receiveName, generation);
    Event2 eventA(sendTime, receiveTime, sendName, receiveName, generation);
    sendTime = randomGenerator.generateRandomLongLongInt();
    receiveTime = randomGenerator.generateRandomLongLongInt();
    sendName = randomGenerator.generateRandomString();
    receiveName = randomGenerator.generateRandomString();
    generation = randomGenerator.generateRandomUint8();

    Event event2(sendTime, receiveTime, sendName, receiveName, generation);
    Event2 eventB(sendTime, receiveTime, sendName, receiveName, generation);

    std::cout<<compareEvents(event1, event2)<<std::endl;
    std::cout<<compareEvents2(eventA, eventB)<<std::endl;
}