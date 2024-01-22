#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <algorithm>

// Function object for the transform example
struct Original {
    int operator()(int x) const {
        return x;
    }
};

int main() {
    // Vector
    std::vector<int> myVector = {1, 2, 3, 4, 5};
    myVector.push_back(6);  // Adds the value 6 to the end of the vector
    myVector.pop_back();    // Removes the last element from the vector

    // Display the elements of the vector
    std::cout << "Vector: ";
    for (const auto& element : myVector) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    // List
    std::list<int> myList = {1, 2, 3, 4, 5};
    myList.push_front(0);   // Adds the value 0 to the front of the list
    myList.pop_back();       // Removes the last element from the list

    // Display the elements of the list
    std::cout << "List: ";
    for (const auto& element : myList) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    // Queue
    std::queue<int> myQueue;
    for (int i = 1; i <= 5; ++i) {
        myQueue.push(i);    // Enqueues the value i
    }

    // Display the elements of the queue
    std::cout << "Queue: ";
    while (!myQueue.empty()) {
        std::cout << myQueue.front() << " ";  // Displays the front element of the queue
        myQueue.pop();      // Dequeues (removes) the front element from the queue
    }
    std::cout << "\n";

    // Stack
    std::stack<int> myStack;
    for (int i = 1; i <= 5; ++i) {
        myStack.push(i);    // Pushes the value i onto the stack
    }

    // Display the elements of the stack
    std::cout << "Stack: ";
    while (!myStack.empty()) {
        std::cout << myStack.top() << " ";   // Displays the top element of the stack
        myStack.pop();      // Pops (removes) the top element from the stack
    }
    std::cout << "\n";

    // Map
    std::map<std::string, int> myMap;
    myMap["one"] = 1;       // Inserts a key-value pair into the map
    myMap["two"] = 2;
    myMap["three"] = 3;

    // Display the elements of the map
    std::cout << "Map: ";
    for (const auto& pair : myMap) {
        std::cout << pair.first << ": " << pair.second << " ";
    }
    std::cout << "\n";

    // Set
    std::set<int> mySet = {4, 2, 1, 3, 5};
    mySet.insert(6);        // Inserts the value 6 into the set

    // Display the elements of the set
    std::cout << "Set: ";
    for (const auto& element : mySet) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    // Sort
    std::vector<int> sortVector = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    std::sort(sortVector.begin(), sortVector.end());

    // Display the sorted elements of the vector
    std::cout << "Sorted Vector: ";
    for (const auto& element : sortVector) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    // Find
    auto findIt = std::find(sortVector.begin(), sortVector.end(), 5);
    if (findIt != sortVector.end()) {
        // Display the index where the element is found
        std::cout << "Element found at index: " << std::distance(sortVector.begin(), findIt) << "\n";
    } else {
        std::cout << "Element not found\n";
    }

    // Transform
    std::vector<int> transformInput = {1, 2, 3, 4, 5};
    std::vector<int> transformResult;

    // Original each element using transform
    std::transform(transformInput.begin(), transformInput.end(), std::back_inserter(transformResult), Original());

    // Display the transformed elements
    std::cout << "Transformed Vector: ";
    for (const auto& element : transformResult) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    return 0;
}
