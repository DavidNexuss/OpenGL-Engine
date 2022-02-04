#pragma once
#include <vector>
#include <algorithm>
#include <iostream>

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
            std::cerr << "Reuse" << std::endl;
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
    size_t emplace(Args&& ... args) {return insert(T(std::forward<Args>(args)...)); }

    int next(int index) {
        fix();
        int i;
        for(i = index; i < std::vector<T>::size() && removeMarched[i]; i++) { }
        return i;
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

/**
 * Same as storage but providing a function to sort data and its indexes so data can be iterated sorted continuously and leave the
 * index accessors unaffected.
 * Designed for reordering models if needed before rendering
 */

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