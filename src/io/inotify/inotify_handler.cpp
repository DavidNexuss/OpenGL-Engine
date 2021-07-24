#include "inotify.h"
#include "inotify_handler.h"
#include <io/resource.h>
#include <list>

Inotify::~Inotify() {
    InotifyHandler::removeInotify(*this);
}
namespace InotifyHandler
{

    std::shared_ptr<Inotify> addInotify(const std::string& path)
    {
        acquire();
        std::lock_guard<std::mutex> lock(modifyLocks_mtx);
        
        int wd = context->addWatch(path.c_str());
        Inotify* watcher = new Inotify();
        watcher->watcherID = wd;
        watcher->path = path;
        watchers[wd] = watcher;
        return std::shared_ptr<Inotify>(watcher);
    }

    void removeInotify(const Inotify& watcher) 
    {
        std::lock_guard<std::mutex> lock(modifyLocks_mtx);
        watchers.erase(watcher.watcherID);
        context->removeWatch(watcher.watcherID);
    }

    void acquire()
    {
        if(context == nullptr) {
            context = new InotifyContext();
            inotify_thread = createInotifyThread();
        }
    }

    std::thread* createInotifyThread()
    {
        return new std::thread([&]() {
            while(context->events()) {
                std::list<Inotify*> reload;
                {
                    std::lock_guard<std::mutex> lock(modifyLocks_mtx);
                    context->check(watchers,reload);
                }

                for(const auto& watcher : reload) {
                    watcher->resource->use();
                }
            }
        });
    }

    std::mutex modifyLocks_mtx;
    std::thread* inotify_thread;
    std::map<int,Inotify*> watchers;
    InotifyContext* context;

} // namespace InotifyHandler
