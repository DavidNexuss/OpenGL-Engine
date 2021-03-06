#pragma once
#include <vector>
#include <algorithm>


template<typename T>
class storage {

    protected:
    std::vector<T> data;
    std::vector<bool> removeMarched;
    std::vector<size_t> toRemove;
    bool dirty = false;

    private:
    void fix() {
        removeMarched.resize(size());
    }

    public:
    using type = T;

    size_t add(const T& value) {
        dirty = true;
        fix();
        if(toRemove.empty()) {
            data.push_back(value);
            removeMarched.resize(size());
            return data.size() - 1;
        }
        else{
            size_t i = toRemove[toRemove.size() - 1];
            toRemove.pop_back();
            removeMarched[i] = false;
            data[i] = value;
            return i;
        }
    }

    size_t size() const { return data.size(); }

    void remove(size_t index) {
        fix();
        dirty = true;
        toRemove.push_back(index);
        removeMarched[index] = 1;
    }

    template <typename ... Args>
    size_t emplace(Args&& ... args) {return add(T(std::forward<Args>(args)...)); }

    int next(int index) {
        fix();
        int i;
        for(i = index; i < std::vector<T>::size() && removeMarched[i]; i++) { }
        return i;
    }

    template <typename F>
    void iterate(F&& func) {
        fix();
        for(size_t i = 0; i < data.size(); i++) {
            while(removeMarched[i]) i++;
            func(at(i));
        }
    }
    bool valid(int index) const {
        return !removeMarched[index];
    }

    bool isDirty() const { return dirty; }
    void cleanDirty() { dirty = false; }

    const T& at(int idx) const { return data[idx]; }
    const T& operator[](int idx) const { return at(idx); }

    T& at(int idx) { return data[idx]; }
    T& operator[](int idx) { return at(idx); }
};

template <typename T>
struct sorted_storage : public storage<T>
{

    struct sorted_view {
        sorted_storage<T>& storage;

        T& at(int idx) { return storage.data[storage.sortedIndexs[idx]]; }
        T& operator[](int idx) { return at(idx); }

        size_t size() const {
            return storage.upperBound;
        }
    };

    std::vector<size_t> sortedIndexs;
    size_t upperBound;

    void sort() {
        sortedIndexs.resize(storage<T>::size());

        for(size_t i = 0; i < storage<T>::data.size(); i++) {
            sortedIndexs[i] = i;
        }

        std::sort(sortedIndexs.begin(),sortedIndexs.end(),
        [&](size_t lhs ,size_t rhs){ 
            bool validLhs = storage<T>::valid(lhs);
            bool validRhs = storage<T>::valid(rhs);

            bool diff = validLhs != validRhs;
            return diff ? validLhs > validRhs : storage<T>::data[lhs] < storage<T>::data[rhs]; 
        });

        size_t i;
        for(i = 0; i < storage<T>::size() && storage<T>::valid(sortedIndexs[i]); i++) { }
        upperBound = i;
    }

    sorted_view getSortedView() {
        if(storage<T>::isDirty()) { 
            sort();
            storage<T>::cleanDirty();
        }
        return sorted_view{*this};
    }
};

template <typename Container, Container& cont>
struct storage_pointer
{
    using type = typename Container::type;
    size_t index = -1;

    storage_pointer() : index(validId()) { }
    storage_pointer(size_t _index) { index = _index; }

    type* operator->() const { return &cont[index]; }

    operator size_t () const { return index; }
    bool valid() const { return index != validId(); }

    static size_t validId() { return -1; }

    bool operator <(size_t other) const { return index < other; }
    bool operator <=(size_t other) const { return index <= other; }
    bool operator ==(size_t other) const { return index == other; }

    static storage_pointer invalidInstance() { return storage_pointer(-1); }
};

#define STORAGE_POINTER_TYPE(x) storage_pointer<decltype(x),x>
