#include <iostream>
#include <string>

namespace Breadboard8
{
    class Clock
    {
    public:
        unsigned int frequency;
        Clock(unsigned int fq)
        {
            this->frequency = fq;
        }
    };

}