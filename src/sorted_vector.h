#include <vector>

template <typename T>
struct sorted_vector
{
    std::vector<T> values;
    std::vector<size_t> indexs;

    sorted_vector() { }

    size_t push_back(T value)
    {
        auto it = lower_bound(values.begin(), values.end(),value);
        size_t a = it - values.begin();
        it = values.insert(it, value);
        
        for (size_t i = a; i < indexs.size(); i++)
        {
            indexs[i]++;
        }
        indexs.push_back(a);
        return indexs.size() - 1;
    }

    const T& operator[] (size_t idx) const { return values[indexs[idx]]; }
    T& operator[] (size_t idx) { return values[indexs[idx]]; }

    inline bool size() { return indexs.size(); } 
    std::vector<T>& native() {return values; }
};
