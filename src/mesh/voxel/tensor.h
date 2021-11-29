#include <array>
#include <cstddef>
#include <vector>

/**
 * static allocated 3D Matrix
 */
template <typename ContainerClass,typename T,size_t N>
struct tensor
{
    const static auto _size = N*N*N;
    ContainerClass container;

    struct tensorProxy
    {
        struct tensorProxy2 {
            ContainerClass& _container;
            size_t x;
            size_t y;

            inline T& operator[](size_t z) {
                return _container[x * N*N + y*N + z];
            }
        };

        ContainerClass& _container;
        size_t x;

        inline tensorProxy2 operator[](size_t idx2) {
            return tensorProxy2{_container,x,idx2};
        }
    };

    public:
    template <typename ... Args>
    tensor(Args&&... args) : container{std::move(args)...} { }

    inline size_t size() const { return _size; }

    inline tensorProxy operator[](size_t idx) {
        return {container,idx};
    }
};

/**
 * Heap allocated array
 */

template <typename T,size_t N>
struct big_array : public std::vector<T> 
{
    big_array() : std::vector<T>(N) { }
};


template <typename T>
struct dynamic_tensor
{
    using ContainerClass =  std::vector<T>;
    std::vector<T> container;
    size_t dimensionSize;

    struct tensorProxy
    {
        struct tensorProxy2 {
            dynamic_tensor& _tensor;
            size_t x;
            size_t y;

            inline T& operator[](size_t z) {
                return _tensor.container[
                    x * _tensor.dimensionSize*_tensor.dimensionSize + y*_tensor.dimensionSize + z];
            }
        };

        dynamic_tensor& _tensor;
        size_t x;

        inline tensorProxy2 operator[](size_t idx2) {
            return tensorProxy2{_tensor,x,idx2};
        }
    };

    public:
    
    dynamic_tensor(size_t _dimensionSize) : 
    container(_dimensionSize*_dimensionSize*_dimensionSize), 
    dimensionSize(_dimensionSize)
    { }

    inline size_t size() const { return dimensionSize; }

    inline tensorProxy operator[](size_t idx) {
        return {container,idx};
    }
};