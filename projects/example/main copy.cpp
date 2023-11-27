#include "args.hpp"
#include <iostream>
#include <uvw.hpp>

Args Args::args;

int main(int argc, char **argv) {
    Args::instance().init(argc, argv);

    auto loop = uvw::loop::get_default();
    // uv_timer
    auto timer = loop->resource<uvw::timer_handle>();
    timer->data(std::make_shared<int>(0));
    timer->on<uvw::timer_event>([timer](uvw::timer_event &, uvw::timer_handle &) {
        auto tick = timer->data<int>();
        std::cout << "timer: " << (*tick)++ << std::endl;
        if (*tick == 5) timer->close();
    });
    timer->start(uvw::timer_handle::time{1000}, uvw::timer_handle::time{1000});

    return loop->run();
}
