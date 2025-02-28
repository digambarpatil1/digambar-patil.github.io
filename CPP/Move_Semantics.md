# Move Semantics in C++

## �� What is Move Semantics?
Move semantics allow the efficient transfer of resources from one object to another without deep copying.

## �� Why Do We Need Move Semantics?
- **Performance Boost:** Avoids unnecessary deep copies.
- **Efficient Resource Management:** Useful when dealing with dynamic memory, file handles, etc.
- **Rvalue Optimization:** Helps when dealing with temporary objects.

## �� When is Move Constructor Called?
| Scenario | Move Constructor Called? | Reason |
|----------|----------------|---------|
| `Data d = createData();` | ❌ (RVO applies) | Object is constructed directly. |
| `Data d = std::move(createData());` | ✅ | Explicit move requested. |
| `Data d2 = std::move(d1);` | ✅ | `d1` is an rvalue. |

## ⚡ Example Code (GitHub Link)
[�� Move Semantics Example](https://github.com/digambarpatil1/dev-experiments/blob/main/CPP/move_semantics.cpp)


## ⚙️ How to Compile & Run
```sh
g++ -std=c++17 -fno-elide-constructors move_semantics.cpp -o move_test
./move_test

