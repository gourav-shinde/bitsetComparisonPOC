#include<iostream>
#include<boost/random.hpp>
#include"EventClass.hpp"
#include<random>


class RandomEventGenerator{
        //generate random long long int and it is completely random
        static std::uint64_t generateRandomLongLongInt(){
            boost::random::mt19937 gen(std::random_device{}());
            boost::random::uniform_int_distribution<std::uint64_t> dist;
            return dist(gen);
        }
        
        // generate random string
        static std::string generateRandomString(){
            boost::random::mt19937 gen;
            boost::random::uniform_int_distribution<> dist(0, 25);
            std::string str;
            for(int i = 0; i < 10; ++i){
                str += 'a' + dist(gen);
            }
            return str;
        }
        
    public:

        // generate random uint8_t
        static std::uint8_t generateRandomUint8(){
            boost::random::mt19937 gen(std::random_device{}());
            boost::random::uniform_int_distribution<std::uint8_t> dist;
            return dist(gen);
        }

        // generate random uint16_t
        static std::uint8_t generateRandomUint16(){
            boost::random::mt19937 gen(std::random_device{}());
            boost::random::uniform_int_distribution<std::uint16_t> dist;
            return dist(gen);
        }

        Event getEvent(){
            Event e(generateRandomUint16(), generateRandomUint16(), generateRandomString(), generateRandomString(), generateRandomUint8(), generateRandomUint8()%2);
            return e;
        }
};