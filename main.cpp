#include <iostream>
#include <set>
#include "randomGenerator.cpp"
// #include "Event.hpp"
// #include "Event2.hpp"
#include "Event3.hpp"


std::int32_t main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "Pass number of iterations to run" << std::endl;
        exit(0);
    }
    auto numIterations = atoi(argv[1]);
    RandomGenerator randomGenerator;
    std::uint64_t sendTime = 0, receiveTime = 0;
    std::string sendName = "", receiveName = ""; 
    std::uint8_t generation = 0;
    bool eventType = false;

    // std::multiset<struct Event, compareEvents>  newer;
    // std::multiset<struct Event2, compareEvent2> older;
    std::multiset<struct Event3, compareEvent3> tertiary;

    while (numIterations--) {
        sendTime    = 2002;
        receiveTime = 3000;
        sendName    = "sdfasfsa";
        receiveName = "sdfsdfas";
        generation  = randomGenerator.generateRandomUint8();
        eventType   = randomGenerator.generateRandomUint8() % 2;

        // older.emplace(sendTime, receiveTime, sendName, receiveName, generation, eventType);
        // newer.emplace(sendTime, receiveTime, sendName, receiveName, generation, eventType);
        tertiary.emplace(sendTime, receiveTime, sendName, receiveName, generation, eventType);
    }
}