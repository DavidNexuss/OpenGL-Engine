#include <io/resource.h>
#include <io/inotify/inotify_handler.h>

ResourceHeader ResourceHeader::fromFile(const std::string& filepath)
{
    ResourceHeader header;
    header.path = filepath;
    header.type = FILE_RESOURCE;
    return header;
}

ResourceHeader& ResourceHeader::makeSync()
{
    asyncLoading = false;
    return *this;
}

ResourceHeader& ResourceHeader::makeWatch()
{
    fileWatch = true;
    return *this;
}

void Resource::createWatch() {
    inotify = InotifyHandler::addInotify(header.path);
    inotify->resource = this;
}

void Resource::load() {
    if(inotify == nullptr && header.fileWatch) createWatch();

    data = std::async(header.asyncLoading ? std::launch::async : std::launch::deferred ,[&](){
        std::shared_ptr<ResourceBuffer> data = std::make_shared<ResourceBuffer>();
        
        switch(header.type) {
            case FILE_RESOURCE:
                data->size = IO::readFromFile(data->buffer,header.path); break;
            default:
                throw std::runtime_error("undefined type");
        }
        return data;
    });
}

void Resource::acquire() {
    if(data.valid()) {
        readyData = data.get();
    }
}

void Resource::use()
{
    acquire();
    reload(readyData->buffer);
}

Resource::Resource(const ResourceHeader& _header) {
    this->header = _header;
    load();
}

Resource::Resource(const ResourceHeader& _header,ReloadRoutine _routine) {
    this->header = _header;
    reload = _routine;
    load();
}

const std::vector<uint8_t>& Resource::asBuffer() { 
    acquire();
    return readyData->buffer;
}

const char* Resource::asString() {
    return (const char*)asBuffer().data();
}

bool Resource::valid() {
    acquire();
    return readyData->valid();
}

namespace ResourceManager
{
    std::map<std::string,ResourcePtr> loadedResources;
    
    ResourcePtr loadFromFile(const std::string& path) {
        auto it = loadedResources.find(path);
        if(it != loadedResources.end()) return it->second;
        return loadedResources[path] = std::make_shared<Resource>(ResourceHeader::fromFile(path));
    }
}
