// reorder_args.cpp : Defines the entry point for the application.
//

#include "../include/reorder_args.hpp"
#include <iostream>
#include <cassert>

void fn(int a, float b, void* c) { assert(a == 1 && b == 2.f && c == nullptr); }
auto fn2 = [](int a, float b, void* c) { assert(a == 1 && b == 2.f && c == nullptr); };
auto fn3 = [mfn = fn2](auto&&... args) { return fn(args...); };

void reorder()
{
    args::reorder<0, 1, 2>(fn)(1, 2.f, nullptr);
    args::reorder<0, 2, 1>(fn)(1, nullptr, 2.f);
    args::reorder<1, 0, 2>(fn)(2.f, 1, nullptr);
    args::reorder<1, 2, 0>(fn)(2.f, nullptr, 1);
    args::reorder<2, 0, 1>(fn)(nullptr, 1, 2.f);
    args::reorder<2, 1, 0>(fn)(nullptr, 2.f, 1);

    args::reorder<0, 1, 2>(fn2)(1, 2.f, nullptr);
    args::reorder<0, 2, 1>(fn2)(1, nullptr, 2.f);
    args::reorder<1, 0, 2>(fn2)(2.f, 1, nullptr);
    args::reorder<1, 2, 0>(fn2)(2.f, nullptr, 1);
    args::reorder<2, 0, 1>(fn2)(nullptr, 1, 2.f);
    args::reorder<2, 1, 0>(fn2)(nullptr, 2.f, 1);

    args::reorder<0, 1, 2>(fn3)(1, 2.f, nullptr);
    args::reorder<0, 2, 1>(fn3)(1, nullptr, 2.f);
    args::reorder<1, 0, 2>(fn3)(2.f, 1, nullptr);
    args::reorder<1, 2, 0>(fn3)(2.f, nullptr, 1);
    args::reorder<2, 0, 1>(fn3)(nullptr, 1, 2.f);
    args::reorder<2, 1, 0>(fn3)(nullptr, 2.f, 1);
}

void bind()
{
    args::bind<0>(fn, 1)(2.f, nullptr);
    args::bind<1>(fn, 2.f)(1, nullptr);
    args::bind<2>(fn, nullptr)(1, 2.f);

    args::bind<0, 1>(fn, 1, 2.f)(nullptr);
    args::bind<0, 2>(fn, 1, nullptr)(2.f);
    args::bind<1, 0>(fn, 2.f, 1)(nullptr);
    args::bind<1, 2>(fn, 2.f, nullptr)(1);
    args::bind<2, 0>(fn, nullptr, 1)(2.f);
    args::bind<2, 1>(fn, nullptr, 2.f)(1);

    args::bind<0, 1, 2>(fn, 1, 2.f, nullptr)();
    args::bind<0, 2, 1>(fn, 1, nullptr, 2.f)();
    args::bind<1, 0, 2>(fn, 2.f, 1, nullptr)();
    args::bind<1, 2, 0>(fn, 2.f, nullptr, 1)();
    args::bind<2, 0, 1>(fn, nullptr, 1, 2.f)();
    args::bind<2, 1, 0>(fn, nullptr, 2.f, 1)();


    args::bind<0>(fn2, 1)(2.f, nullptr);
    args::bind<1>(fn2, 2.f)(1, nullptr);
    args::bind<2>(fn2, nullptr)(1, 2.f);

    args::bind<0, 1>(fn2, 1, 2.f)(nullptr);
    args::bind<0, 2>(fn2, 1, nullptr)(2.f);
    args::bind<1, 0>(fn2, 2.f, 1)(nullptr);
    args::bind<1, 2>(fn2, 2.f, nullptr)(1);
    args::bind<2, 0>(fn2, nullptr, 1)(2.f);
    args::bind<2, 1>(fn2, nullptr, 2.f)(1);

    args::bind<0, 1, 2>(fn2, 1, 2.f, nullptr)();
    args::bind<0, 2, 1>(fn2, 1, nullptr, 2.f)();
    args::bind<1, 0, 2>(fn2, 2.f, 1, nullptr)();
    args::bind<1, 2, 0>(fn2, 2.f, nullptr, 1)();
    args::bind<2, 0, 1>(fn2, nullptr, 1, 2.f)();
    args::bind<2, 1, 0>(fn2, nullptr, 2.f, 1)();


    args::bind<0>(fn3, 1)(2.f, nullptr);
    args::bind<1>(fn3, 2.f)(1, nullptr);
    args::bind<2>(fn3, nullptr)(1, 2.f);

    args::bind<0, 1>(fn3, 1, 2.f)(nullptr);
    args::bind<0, 2>(fn3, 1, nullptr)(2.f);
    args::bind<1, 0>(fn3, 2.f, 1)(nullptr);
    args::bind<1, 2>(fn3, 2.f, nullptr)(1);
    args::bind<2, 0>(fn3, nullptr, 1)(2.f);
    args::bind<2, 1>(fn3, nullptr, 2.f)(1);

    args::bind<0, 1, 2>(fn3, 1, 2.f, nullptr)();
    args::bind<0, 2, 1>(fn3, 1, nullptr, 2.f)();
    args::bind<1, 0, 2>(fn3, 2.f, 1, nullptr)();
    args::bind<1, 2, 0>(fn3, 2.f, nullptr, 1)();
    args::bind<2, 0, 1>(fn3, nullptr, 1, 2.f)();
    args::bind<2, 1, 0>(fn3, nullptr, 2.f, 1)();
}

