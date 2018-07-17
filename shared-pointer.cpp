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
    shared_ptr(T *t = nullptr) : ptr(t), counter(new RefCounter(t != nullptr)) {}

    shared_ptr(shared_ptr &t) {
        if (this != &ptr) {
            ptr = t.ptr;
            counter = t.counter;
            counter->AddRef();
        }
    }

    shared_ptr(shared_ptr &&t) {
        ptr = t.ptr;
        counter = t.counter;
    }

    shared_ptr& operator=(shared_ptr &t) {
        if (this != &t) {
            counter->ReleaseRef();
            if (!counter->Get()) {
                delete ptr;
                delete counter;
            }

            ptr = t.ptr;
            counter = t.counter;
            counter->AddRef();
        }
    }

    ~shared_ptr() {
        counter->ReleaseRef();
        if (!counter->Get()) {
            delete ptr;
            delete counter;
        }
    }

private:
    T *ptr;
    RefCounter *counter;
};

struct Foo {
    Foo() { std::cout << "Foo...\n"; }
    ~Foo() { std::cout << "~Foo...\n"; }
};

struct D {
    void operator()(Foo* p) const {
        std::cout << "Call delete for Foo object...\n";
        delete p;
    }
};

int main() {
    std::shared_ptr<Foo> sh1;

    // constructor with object
    std::shared_ptr<Foo> sh2(new Foo);
    std::shared_ptr<Foo> sh3(sh2);
    std::cout << sh2.use_count() << '\n';
    std::cout << sh3.use_count() << '\n';

    //constructor with object and deleter
    std::shared_ptr<Foo> sh4(new Foo, D());
}