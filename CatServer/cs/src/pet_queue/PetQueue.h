#pragma once
#include <deque>
#include <string>

class PetQueue {
public:
    PetQueue() = default;

    // true — в очереди появились новые ID; false — пришёл фрагмент, ждём продолжения
    bool push(const std::string & msg);

    std::string & front() { return data_.front(); }
    void pop()            { data_.pop_front(); }
    bool empty() const    { return data_.empty(); }

private:
    std::deque<std::string> data_;
    std::string             buf_;   // накапливаем куски до ~
};
