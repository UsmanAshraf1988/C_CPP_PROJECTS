#include <iostream>

template <typename T>
class SharedPointer;

template <typename T>
class WeakPtr {
private:
    T* ptr;
    const SharedPointer<T>& shrdPtr;
    WeakPtr();
    WeakPtr(T* p);

public:

    WeakPtr(const SharedPointer<T>& shrPtr): shrdPtr(shrPtr){
        ptr = shrPtr.operator->();
    }

    WeakPtr& operator=(const SharedPointer<T>& shrPtr) {
        ptr = shrPtr.operator->();
        shrdPtr = shrPtr;
        return *this; 
    }

    // Destructor: No need to delete anything
    ~WeakPtr() = default;

    // Lock: Convert to shared_ptr if the object still exists
    SharedPointer<T> lock() const {
        return ptr? SharedPointer<T>(shrdPtr): SharedPointer<T>();
    }

    // Reset: Release the tracked object
    void reset() {
        ptr = nullptr;
    }

    // Check if the tracked object exists
    bool expired() const {
        return (ptr==nullptr)? true : false;
    }
};




template <typename T>
class SharedPointer {
public:
    // Constructors
    SharedPointer() : ptr(nullptr), refCount(nullptr) {}

    // Explicit overloader constructor
    explicit SharedPointer(T* ptr) : ptr(ptr) {
        std::cout << "Constructor: SharedPointer" << std::endl;
        refCount = new volatile unsigned int(1); // Start with reference count 1
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

    SharedPointer(SharedPointer&& other) noexcept {
        std::cout << "Move Constructor: SharedPointer&&" << std::endl;
        ptr = other.ptr;
        refCount = other.refCount;
        other.makeItNullPtr();
    }
    SharedPointer& operator=(SharedPointer&& other) noexcept {
        std::cout << "Operator= &&: SharedPointer&&" << std::endl;
        if (this != &other) {
            release(); // Release the current memory

            ptr = other.ptr;
            refCount = other.refCount;

            other.makeItNullPtr();
        }
        return *this;
    }

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

    operator bool() const {
        return (*refCount>=1)? true: false;
    }

    //refcount --> use_count
    volatile unsigned int use_count() { return *refCount; }

    void makeItNullPtr() {
        ptr = nullptr;
        refCount = nullptr;
    }

private:
    T* ptr = nullptr;
    volatile unsigned int* refCount = nullptr;

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
    SharedPointer<int> shrPtr(new int(42));
    WeakPtr<int> weakInt = shrPtr;

    std::cout << "shrPtr.use_count(): " << shrPtr.use_count() << ", *shrPtr: " << *shrPtr << "\n" << std::endl;

    {
        
        if (auto sharedInt = weakInt.lock()) {
            std::cout << "shrPtr.use_count(): " << shrPtr.use_count() << ", *shrPtr: " << *shrPtr << std::endl;
            std::cout << "Value: " << *sharedInt << std::endl;
        }
        else {
            std::cout << "Object has been deleted." << std::endl;
        }
    }

    std::cout << "shrPtr.use_count(): " << shrPtr.use_count() << ", *shrPtr: " << *shrPtr << "\n" << std::endl;
    // Simulate object deletion
    //delete rawInt;
    weakInt.reset();

    if (weakInt.expired()) {
        std::cout << "WeakPtr is empty." << std::endl;
    }
    
    return 0;
}
