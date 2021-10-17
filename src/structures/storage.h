#pragma once
#include <vector>
#include <algorithm>

/**
 * Similar to vector but with O(1) erase using an index map.
 * General purposes storage container designed for main components of the engine, models, textures, materials, material instances ...
 */
template <typename T>
struct storage
{
    std::vector<T> data;
    std::vector<size_t> indices;
    bool dirty = false;

    size_t push_back(T&& object)
    {
        data.push_back(std::move(object));
        indices.push_back(indices.size());
        dirty = true;
        return indices.size() - 1;
    }

    void erase(size_t idx) {
        
        std::swap(data[idx],data[data.size() -1]);
        data.pop_back();
        indices.pop_back();
        dirty = true;
    }

    T& get(size_t idx) const {
        return data[indices[idx]];
    }

    const T& operator[] (size_t idx) const { return data[indices[idx]]; }
    T& operator[] (size_t idx) { return data[indices[idx]]; }

    std::vector<T>& internal() {
        return data;
    }
};

/**
 * Same as storage but providing a function to sort data and its indexes so data can be iterated sorted continuously and leave the
 * index accessors unaffected.
 * Designed for reordering models if needed before rendering
 */

template <typename T>
struct sorted_storage : public storage<T>
{

    using storage<T>::data;
    using storage<T>::indices;
    using storage<T>::dirty;

    void sort()
    {
        struct comparable { T first; size_t second; };
        std::vector<comparable> sortedData(data.size());

        for(size_t i = 0; i < data.size(); i++) {
            sortedData[i] = {data[i], indices[i]};
        }

        std::sort(sortedData.begin(),sortedData.end(),[&](const comparable& lobj,const comparable& robj){ return (lobj.first < robj.first); });

        for(size_t i = 0; i < indices.size(); i++) { 
            indices[sortedData[i].second] = i;
            data[i] = sortedData[i].first;
        }

    }

    std::vector<T>& sorted_internal() {
        if(dirty) {
            sort();
            dirty = false;
        }
        return data;
    }
};

/**
 * Same as storage but returns a sorted index vector for order vector traversal
 */
/*
template <typename T>
struct sorted_storage : public storage<T>
{
    using storage<T>::data;
    using storage<T>::indices;
    using storage<T>::dirty;

    std::vector<size_t> sortedIndices;
    
    struct SortProxy
    {
        std::vector<T>& vectorData;
        std::vector<size_t>& indices;

        SortProxy(std::vector<T>& _vectorData,std::vector<size_t>& _indices) : vectorData(_vectorData), indices(_indices) { }

        T& operator[] (size_t idx) { return vectorData[idx]; }
    };

    void sort()
    {
        size_t n = 0;
        std::generate(sortedIndices.begin(),sortedIndices.end(),[n = 0]() mutable {return n++; });
        std::sort(sortedData.begin(),sortedData.end(),[&](const size_t i,const size_t j){return data[i] < data[j]; });
    }

    SortProxy sorted_internal() {
        if(dirty) sort();
        dirty = false;
        return {data,sortedIndices};
    }


};*/
