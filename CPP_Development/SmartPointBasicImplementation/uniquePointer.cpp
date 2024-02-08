#include <iostream>

template <typename T>
class UniquePointer {
public:
    // Constructors
    UniquePointer() : ptr(nullptr) {}

    explicit UniquePointer(T* ptr) : ptr(ptr) {}

    UniquePointer(const UniquePointer& other) = delete;
    UniquePointer& operator=(const UniquePointer& other) = delete;

    // Move constructor
    UniquePointer(UniquePointer&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr; // Transfer ownership
    }

    // Move assignment
    UniquePointer& operator=(UniquePointer&& other) noexcept {
        if (this != &other) {
            reset(); // Release the current memory

            ptr = other.ptr;
            other.ptr = nullptr; // Transfer ownership
        }
        return *this;
    }

    // Destructor
    ~UniquePointer() {
        reset(); // Release the memory
    }

    // Overloaded dereference operator
    T& operator*() const {
        return *ptr;
    }

    // Overloaded arrow operator
    T* operator->() const {
        return ptr;
    }

    // Release the memory
    void reset() {
        if (ptr) {
            delete ptr;
            ptr = nullptr;
        }
    }

    // Release the memory and return the raw pointer
    T* release() {
        T* releasedPtr = ptr;
        ptr = nullptr;
        return releasedPtr;
    }

private:
    T* ptr;
};

int main() {
    // Using the custom UniquePointer class
    UniquePointer<int> up1(new int(42));
    UniquePointer<int> up2 = std::move(up1); // Move constructor, transfers ownership

    std::cout << "up1: " << (up1.operator->() ? *up1 : 0) << std::endl; // Should print 0
    std::cout << "up2: " << *up2 << std::endl;

    UniquePointer<int> up3;
    up3 = std::move(up2); // Move assignment, transfers ownership

    std::cout << "up2: " << (up2.operator->() ? *up2 : 0) << std::endl; // Should print 0
    std::cout << "up3: " << *up3 << std::endl;

    return 0;
}

