#include <iostream>

template <typename T>
class SharedPointer {
public:
    // Constructors
    SharedPointer() : ptr(nullptr), refCount(nullptr) {}

    // Explicit overloader constructor
    explicit SharedPointer(T* ptr) : ptr(ptr) {
        std::cout << "Constructor: SharedPointer" << std::endl;
        refCount = new unsigned int(1); // Start with reference count 1
    }

    // Copy constructor
    SharedPointer(const SharedPointer& other) : ptr(other.ptr), refCount(other.refCount) {
        std::cout << "Copy-Constructor: SharedPointer&" << std::endl;
        if (refCount) {
            (*refCount)++; // Increase the reference count
        }
    }

    // Copy assignment
    SharedPointer& operator=(const SharedPointer& other) {
        std::cout << "Operator= : SharedPointer=" << std::endl;
        if (this != &other) {
            release(); // Release the current memory

            ptr = other.ptr;
            refCount = other.refCount;

            if (refCount) {
                (*refCount)++; // Increase the reference count
            }
        }
        return *this;
    }

    SharedPointer(SharedPointer&& other) = delete;
    SharedPointer& operator=(SharedPointer&& other) = delete;

    // Destructor
    ~SharedPointer() {
        std::cout << "Destructor: ~SharedPointer" << std::endl;
        release(); // Release the memory and decrement the reference count
    }

    // Overloaded dereference operator
    T& operator*() const {
        return *ptr;
    }

    // Overloaded arrow operator
    T* operator->() const {
        return ptr;
    }

    //refcount --> use_count
    unsigned int use_count() { return *refCount; }

private:
    T* ptr=nullptr;
    unsigned int* refCount=nullptr;

    // Release the memory and decrement the reference count
    void release() {
        if (refCount) {
            (*refCount)--;
            if (*refCount == 0) {
                delete ptr;
                delete refCount;
            }
            ptr = nullptr;
            refCount = nullptr;
        }
    }
};

int main() {
    // Using the custom SharedPointer class
    SharedPointer<int> sp1(new int(42));
    std::cout << "sp1 use_count: " << sp1.use_count() << ", *sp1: " << *sp1 << "\n" << std::endl;
    SharedPointer<int> sp2 = sp1; // Copy constructor, increases reference count

    std::cout << "sp1 use_count: " << sp1.use_count() << ", *sp1: " << *sp1 << std::endl;
    std::cout << "sp2 use_count: " << sp2.use_count() << ", *sp2: " << *sp2 << "\n" << std::endl;

    SharedPointer<int> sp3;
    sp3 = sp1; // Copy assignment, increases reference count

    std::cout << "sp1 use_count: " << sp1.use_count() << ", *sp1: " << *sp1 << std::endl;
    std::cout << "sp2 use_count: " << sp2.use_count() << ", *sp2: " << *sp2 << std::endl;
    std::cout << "sp3 use_count: " << sp3.use_count() << ", *sp3: " << *sp3 << "\n" << std::endl;

    return 0;
}
