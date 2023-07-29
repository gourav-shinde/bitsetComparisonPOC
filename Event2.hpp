#include <bitset>
#include <type_traits>
#include <limits>
#include <string>


struct Event2 {
    std::uint64_t    receiveTime_;
    std::uint64_t    sendTime_;
    std::uint8_t     sendName_;
    std::uint8_t     receiveName_;
    std::uint8_t     generation_;
    bool             Event2Type_;


    Event2(std::uint64_t sendTime,
          std::uint64_t receiveTime,
          std::uint8_t  sendName,
          std::uint8_t  receiveName,
          std::uint8_t  generation,
          bool          Event2Type) :
              sendTime_(sendTime),
              receiveTime_(receiveTime),
              sendName_(sendName),
              receiveName_(receiveName),
              generation_(generation),
              Event2Type_(Event2Type) {
    }
};

// compare two Event2s
struct compareEvent2 {
public:
    bool operator() (const Event2& lhs, const Event2& rhs) const {
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

