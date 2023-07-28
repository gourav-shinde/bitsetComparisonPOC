#include <iostream>
#include <set>
#include "randomGenerator.cpp"
#include "Event.hpp"


std::int32_t main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "Pass number of iterations to run" << std::endl;
        exit(0);
    }
    auto numIterations = atoi(argv[1]);
    RandomGenerator randomGenerator;
    std::uint64_t sendTime = 0, receiveTime = 0;
    std::uint8_t sendName = 0, receiveName = 0, generation = 0;
    bool eventType = false;

    std::multiset<struct Event, compareEvents>  older;
    std::multiset<struct Event, compareEvents2> newer;

    while (numIterations--) {
        sendTime    = randomGenerator.generateRandomLongLongInt();
        receiveTime = randomGenerator.generateRandomLongLongInt();
        sendName    = randomGenerator.generateRandomUint8();
        receiveName = randomGenerator.generateRandomUint8();
        generation  = randomGenerator.generateRandomUint8();
        eventType   = randomGenerator.generateRandomUint8() % 2;

        older.emplace(sendTime, receiveTime, sendName, receiveName, generation, eventType);
        newer.emplace(sendTime, receiveTime, sendName, receiveName, generation, eventType);
    }
}