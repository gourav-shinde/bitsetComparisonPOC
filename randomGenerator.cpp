#include<iostream>
#include<boost/random.hpp>


class RandomGenerator{
    public:
        //generate random long long int
        static std::uint64_t generateRandomLongLongInt(){
            boost::random::mt19937 gen;
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
        // generate random uint8_t
        static std::uint8_t generateRandomUint8(){
            boost::random::mt19937 gen;
            boost::random::uniform_int_distribution<std::uint8_t> dist;
            return dist(gen);
        }
};