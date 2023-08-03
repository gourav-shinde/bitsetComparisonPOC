#include <bitset>
#include <type_traits>
#include <limits>
#include <string>

template<std::size_t N1,std::size_t N2>
std::bitset<N1+N2> operator +( 
                            const std::bitset<N1>& a,
                            const std::bitset<N2>& b
                            )
{
    std::bitset<N1+N2> res;
    for(size_t i=0;i<N1;++i)
        res.set(i, a[i]);

    for(size_t i=N1;i<N2;++i)
        res.set(i, b[i]);

    return res;
}

template<std::size_t N>
bool operator<(const std::bitset<N>& x, const std::bitset<N>& y) {
    size_t a{0},b{0};
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
        mask_ = std::bitset<64>(receiveTime_) +
                std::bitset<64>(sendTime_) +
                std::bitset<8>(receiveName_) +
                std::bitset<8>(sendName_) +
                std::bitset<8>(generation_) +
                std::bitset<1>(eventType_);
    }
};



struct compareEvents {
public:
    bool operator() (const Event& lhs, const Event& rhs) const {
        return lhs.mask_ < rhs.mask_;
    }
};