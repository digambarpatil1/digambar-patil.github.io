# Smart Pointers in C++
- [Overview](#overview)
- [std::unique_ptr](#stdunique_ptr)
- [std::shared_ptr](#stdshared_ptr)
- [std::weak_ptr](#stdweak_ptr)
- [The Circular Dependency Problem in Observer Pattern](#the-circular-dependency-problem-in-observer-pattern)
- [Own uniquepointer implimentation](#Own-uniquepointer-implimentation)

## Overview
the use of **smart pointers** in C++: \`std::unique_ptr\`, \`std::shared_ptr\`, and \`std::weak_ptr\`. These smart pointers help manage dynamic memory efficiently and safely.

### \`std::unique_ptr\`
- **Exclusive ownership** of a resource.
- Ownership **can be transferred** but **not copied**.
- Used for managing resources like **file handles, database connections**.
- \`std::make_unique<T>(...)\` is the preferred way to create a \`unique_ptr\`.

```cpp
std::unique_ptr<int> uptr = std::make_unique<int>(20);
```

- Calling \`uptr.get()\` returns the **raw pointer** without transferring ownership.
- \`std::move(uptr)\` transfers ownership to another \`unique_ptr\`.

### \`std::shared_ptr\`
- **Allows multiple shared owners** of a resource.
- The resource is deleted when the **last \`shared_ptr\`** managing it is destroyed.
- Uses **reference counting**.
  
```cpp
std::shared_ptr<int> sptr = std::make_shared<int>(30);
std::shared_ptr<int> sptr1 = sptr; // Increases reference count
```

### \`std::weak_ptr\`
- Observes a resource **without affecting its lifetime**.
- Used to **break circular references**.
- \`lock()\` converts it into a \`shared_ptr\` **if the resource is still valid**.

```cpp
std::weak_ptr<int> weakPtr = sptr;
std::cout << "*weakPtr = " << *weakPtr.lock() << std::endl;
```

```cpp
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
```
# The Circular Dependency Problem in Observer Pattern

## Problem
When implementing the **Observer Pattern** using `std::shared_ptr` in both `Subject` and `Observer` classes, a **circular reference** can occur. This leads to memory leaks because the reference count never reaches zero.
### Scenario
- The `Subject` maintains a `std::vector<std::shared_ptr<Observer>>`, keeping all observers alive.
- Each `Observer` holds a `std::shared_ptr<Subject>` to interact with the subject or unregister itself.
- This creates a **circular reference**, preventing destruction even when no external references exist.
## Solution
To break this cycle, we replace `std::shared_ptr<Observer>` with `std::weak_ptr<Observer>` in the `Subject` class. This allows observers to be garbage collected when they are no longer referenced externally.
## Implementation
```cpp
class Observer {
public:
    virtual void update() = 0; // Interface for observers
    virtual ~Observer() = default;
};
class Subject {
public:
    void addObserver(std::weak_ptr<Observer> observer) {
        observers.push_back(observer);
    }
    void notify() {
        for (auto& weakObserver : observers) {
            if (auto observer = weakObserver.lock()) { // Check if observer is still alive
                observer->update();
            }
        }
    }
private:
    std::vector<std::weak_ptr<Observer>> observers; // Weak pointers to avoid circular references
};
class ConcreteObserver: public Observer {
public:
    explicit ConcreteObserver(std::string name) : name(std::move(name)) {}
    void update() override {
        std::cout << name << " received notification." << std::endl;
    }

private:
    std::string name;
};
int main() {
    std::shared_ptr<Subject> subject = std::make_shared<Subject>();

    std::shared_ptr<ConcreteObserver> observer1 = std::make_shared<ConcreteObserver>("Observer 1");
    std::shared_ptr<ConcreteObserver> observer2 = std::make_shared<ConcreteObserver>("Observer 2");

    subject->addObserver(observer1);
    subject->addObserver(observer2);

    subject->notify();  // Both observers receive notification
    observer1.reset();  // Observer 1 is removed
    subject->notify();  // Only Observer 2 is notified
    
    return 0;
}
```
# Own uniquepointer implimentation
```C++
#include <iostream>
template<typename T>
class uniqueptr
{
private:

	T* ptr;

public:
	explicit uniqueptr(T* p =nullptr):ptr(p) {}

	uniqueptr(const uniqueptr& obj)=delete;
	uniqueptr& operator=(uniqueptr& obj)=delete;

	uniqueptr(uniqueptr&& obj)noexcept :ptr(obj.ptr)
	{
		obj.ptr = nullptr;
	}

	uniqueptr& operator=(uniqueptr&& obj)
	{
		if (this != &obj) {
			delete ptr;   // Clean up existing resource
			ptr=obj.ptr;
			obj.ptr = nullptr;
		}

		return *this;
	}
	~uniqueptr()
	{
		delete ptr;
	}

	// Overload derefernce operator
	T& operator*()const
	{
		return *ptr;
	}
	// Overload arrow operator
	T* operator->()const
	{
		return ptr;
	}
	// Get raw pointer
	T* get() const {
		return ptr;
	}

	// Release ownership and return raw pointer
	T* release() {
		T* temp = ptr;
		ptr = nullptr;
		return temp;
	}

	// Reset with a new pointer
	void reset(T* newPtr = nullptr) {
		delete ptr;
		ptr = newPtr;
	}
};

int main()
{
	uniqueptr<int> uptr1(new int(42));
	std::cout << "Value: " << *uptr1 << std::endl;

	// Transfer ownership using move constructor
	uniqueptr<int> uptr2 = std::move(uptr1);
	std::cout << "After move, uptr2 value: " << *uptr2 << std::endl;

	// Reset with a new resource
	uptr2.reset(new int(100));
	std::cout << "After reset, uptr2 value: " << *uptr2 << std::endl;
	return 0;
}
```

