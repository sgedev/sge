//
//
#include <thread>

#include <sge/system/local.hpp>

SGE_SYSTEM_BEGIN

Local::Local()
    : window_(sf::VideoMode({ 800, 600 }), "sge")
    , elapsed_min_(std::chrono::nanoseconds(16666666)) {
}

Local::~Local() {
}

void Local::exit() {
    running_ = false;
}

void Local::run() {
    if (!window_.setActive(true)) {
        throw std::runtime_error("Failed to active window.");
    }

    running_ = true;
    frame_last_ = Clock::now();

    while (running_) {
        pollEvents();
        auto curr = Clock::now();
        auto elapsed = curr - frame_last_;
        if (elapsed >= elapsed_min_) {
            frame(elapsed);
            frame_last_ = curr;
        } else {
            std::this_thread::sleep_for(elapsed_min_ - elapsed);
        }
    }
}

void Local::frame(Clock::duration elapsed) {
    Base::frame(elapsed);

    window_.display();
}

void Local::handleEvent(const sf::Event& evt) {

}

void Local::pollEvents() {
    std::optional<sf::Event> evt;
    while (evt = window_.pollEvent()) {
        if (evt->is<sf::Event::Closed>()) {
            window_.close();
            running_ = false;
        } else {
            handleEvent(*evt);
        }
    }
}

SGE_SYSTEM_END
