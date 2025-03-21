# Design Patterns

- [Singleton Pattern](#singleton-pattern)
- [Abstract factory method](#abstract-factory-method)
- [AbstractFactory with a parameterized method](Factory-with-a-parameterized-method)
- [Builder Pattern](Builder-Pattern)
  

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
```

### Abstract factory method
Encapsulates object creation inside a separate factory class
Allows extensibility—new product types can be added without modifying existing code.
Reduces coupling between classes by using polymorphism.
```cpp
//abstract factory product
class Shape
{
public:
	virtual void draw() = 0;
};

class rectangle: public Shape
{
public:
	void draw() override {
		std::cout<<"rectangle "<<"\n";
	}
};

class circle: public Shape
{
public:
	void draw() override {
		std::cout<<"circle "<<"\n";
	}
};

//abstract factory method
class AbstractFactoryShape
{
public:

	virtual std::unique_ptr<Shape> createabstractMetod()=0;

};

class rectangleAbstractFactory: public AbstractFactoryShape
{
public:
	std::unique_ptr<Shape> createabstractMetod() override
	{
		return std::make_unique<rectangle>();
	}
};
class circleAbstractFactory:public AbstractFactoryShape
{
public:
	std::unique_ptr<Shape>createabstractMetod() override
	{
		return  std::make_unique<circle>();
	}
};
	std::unique_ptr<AbstractFactoryShape> factory;
	factory = std::make_unique<circleAbstractFactory>();
	std::unique_ptr<Shape> shape1 = factory->createabstractMetod();
	shape1->draw();
```
### AbstractFactory with a parameterized method
Instead of having multiple factory classes (CircleFactory, RectangleFactory), we will have one factory class that takes a parameter to decide which object to create.

```C++
// Factory with a parameterized method
class ShapeFactory {
public:
	enum class ShapeType { CIRCLE, RECTANGLE };

	static std::unique_ptr<Shape> createShape(ShapeType type) {
		switch (type) {
		case ShapeType::CIRCLE:
			return std::make_unique<circle>();
		case ShapeType::RECTANGLE:
			return std::make_unique<rectangle>();
		default:
			return nullptr;
		}
	}
};

	auto shape = ShapeFactory::createShape(ShapeFactory::ShapeType::RECTANGLE);
	shape->draw();
```
## Builder Pattern 
The Builder Pattern is a creational design pattern that helps construct complex objects step by step. Unlike Factory patterns, which focus on instantiating objects, the Builder pattern separates construction logic from the object itself, making it easier to create complex structures.
Encapsulates object creation into step-by-step methods.
✅ Fluent interface makes code more readable.
✅ Reduces constructor overload complexity.
✅ Allows immutable objects while supporting optional parameters.

```C++
// Product: Car
class Car {
private:
    std::string engine;
    int seats;
    bool hasGPS;
    bool hasSunroof;

public:
    // Constructor is private to enforce object creation via Builder
    Car(std::string engineType, int seatCount, bool gps, bool sunroof)
        : engine(std::move(engineType)), seats(seatCount), hasGPS(gps), hasSunroof(sunroof) {}

    void showSpecifications() const {
        std::cout << "Car Specifications:\n"
                  << "Engine: " << engine << "\n"
                  << "Seats: " << seats << "\n"
                  << "GPS: " << (hasGPS ? "Yes" : "No") << "\n"
                  << "Sunroof: " << (hasSunroof ? "Yes" : "No") << "\n";
    }
};

// Builder Class
class CarBuilder {
private:
    std::string engine = "Standard"; // Default values
    int seats = 4;
    bool hasGPS = false;
    bool hasSunroof = false;

public:
    CarBuilder& setEngine(const std::string& engineType) {
        engine = engineType;
        return *this;
    }

    CarBuilder& setSeats(int seatCount) {
        seats = seatCount;
        return *this;
    }

    CarBuilder& addGPS() {
        hasGPS = true;
        return *this;
    }

    CarBuilder& addSunroof() {
        hasSunroof = true;
        return *this;
    }

    // Build the final Car object
    std::unique_ptr<Car> build() {
        return std::make_unique<Car>(engine, seats, hasGPS, hasSunroof);
    }
};
    // Build a Luxury Car with GPS and Sunroof
    auto luxuryCar = CarBuilder()
                        .setEngine("V8")
                        .setSeats(2)
                        .addGPS()
                        .addSunroof()
                        .build();

    luxuryCar->showSpecifications();

    std::cout << "\n";

    // Build a Basic Car
    auto basicCar = CarBuilder()
                        .setEngine("Electric")
                        .setSeats(5)
                        .build();

    basicCar->showSpecifications();
```
