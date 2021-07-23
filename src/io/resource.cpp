#include <io/resource.h>

ResourceHeader ResourceHeader::fromFile(const std::string& filepath)
{
    ResourceHeader header;
    header.path = filepath;
    header.type = FILE_RESOURCE;
    return header;
}

ResourceHeader& ResourceHeader::makeAsync()
{
    asyncLoading = true;
    return *this;
}

void Resource::load() {
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
