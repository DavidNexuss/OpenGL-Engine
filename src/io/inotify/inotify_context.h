#pragma once
#include <map>
#include <list>
#include <thread>
#include "inotify.h"
#include <debug.h>

struct InotifyContextBase
{
    virtual int addWatch(const char* path);
    virtual void removeWatch(int watchID);
    virtual void check(std::map<int,Inotify*>& watchers,std::list<Inotify*>& reload) = 0;
    virtual int events() = 0;
};

#ifdef __linux__
struct InotifyContext : public InotifyContextBase
{
    int inotifyFd;
    size_t numEvents;
    char* eventBuffer;

    InotifyContext();
    virtual int addWatch(const char* path) override;
    virtual void removeWatch(int watchID) override;
    virtual void check(std::map<int,Inotify*>& watchers,std::list<Inotify*>& reload) override;
    virtual int events() override;
};

#endif // __linux__