# Smart Pointers in C++

## Overview
This document explains the use of **smart pointers** in C++: \`std::unique_ptr\`, \`std::shared_ptr\`, and \`std::weak_ptr\`. These smart pointers help manage dynamic memory efficiently and safely.

## Code Example

\`\`\`cpp
#include <iostream>
#include <memory>

int main()
{
    // Owns a resource exclusively
    // Ownership transferred to uptr2 but not copyable
    // Excellent for managing resources like file handles or database connections
    std::unique_ptr<int> uptr = std::make_unique<int>(20);
    // std::unique_ptr<int> uptr2 = std::move(uptr);
    
    // Returns the raw pointer without transferring ownership
    int* rawPtr = uptr.get();

    // Allows multiple shared owners of a resource.
    // The resource is deleted when the last shared_ptr that owns it is destroyed.
    std::shared_ptr<int> sptr = std::make_shared<int>(30);
    std::shared_ptr<int> sptr1 = sptr;

    // Allows observing a resource managed by a shared_ptr without affecting its lifetime.
    // Useful to break circular references.
    std::weak_ptr<int> weakPtr = sptr;  // weak_ptr does not affect the reference count

    std::cout << "*unique_ptr = " << *uptr << std::endl;
    std::cout << "*rawPtr = " << *rawPtr << std::endl;
    std::cout << "*shared_ptr = " << *sptr << std::endl;
    std::cout << "*shared_ptr copy = " << *sptr1 << std::endl;
    std::cout << "*weakPtr = " << *weakPtr.lock() << std::endl;

    return 0;
}
\`\`\`

## Explanation

### \`std::unique_ptr\`
- **Exclusive ownership** of a resource.
- Ownership **can be transferred** but **not copied**.
- Used for managing resources like **file handles, database connections**.
- \`std::make_unique<T>(...)\` is the preferred way to create a \`unique_ptr\`.

\`\`\`cpp
std::unique_ptr<int> uptr = std::make_unique<int>(20);
\`\`\`

- Calling \`uptr.get()\` returns the **raw pointer** without transferring ownership.
- \`std::move(uptr)\` transfers ownership to another \`unique_ptr\`.

### \`std::shared_ptr\`
- **Allows multiple shared owners** of a resource.
- The resource is deleted when the **last \`shared_ptr\`** managing it is destroyed.
- Uses **reference counting**.

\`\`\`cpp
std::shared_ptr<int> sptr = std::make_shared<int>(30);
std::shared_ptr<int> sptr1 = sptr; // Increases reference count
\`\`\`

### \`std::weak_ptr\`
- Observes a resource **without affecting its lifetime**.
- Used to **break circular references**.
- \`lock()\` converts it into a \`shared_ptr\` **if the resource is still valid**.

\`\`\`cpp
std::weak_ptr<int> weakPtr = sptr;
std::cout << "*weakPtr = " << *weakPtr.lock() << std::endl;
\`\`\`

