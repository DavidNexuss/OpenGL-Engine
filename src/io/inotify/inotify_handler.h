#pragma once
#include <thread>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <memory>
#include <io/inotify/inotify_context.h>
#include <io/inotify/inotify.h>

namespace InotifyHandler
{
    extern std::mutex modifyLocks_mtx;
    extern std::thread* inotify_thread;
    extern std::map<int,Inotify*> watchers;

    extern InotifyContext* context;

    std::thread* createInotifyThread();
    std::shared_ptr<Inotify> addInotify(const std::string& filepath);
    void removeInotify(const Inotify& inotify);
    void acquire();
};