void curry()
{
    args::curry<0>(fn)(1)(2.f, nullptr);
    args::curry<1>(fn)(2.f)(1, nullptr);
    args::curry<2>(fn)(nullptr)(1, 2.f);

    args::curry<0, 1>(fn)(1, 2.f)(nullptr);
    args::curry<0, 2>(fn)(1, nullptr)(2.f);
    args::curry<1, 0>(fn)(2.f, 1)(nullptr);
    args::curry<1, 2>(fn)(2.f, nullptr)(1);
    args::curry<2, 0>(fn)(nullptr, 1)(2.f);
    args::curry<2, 1>(fn)(nullptr, 2.f)(1);

    args::curry<0, 1, 2>(fn)(1, 2.f, nullptr)();
    args::curry<0, 2, 1>(fn)(1, nullptr, 2.f)();
    args::curry<1, 0, 2>(fn)(2.f, 1, nullptr)();
    args::curry<1, 2, 0>(fn)(2.f, nullptr, 1)();
    args::curry<2, 0, 1>(fn)(nullptr, 1, 2.f)();
    args::curry<2, 1, 0>(fn)(nullptr, 2.f, 1)();


    args::curry<0>(fn2)(1)(2.f, nullptr);
    args::curry<1>(fn2)(2.f)(1, nullptr);
    args::curry<2>(fn2)(nullptr)(1, 2.f);

    args::curry<0, 1>(fn2)(1, 2.f)(nullptr);
    args::curry<0, 2>(fn2)(1, nullptr)(2.f);
    args::curry<1, 0>(fn2)(2.f, 1)(nullptr);
    args::curry<1, 2>(fn2)(2.f, nullptr)(1);
    args::curry<2, 0>(fn2)(nullptr, 1)(2.f);
    args::curry<2, 1>(fn2)(nullptr, 2.f)(1);

    args::curry<0, 1, 2>(fn2)(1, 2.f, nullptr)();
    args::curry<0, 2, 1>(fn2)(1, nullptr, 2.f)();
    args::curry<1, 0, 2>(fn2)(2.f, 1, nullptr)();
    args::curry<1, 2, 0>(fn2)(2.f, nullptr, 1)();
    args::curry<2, 0, 1>(fn2)(nullptr, 1, 2.f)();
    args::curry<2, 1, 0>(fn2)(nullptr, 2.f, 1)();


	args::curry<0>(fn3)(1)(2.f, nullptr);
    args::curry<1>(fn3)(2.f)(1, nullptr);
    args::curry<2>(fn3)(nullptr)(1, 2.f);

    args::curry<0, 1>(fn3)(1, 2.f)(nullptr);
    args::curry<0, 2>(fn3)(1, nullptr)(2.f);
    args::curry<1, 0>(fn3)(2.f, 1)(nullptr);
    args::curry<1, 2>(fn3)(2.f, nullptr)(1);
    args::curry<2, 0>(fn3)(nullptr, 1)(2.f);
    args::curry<2, 1>(fn3)(nullptr, 2.f)(1);

    args::curry<0, 1, 2>(fn3)(1, 2.f, nullptr)();
    args::curry<0, 2, 1>(fn3)(1, nullptr, 2.f)();
    args::curry<1, 0, 2>(fn3)(2.f, 1, nullptr)();
    args::curry<1, 2, 0>(fn3)(2.f, nullptr, 1)();
    args::curry<2, 0, 1>(fn3)(nullptr, 1, 2.f)();
    args::curry<2, 1, 0>(fn3)(nullptr, 2.f, 1)();
}

int main()
{
    reorder();
    bind();
    curry();
}
