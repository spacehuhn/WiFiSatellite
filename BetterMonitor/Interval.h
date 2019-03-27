#ifndef Interval_h
#define Interval_h

#include <functional> // lambda
#include <esp_timer.h> // esp_timer_get_time()

class Interval {
    public:
        std::function<void()> callback = nullptr;

        Interval(int64_t us);
        ~Interval();

        void set_interval(int64_t us);

        bool update();

    private:
        int64_t last_update_time { 0 };
        int64_t interval_time { 0 };
};

#endif /* ifndef Interval_h */