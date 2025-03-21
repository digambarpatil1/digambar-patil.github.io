# Design Patterns

- [Singleton Pattern](#singleton-pattern)

## Singleton Pattern
- Manages shared resources (e.g., logging, configuration settings, database connections).
- Ensures a single point of control (e.g., managing a game engine instance).

```cpp
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance; // Guaranteed to be initialized only once
        return instance;
    }

    void showMessage() {
        std::cout << "Singleton instance running!" << std::endl;
    }

private:
    Singleton() {}                          // Private constructor
    ~Singleton() {}                         // Private destructor
    Singleton(const Singleton&) = delete;   // Prevent copy
    Singleton& operator=(const Singleton&) = delete; // Prevent assignment
};

int main() {
    Singleton& instance = Singleton::getInstance();
    instance.showMessage();
    return 0;
}
