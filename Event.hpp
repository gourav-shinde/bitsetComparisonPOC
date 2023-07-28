#include <bitset>
#include <type_traits>
#include <limits>
#include <string>

template< std::size_t N1, std::size_t N2 >
std::bitset<N1+N2> operator+(const std::bitset<N1>& a, const std::bitset<N2>& b ) {
    return std::bitset<N1+N2>(a.to_string() + b.to_string());
}

template<std::size_t N>
bool operator<(const std::bitset<N>& x, const std::bitset<N>& y) {
    for (int i = N-1; i >= 0; i--) {
        if (x[i] ^ y[i]) return y[i];
    }
    return false;
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

// compare two events
struct compareEvents {
public:
    bool operator() (const Event& lhs, const Event& rhs) const {
        if(lhs.receiveTime_ < rhs.receiveTime_) {
            return true;
        } else if (lhs.receiveTime_ == rhs.receiveTime_) {
            if (lhs.sendTime_ < rhs.sendTime_) {
                return true;
            } else if (lhs.sendTime_ == rhs.sendTime_) {
                if (lhs.receiveName_ < rhs.receiveName_) {
                    return true;
                } else if (lhs.receiveName_ == rhs.receiveName_) {
                    if(lhs.sendName_ < rhs.sendName_) {
                        return true;
                    } else if(lhs.sendName_ != rhs.sendName_) {
                        if(lhs.generation_ < rhs.generation_) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
};

struct compareEvents2 {
public:
    bool operator() (const Event& lhs, const Event& rhs) const {
        return lhs.mask_ < rhs.mask_;
    }
};