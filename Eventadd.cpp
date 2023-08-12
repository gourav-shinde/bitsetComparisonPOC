#include <bitset>
#include <type_traits>
#include <limits>
#include <string>
#include<iostream>

template<std::int64_t N1,std::int64_t N2,std::int64_t N3,std::int64_t N4,std::int64_t N5,std::int64_t N6>
std::bitset<N1+N2+N3+N4+N5> add( 
                            const std::bitset<N1> a,
                            const std::bitset<N2> b,
                            const std::bitset<N3> c,
                            const std::bitset<N4> d,
                            const std::bitset<N5>e,
                            const std::bitset<N6>f
                            )
{
    std::bitset<N1+N2+N3+N4+N5> res;
    // start with rhs
    int64_t i=0;
    for(i=0;N1>i;++i)
        res.set(i, a[i]);
    for(i;N1+N2>i;++i)
        res.set(i, b[i]);
    for(i;N1+N2+N3>i;++i)
        res.set(i, c[i]);
    for(i;N1+N2+N3+N4>i;++i)
        res.set(i, d[i]);
    for(i;N1+N2+N3+N4+N5>i;++i)
        res.set(i, e[i]);
    for(i;N1+N2+N3+N4+N5+N6>i;++i)
        res.set(i, e[i]);
    return res;
}


// template< std::int64_t N1, std::int64_t N2 >
// std::bitset<N1+N2> operator+(const std::bitset<N1>& a, const std::bitset<N2>& b ) {
//     return std::bitset<N1+N2>(a.to_string() + b.to_string());
// }

// template<std::int64_t N1,std::int64_t N2>
// std::bitset<N1+N2> operator +( 
//                             const std::bitset<N1>& a,
//                             const std::bitset<N2>& b
//                             )
// {
//     std::bitset<N1+N2> res;
//     res=res|std::bitset<N1+N2>(a);
//     res=res<<N2;
//     res=res|std::bitset<N1+N2>(b);
//     return res;
// }

template<std::int64_t N>
bool operator<(const std::bitset<N>& x, const std::bitset<N>& y) {
    int64_t a{0},b{0};
    while(a!=N && b!=N){
        if((a=x._Find_next(a))<(b=y._Find_next(b))){
            return false;
        }
    }
    return true;
}

struct Event {
    std::uint64_t    receiveTime_;
    std::uint64_t    sendTime_;
    std::uint8_t     sendName_;
    std::uint8_t     receiveName_;
    std::uint8_t     generation_;
    bool             eventType_;
    std::bitset<153> mask_;

    Event(std::uint64_t sendTime,
          std::uint64_t receiveTime,
          std::uint8_t  sendName,
          std::uint8_t  receiveName,
          std::uint8_t  generation,
          bool          eventType) :
              sendTime_(sendTime),
              receiveTime_(receiveTime),
              sendName_(sendName),
              receiveName_(receiveName),
              generation_(generation),
              eventType_(eventType) {
        mask_ = add(
                &std::bitset<64U>(receiveTime_) ,
                &std::bitset<64U>(sendTime_) ,
                &std::bitset<8U>(receiveName_) ,
                &std::bitset<8U>(sendName_) ,
                &std::bitset<8U>(generation_) ,
                &std::bitset<1U>(eventType_)
                );
                std::cout<<mask_;
    }
};



struct compareEvents {
public:
    bool operator() (const Event& lhs, const Event& rhs) const {
        return lhs.mask_ < rhs.mask_;
    }
};

int main(){
    Event(5,5,5,5,5,1);

}