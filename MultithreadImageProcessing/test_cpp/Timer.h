#pragma once

struct ScopeTimer
{
public:
    ScopeTimer();
    ScopeTimer(char const* title);
    ~ScopeTimer();
    
private:
    struct pImpl;
    pImpl* _pImpl;
};
