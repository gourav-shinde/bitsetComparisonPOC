
#include<iostream>
#include"randomGenerator.cpp"
#include"currentComparison.cpp"
#include"bitsetPOC.cpp"


std::int32_t main(int argc, char** argv){
    int numIterations = atoi(argv[1]);

    RandomGenerator randomGenerator;
    std::uint64_t sendTime ,receiveTime;
    std::string sendName, receiveName;
    std::uint8_t generation;
    while(numIterations--){
    sendTime = randomGenerator.generateRandomLongLongInt();
    receiveTime = randomGenerator.generateRandomLongLongInt();
    sendName = randomGenerator.generateRandomString();
    receiveName = randomGenerator.generateRandomString();
    generation = randomGenerator.generateRandomUint8();

    Event event1(sendTime, receiveTime, sendName, receiveName, generation);
    Event2 eventA(sendTime, receiveTime, sendName, receiveName, generation);

    sendTime = randomGenerator.generateRandomLongLongInt();
    receiveTime = randomGenerator.generateRandomLongLongInt();
    sendName = randomGenerator.generateRandomString();
    receiveName = randomGenerator.generateRandomString();
    generation = randomGenerator.generateRandomUint8();

    Event event2(sendTime, receiveTime, sendName, receiveName, generation);
    Event2 eventB(sendTime, receiveTime, sendName, receiveName, generation);
    

    if(compareEvents(event1, event2) != compareEvents2(eventA, eventB)){
        std::cout<<"False"<<std::endl;
        break;
    }
    

    }

}