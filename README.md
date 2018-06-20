# args
A header only functional programming support library without type erasure.

```cpp
void foo(int, float, const char*);

// all of the examples below call foo(1, 2.f, "3") and are optimized out in release builds

auto reordered = args::reorder<2, 0, 1>(foo);
reordered("3", 1, 2.f);

auto bound = args::bind<1>(foo, 2.f);
bound(1, "hello");

auto curried = args::curry<0, 2>(foo);
auto bound2  = curried(1, "3");
bound2(2.f);
```

## quick documentation
* Both functions and arguments are stored by value. Use std::ref to avoid copies.
* Chaining of the library functions is possible.
* Type erasure is not performed.

```cpp
template<std::size_t... ArgIndices, class Fn>
constexpr /* callable object */ reorder(Fn&& function);

reorder<2, 0, 1>(void(int, float, const char*)) -> void(const char*, int, float)
```

```cpp
template<std::size_t... ArgIndices, class Fn, class... Args>
constexpr /* callable object */ bind(Fn&& function, Args&&... values_to_bind);

bind<1>(void(int, float, const char*), float) -> void(int, const char*)
```

```cpp
template<std::size_t... ArgIndices, class Fn>
constexpr /* callable object */ curry(Fn&& function);

curry<2>(void(int, float, const char*)) -> (void(int, float))(const char*)
```
