#include <io/inotify/inotify_context.h>
using namespace std;
#ifdef __linux__
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <list>


const static int BUF_LEN = (10 * (sizeof(struct inotify_event) + NAME_MAX + 1));

InotifyContext::InotifyContext()
{
    inotifyFd = inotify_init();
    eventBuffer = new char[BUF_LEN];
}

int InotifyContext::addWatch(const char* path)
{
    InotifyContextBase::addWatch(path);
    return inotify_add_watch(inotifyFd, path,IN_ALL_EVENTS); 
}
void InotifyContext::removeWatch(int watchID)
{
    InotifyContextBase::removeWatch(watchID);
    inotify_rm_watch(inotifyFd, watchID);
}
int InotifyContext::events()
{
    return numEvents = read(inotifyFd,eventBuffer,BUF_LEN);
}
void InotifyContext::check(map<int,Inotify*>& map,list<Inotify*>& reload)
{
    inotify_event* event;
    char* p;

    for (p = eventBuffer; p < eventBuffer + numEvents;)
    {
        event = (struct inotify_event*)p;
        if(event->mask & (IN_MODIFY | IN_DELETE_SELF | IN_CLOSE_WRITE)) {
            reload.push_back(map[event->wd]);
        }

        if(event->mask & (IN_DELETE_SELF))
        {
            Inotify* inotify = map[event->wd];
            map.erase(inotify->watcherID);
            removeWatch(inotify->watcherID);
            inotify->watcherID = addWatch(inotify->path.c_str());
            map[inotify->watcherID] = inotify;
        }
        p+= sizeof(struct inotify_event) + event->len;

    }
    
}
#endif


int InotifyContextBase::addWatch(const char* path)
{
    REGISTER_WATCH_ADDITION();
    return 0;
}
void InotifyContextBase::removeWatch(int watchID)
{
    REGISTER_WATCH_REMOVAL();
}