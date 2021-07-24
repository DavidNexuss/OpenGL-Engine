#pragma once
#include <string>

class Resource;
struct Inotify
{
    ~Inotify();
    std::string path;
    int watcherID;
    Resource* resource;
};
