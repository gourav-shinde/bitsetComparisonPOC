#include <type_traits>
#include <limits>
#include <compare>
#include <string>


struct Event {
    std::uint64_t    receiveTime_;
    std::string      receiveName_;
    std::string      sendName_;
    std::uint8_t     generation_;
    bool             Event2Type_;
    std::uint64_t    sendTime_;
    
    Event() = default;

    Event(std::uint64_t sendTime,
          std::uint64_t receiveTime,
          std::string  sendName,
          std::string  receiveName,
          std::uint8_t  generation,
          bool          Event2Type) :
              sendTime_(sendTime),
              receiveTime_(receiveTime),
              sendName_(sendName),
              receiveName_(receiveName),
              generation_(generation),
              Event2Type_(Event2Type) {
    }

    bool operator== (const Event &other) {
        return ((this->receiveTime_ == other.receiveTime_)
                && (this->sendTime_ == other.sendTime_)
                && (this->sendName_ == other.sendName_)
                && (this->generation_ == other.generation_)
                && (this->Event2Type_ == other.Event2Type_));
    }
};

// compare two Event2s
/* Compares two events to see if one has a receive time less than to the other */
struct compareEvent {
public:
    bool operator() (const Event& first,
                     const Event& second) const {
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




 