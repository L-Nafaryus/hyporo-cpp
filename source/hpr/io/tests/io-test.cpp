#include <gtest/gtest.h>

#include <hpr/io/logger.hpp>
#include <thread>


void task1()
{
    using namespace hpr;
    logging::error() << "Not error: thread " << std::this_thread::get_id() << logging::flush();
    //std::cout << (logging::error() << "Not error: thread " << std::this_thread::get_id()).str() << std::endl;// << logging::flush();
    //std::cout << "Not error: thread " << std::this_thread::get_id() << std::endl;
}

TEST(io, Logger)
{
    using namespace hpr;
    logging::error() << "Not error: main thread" << logging::flush();
    std::cout << "Main thread " << std::this_thread::get_id() << std::endl;
    /*std::thread t1 {task1};
    std::thread t2 {task1};
    t1.join();
    t2.join();*/
}