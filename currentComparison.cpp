#include<iostream>
#include<string>


struct Event2{
    std::uint64_t mSendTime;
    std::uint64_t mReceiveTime;
    std::string mSendName;
    std::string mReceiveName;
    std::uint8_t mGeneration;

    Event2(std::uint64_t sendTime, std::uint64_t receiveTime, std::string sendName, std::string receiveName, std::uint8_t generation) :
        mSendTime(sendTime),
        mReceiveTime(receiveTime),
        mSendName(sendName),
        mReceiveName(receiveName),
        mGeneration(generation) {}
};

// compare two events
bool compareEvents2(const Event2& lhs, const Event2& rhs){
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