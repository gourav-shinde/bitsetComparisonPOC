
#include<iostream>
#include"randomGenerator.cpp"
#include"currentComparison.cpp"
#include"bitsetPOC.cpp"


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