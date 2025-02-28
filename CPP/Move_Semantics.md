**Move semantics**

Moving an object means to transfer ownership of some resource it manages to another object.

The first benefit of move semantics is performance optimization. When an object is about to reach the end of its lifetime, either because it's temporary or by explicitly calling std::move, a move is often a cheaper way to transfer resources, for example, moving a std::vector is just copying some pointers and internal state over to the new vector -- copying would involve copying every single element in the vector, which is expensive and unnecessary if the old vector will soon be destroyed.

Moves also make it possible for non-copyable types such as std::unique_ptrs (smart pointers) to guarantee at the language level that there is only ever one instance of a resource being managed at a time while being able to transfer an instance between scopes.

code link:[CPP/move_semantics.cpp](https://github.com/digambarpatil1/dev-experiments/blob/main/CPP/move_semantics.cpp)

 rvalue reference:
C++11 introduces a new reference termed the rvalue reference. An rvalue reference to T, which is a non-template type parameter (such as int, or a user-defined type), is created with the syntax T&&. Rvalue references only bind to rvalues.
