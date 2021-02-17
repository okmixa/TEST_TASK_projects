#include "Timer.h"

#include <chrono>
#include <iostream>

struct ScopeTimer::pImpl
{
public:
    pImpl()
    {
        _title = nullptr;
        _start = std::chrono::high_resolution_clock::now();
    }
    pImpl(const char* title)
    {
        _title = title;
        _start = std::chrono::high_resolution_clock::now();
    }
    ~pImpl()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - _start);        
        std::cout << (_title ? _title : "") << ": " << duration.count() << " ms\n";
    }

private:
    std::chrono::high_resolution_clock::time_point _start;
    char const* _title;
};

ScopeTimer::ScopeTimer()
{
    _pImpl = new pImpl();
}

ScopeTimer::ScopeTimer(char const* title)
{
    _pImpl = new pImpl(title);
}

ScopeTimer::~ScopeTimer()
{
    delete _pImpl;
}
