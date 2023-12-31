#include <type_traits>
#include <limits>
#include <string>


struct Event3 {
    std::uint64_t    receiveTime_;
    std::uint64_t    sendTime_;
    std::uint8_t     sendName_;
    std::uint8_t     receiveName_;
    std::uint8_t     generation_;
    bool             Event2Type_;


    Event3(std::uint64_t sendTime,
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
/* Compares two events to see if one has a receive time less than to the other */
struct compareEvent3 {
public:
    bool operator() (const Event3& first,
                     const Event3& second) const {
        return  (first.receiveTime_ < second.receiveTime_) ? true :
                ((first.receiveTime_ != second.receiveTime_) ? false :
                  ((first.sendTime_ < second.sendTime_) ? true :
                  ((first.sendTime_ != second.sendTime_) ? false :
                    ((first.sendName_ < second.sendName_) ? true :
                    ((first.sendName_ != second.sendName_) ? false :
                      ((first.generation_ < second.generation_) ? true :
                      ((first.generation_ != second.generation_) ? false :
                        ((first.Event2Type_ < second.Event2Type_) ? true :
                        ((first.Event2Type_ != second.Event2Type_) ? false : false)))))))));
    }

};
