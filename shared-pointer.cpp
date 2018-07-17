#include <iostream>

class RefCounter {
public:
    RefCounter(int counter = 0) : counter(counter) {}

    void AddRef() {
        ++counter;
    }

    void ReleaseRef() {
        --counter;
    }

    int Get() const {
        return counter;
    }

private:
    int counter;
};

template <class T>
class shared_ptr {
public:
    explicit shared_ptr() : ptr(nullptr), counter(nullptr) {}

    explicit shared_ptr(T *t) : ptr(t), counter(new RefCounter(1)) {
        std::cout << "constructor\n";
    }

    shared_ptr(const shared_ptr &t) {
        std::cout << "copy constructor\n";
        ptr = t.ptr;
        counter = t.counter;
        counter->AddRef();
    }

    shared_ptr(shared_ptr &&t) : ptr(std::move(t.ptr)), counter(std::move(t.counter)) {
        std::cout << "move constructor\n";
    }

    shared_ptr& operator=(shared_ptr t)  {
        std::cout << "operator=\n";
        std::swap(ptr, t.ptr);
        std::swap(counter, t.counter);
    }

    ~shared_ptr() {
        counter->ReleaseRef();
        if (!counter->Get()) {
            if (ptr != nullptr) {
                delete ptr;
            }
            delete counter;
        }
    }

    int use_count() const {
        return counter->Get();
    }

    T* operator->() {
        return ptr;
    }

    T operator*() {
        return *ptr;
    }

private:
    T *ptr;
    RefCounter *counter;
};

int main() {
    shared_ptr<int> sh4;
    sh4 = shared_ptr<int>(new int(10));
    std::cout << *sh4 << "\n";
}