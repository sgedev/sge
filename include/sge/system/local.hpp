//
//
#ifndef SGE_SYSTEM_LOCAL_HPP
#define SGE_SYSTEM_LOCAL_HPP

#include <SFML/Graphics.hpp>

#include <sge/system/common.hpp>
#include <sge/system/base.hpp>

SGE_SYSTEM_BEGIN

class Local: public Base {
public:
    Local();
    ~Local() override;

public:
    void exit() override;
    void run() override;

protected:
    void frame(Clock::duration elapsed) override;
    virtual void handleEvent(const sf::Event& evt);

private:
    void pollEvents();

private:
    sf::RenderWindow window_;
    bool running_;
    Clock::time_point frame_last_;
    Clock::duration elapsed_min_;
};

SGE_SYSTEM_END

#endif // SGE_SYSTEM_LOCAL_HPP
