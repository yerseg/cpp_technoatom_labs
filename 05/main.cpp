#include <iostream>
#include <thread>
#include <atomic>

const std::size_t ITER_NUMBER = 1000000;
std::atomic_size_t counter(0);

void PingPong(const size_t flag)
{
    while (counter < ITER_NUMBER)
    {
        if (flag == 0 && counter % 2 == 0)
        {
            std::cout << "ping" << std::endl;
            counter++;
        }
        else if (flag == 1 && counter % 2 == 1)
        {
            std::cout << "pong" << std::endl;
            counter++;
        }

    }
}

int main()
{
    std::thread ping_th(PingPong, 0);
    std::thread pong_th(PingPong, 1);
    ping_th.join();
    pong_th.join();
    return 0;
}