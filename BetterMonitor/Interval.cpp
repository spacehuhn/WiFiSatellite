#include "Interval.h"

Interval::Interval(int64_t interval_time) {
    set_interval(interval_time);
}

Interval::~Interval() {}

void Interval::set_interval(int64_t us) {
    assert(us > 0);
    this->interval_time    = us;
    this->last_update_time = esp_timer_get_time();
}

bool Interval::update() {
    int64_t difference = esp_timer_get_time() - last_update_time;

    if (difference >= interval_time) {
        last_update_time = esp_timer_get_time() - (interval_time - difference);
        if (callback) callback();
        return true;
    }
    return false;
}