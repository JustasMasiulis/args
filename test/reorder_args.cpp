// reorder_args.cpp : Defines the entry point for the application.
//

#include "../include/reorder_args.hpp"
#include <iostream>

void fn(int a, float b, void* c) { std::cout << a << b << c; }
auto fn2 = [](int a, float b, void* c) { std::cout << a << b << c; };
auto fn3 = [ fn = fn2, ofn = fn ](auto&&... args) { return (fn(args...), ofn(args...)); };


int main()
{
    auto reordered1 = jm::reorder_args<2, 1, 0>(fn);
    // should be the size of a function pointer
    static_assert(sizeof(reordered1) == sizeof(void*), "invalid size");
    reordered1(nullptr, 2.f, 1);


    auto reordered2 = jm::reorder_args<2, 1, 0>(fn2);
    std::cout << '\n' << sizeof(reordered2) << ' ' << sizeof(fn2) << '\n';
    static_assert(sizeof(reordered2) == sizeof(fn2), "invalid size");
    reordered2(nullptr, 2.f, 1);

    auto reordered3 = jm::reorder_args<2, 1, 0>(fn3);
    static_assert(sizeof(reordered3) == sizeof(fn3), "invalid size");
    reordered3(nullptr, 2.f, 1);

	auto reordered4 = jm::reorder_args<2, 1, 0>(std::ref(fn3));
    static_assert(sizeof(reordered4) == sizeof(void*), "invalid size");
    reordered4(nullptr, 2.f, 1);
}
