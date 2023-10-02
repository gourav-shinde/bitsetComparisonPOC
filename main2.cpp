#include <iostream>
#include <set>
#include "randomGenerator.cpp"
// #include "Event.hpp"
// #include "Event2.hpp"
#include "Event3.hpp"
#include "chrono"
#pragma optimize("", off)

void do_something(int numIterations,const Event3 &A,const Event3 &B){
    std::chrono::time_point<std::chrono::system_clock> start, end;
    compareEvent3 normal;
    compareEventrelax relax;
    compareEventshort shortvar;
    long tempp=numIterations;
    start = std::chrono::system_clock::now();
    while (--tempp) {
        normal(A,B);
    }
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout<< "elapsed time normal compare events: " << elapsed_seconds.count() << "s\n";
    tempp=numIterations;
    start = std::chrono::system_clock::now();
    while (--tempp) {
        relax(A,B);
    }
    end = std::chrono::system_clock::now();

    elapsed_seconds = end - start;
    end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout<< "elapsed time relax compare events: " << elapsed_seconds.count() << "s\n";

    tempp=numIterations;
    start = std::chrono::system_clock::now();
    while (--tempp) {
        shortvar(A,B);
    }
    end = std::chrono::system_clock::now();

    elapsed_seconds = end - start;
    end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout<< "elapsed time short(wrong compare logic) compare events: " << elapsed_seconds.count() << "s\n";
}

std::int32_t main(int argc, char** argv) {


    
    if (argc != 2) {
        std::cout << "Pass number of iterations to run" << std::endl;
        exit(0);
    }
    auto numIterations = atoi(argv[1]);
    Event3 A(223372036854775808,223372036854775500,"Chicago","Cincinnati",1,true);
    Event3 B(22337203685477580,2233720368547600,"Chicago","Columbus",1,true);
    Event3 C(223372036854775808,223372036854775500,"Chicago","Cincinnati",1,false);

    // A>B 
    std::cout<<"A>B\n";
    do_something(numIterations,A,B);

    // A==B
    std::cout<<"A==B\n";
    do_something(numIterations,A,C);
}