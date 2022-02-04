#pragma once
template<typename T,typename Container,Container container>
struct AccessPointer {
    size_t index;
    inline T* operator ->() const {
        return &container[index];
    }
};
