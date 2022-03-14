#include <stack>
#include <vector>
template <typename T,T*& addr>
class Pack
{
    std::stack<int> operationStack;
    std::vector<T> data;
    int current;

    void setCurrent(int current) {
        addr = &data[current];
    }

    public:

    Pack() {
        current = 0;
        resize(1);
    }

    void resize(int n) {
        data.resize(n);
        setCurrent(current);
    }

    void push(int i) {
        operationStack.push(current);
        current = i;
        setCurrent(current);
    }

    void pop() {
        current = operationStack.top();
        setCurrent(current);
    }
};