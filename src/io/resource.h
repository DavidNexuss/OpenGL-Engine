#pragma once
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <future>
#include <map>
#include <io/io.h>
#include <io/inotify/inotify.h>

using ReloadRoutine = std::function<void(const std::vector<uint8_t>&)>;
enum ResourceType {
    FILE_RESOURCE = 0
};

struct ResourceHeader
{
    ResourceType type;
    std::string path;
    bool asyncLoading = true;
    bool fileWatch = false;

    static ResourceHeader fromFile(const std::string& filepath);
    ResourceHeader& makeSync();
    ResourceHeader& makeWatch();
};

struct ResourceBuffer
{
    int size;
    std::vector<uint8_t> buffer;

    bool valid() const {return size >= 0; }
};

class Resource
{
    ResourceHeader header;
    ReloadRoutine reload;

    std::future<std::shared_ptr<ResourceBuffer>> data;
    std::shared_ptr<ResourceBuffer> readyData;
    std::shared_ptr<Inotify> inotify;

    void acquire();
    void createWatch();

    public:
    
    Resource(const ResourceHeader& _header);
    Resource(const ResourceHeader& _header,ReloadRoutine _routine);

    const std::vector<uint8_t>& asBuffer();
    const char* asString();
    
    bool valid();
    void onReload();
    void use();
    void load();
};

using ResourcePtr = std::shared_ptr<Resource>;
namespace ResourceManager
{
    extern std::map<std::string,ResourcePtr> loadedResources;
    
    ResourcePtr loadFromFile(const std::string& path);
}