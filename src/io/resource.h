#pragma once
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <future>
#include <io/io.h>

using ReloadRoutine = std::function<void(const std::vector<uint8_t>&)>;

enum ResourceType {
    FILE_RESOURCE = 0
};

struct ResourceHeader
{
    ResourceType type;
    bool asyncLoading;
    std::string path;

    static ResourceHeader fromFile(const std::string& filepath);
    ResourceHeader& makeAsync();
};

struct ResourceBuffer
{
    bool isDirty;
    bool isNew;
    
    int size;
    std::vector<uint8_t> buffer;

    bool valid() const {return size >= 0; }

};

class Resource
{
    ResourceHeader header;
    ReloadRoutine reload;

    bool isDirty = false;   //In memory but not saved
    bool isNew = false;     //New but not notified
    
    std::future<std::shared_ptr<ResourceBuffer>> data;
    std::shared_ptr<ResourceBuffer> readyData;

    void load();
    void acquire();

    public:
    Resource(const ResourceHeader& _header);
    Resource(const ResourceHeader& _header,ReloadRoutine _routine);

    const std::vector<uint8_t>& asBuffer();
    const char* asString();
    bool valid();
};