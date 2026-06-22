#include "PetQueue.h"
#include <regex>

bool PetQueue::push(const std::string & msg) {
    static std::regex re_id(R"(@([A-Za-z0-9]+)~)");

    buf_ += msg;

    // Пока в буфере есть завершённый токен @ID~, вытаскиваем их
    bool any = false;
    std::smatch m;
    std::string rest = buf_;
    while (std::regex_search(rest, m, re_id)) {
        data_.push_back(m[1]);
        rest = m.suffix();
        any = true;
    }
    buf_ = rest; // остаток — неполный фрагмент следующего ID

    return any;
}
