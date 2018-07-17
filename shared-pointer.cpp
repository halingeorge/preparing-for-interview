#include <iostream>

template <class T>
class shared_ptr {
public:
    explicit shared_ptr() : ptr(nullptr), counter(nullptr) {}

    explicit shared_ptr(T *t) : ptr(t), counter(new int(1)) {
        std::cout << "constructor\n";
    }

    void swap(shared_ptr &t) {
        std::swap(ptr, t.ptr);
        std::swap(counter, t.counter);
    }

    shared_ptr(const shared_ptr &t) {
        std::cout << "copy constructor\n";
        ptr = t.ptr;
        counter = t.counter;
        ++(*counter);
    }

    shared_ptr(shared_ptr &&t) : ptr(std::move(t.ptr)), counter(std::move(t.counter)) {
        std::cout << "move constructor\n";
    }

    shared_ptr& operator=(shared_ptr &&t)  {
        std::cout << "operator=\n";
        swap(t);
    }

    ~shared_ptr() {
        std::cout << "~shared_ptr()\n";
        if (counter != nullptr) {
            --(*counter);
            if (!(*counter)) {
                if (ptr != nullptr) {
                    delete ptr;
                }
                delete counter;
            }
        }
    }

    int use_count() const {
        return *counter;
    }

    T* operator->() {
        return ptr;
    }

    T operator*() {
        return *ptr;
    }

private:
    T *ptr;
    int *counter;
};

int main() {
    shared_ptr<int> sh4;
    sh4 = shared_ptr<int>(new int(10));
    std::cout << *sh4 << "\n";
}