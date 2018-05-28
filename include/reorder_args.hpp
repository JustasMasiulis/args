/*
 * Copyright 2018 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JM_REORDER_ARGUMENTS_HPP
#define JM_REORDER_ARGUMENTS_HPP

#include <tuple>
#include <utility>
#include <type_traits>

namespace args {

    namespace detail {

        /// \brief A small structure to get an argument from a pack by index
        /// \tparam TargetIndex The index of wanted argument
        /// @{
        template<std::size_t TargetIndex, std::size_t CurrentIndex = 0>
        struct nth_arg {
            /// \brief Returns the wanted argument
            template<class Discard, class... Rest>
            inline constexpr static decltype(auto) get(Discard&&, Rest&&... rest)
            {
                return nth_arg<TargetIndex, CurrentIndex + 1>::get(
                    std::forward<Rest>(rest)...);
            }
        };

        template<std::size_t TargetIndex>
        struct nth_arg<TargetIndex, TargetIndex> {
            template<class Target, class... Rest>
            inline constexpr static decltype(auto) get(Target&& target, Rest&&...)
            {
                return std::forward<Target>(target);
            }
        };
        /// @}

        template<std::size_t ArgIndex,
                 std::size_t Result,
                 std::size_t... ReorderedArgIndices>
        struct reordered_arg_index;


        template<std::size_t ArgIndex,
                 std::size_t Result,
                 std::size_t Next,
                 std::size_t... Rest>
        struct reordered_arg_index<ArgIndex, Result, Next, Rest...>
            : reordered_arg_index<ArgIndex, Result + 1, Rest...> {
        };

        template<std::size_t ArgIndex, std::size_t Result, std::size_t... Rest>
        struct reordered_arg_index<ArgIndex, Result, ArgIndex, Rest...> {
            constexpr static std::size_t value = Result;
        };


        template<class Fn, std::size_t... ArgIndices>
        class reorderer {
            Fn _fn;

            template<std::size_t... Idxs, class... Args>
            inline decltype(auto) _call(std::index_sequence<Idxs...>, Args&&... args)
            {
                return _fn(
                    nth_arg<reordered_arg_index<Idxs, 0, ArgIndices...>::value>::get(
                        std::forward<Args>(args)...)...);
            }

        public:
            template<class Function>
            inline reorderer(Function&& fn) : _fn(std::forward<Function>(fn))
            {}

            template<class... Args>
            inline decltype(auto) operator()(Args&&... args)
            {
                return _call(std::make_index_sequence<sizeof...(ArgIndices)>(),
                             std::forward<Args>(args)...);
            }
        };


        template<std::size_t ToFind, std::size_t Index, std::size_t... Rest>
        struct bound_arg_index;

        template<std::size_t ToFind,
                 std::size_t Index,
                 std::size_t Next,
                 std::size_t... Rest>
        struct bound_arg_index<ToFind, Index, Next, Rest...>
            : bound_arg_index<ToFind, Index + 1, Rest...> {
        };

        template<std::size_t ToFind, std::size_t Index, std::size_t... Rest>
        struct bound_arg_index<ToFind, Index, ToFind, Rest...> {
            constexpr static std::size_t value = Index;
        };

        template<std::size_t ToFind, std::size_t Index>
        struct bound_arg_index<ToFind, Index> {
            constexpr static std::size_t value = -1;
        };


        template<std::size_t ArgIndex,
                 std::size_t Result,
                 std::size_t Next,
                 std::size_t... Rest>
        struct unbound_arg_index
            : unbound_arg_index<ArgIndex,
                                (ArgIndex > Next ? Result - 1 : Result),
                                Rest...> {
        };

        template<std::size_t ArgIndex, std::size_t Result, std::size_t Next>
        struct unbound_arg_index<ArgIndex, Result, Next> {
            constexpr static std::size_t value = ArgIndex > Next ? Result - 1 : Result;
        };


        template<std::size_t BoundArgIndex, std::size_t UnboundArgIndex>
        struct arg_for_bound_func {
            template<class Bound, class... Unbound>
            inline constexpr static decltype(auto) get(Bound&& bound, Unbound&&...)
            {
                return std::get<BoundArgIndex>(bound);
            }
        };

        template<std::size_t UnboundArgIndex>
        struct arg_for_bound_func<static_cast<std::size_t>(-1), UnboundArgIndex> {
            template<class Bound, class... Unbound>
            inline constexpr static decltype(auto) get(Bound&&, Unbound&&... unbound)
            {
                return nth_arg<UnboundArgIndex>::get(std::forward<Unbound>(unbound)...);
            }
        };


        template<class Fn, class TupleType, std::size_t... ArgumentIndices>
        class binder {
            Fn        _fn;
            TupleType _bound_args;

            template<class... Args, std::size_t... Idxs>
            inline decltype(auto) _call(std::index_sequence<Idxs...>, Args&&... args)
            {
                return _fn(arg_for_bound_func<
                           bound_arg_index<Idxs, 0, ArgumentIndices...>::value,
                           unbound_arg_index<Idxs, Idxs, ArgumentIndices...>::value>::
                               get(_bound_args, std::forward<Args>(args)...)...);
            }

        public:
            template<class Function, class... Args>
            inline constexpr binder(Function&& fn, Args&&... args)
                : _fn(std::forward<Function>(fn))
                , _bound_args(std::forward<Args>(args)...)
            {}

            template<class... Args>
            inline decltype(auto) operator()(Args&&... args)
            {
                return _call(std::make_index_sequence<sizeof...(args) +
                                                      sizeof...(ArgumentIndices)>(),
                             std::forward<Args>(args)...);
            }
        };

    } // namespace detail


    /// \brief Reorders function arguments.
    /// \tparam ArgIndices New order of parameters by their index.
    /// \param function The function whose arguments will be reordered.
    /// \returns Returns an object which will take the reordered arguments and pass them
    ///          down to the original function.
    /// \note Function object is saved by value. Use std::ref to avoid copies.
    /// \code{.cpp}
    /// void foo(int, float, void*);
    /// // produces a callable whose signature would be void(void*, float,int).
    /// auto reordered = args::reorder<3, 2, 1>(foo);
    /// // calls foo(1, 2.f, nullptr);
    /// reordered(nullptr, 2.f, 1);
    /// \endcode
    template<std::size_t... ArgIndices, class Fn>
    inline constexpr detail::reorderer<std::decay_t<Fn>, ArgIndices...>
    reorder(Fn&& function)
    {
        return { std::forward<Fn>(function) };
    }

    /// \brief Binds / partially applies function arguments.
    /// \tparam ArgIndices Indices of arguments whose values will be bound.
    /// \param function The function whose arguments will be bound.
    /// \param values The values that will be bound to function arguments at given
    ///               indices.
    /// \returns Returns an object which will take the non bound arguments and
    ///          pass them, together with the bound arguments down to the original
    ///          function.
    /// \note Function object and bound arguments are saved by value. Use std::ref to
    ///       avoid copies.
    /// \code{.cpp}
    /// void foo(int, float, void*);
    /// // produces a callable whose signature would be void(float).
    /// auto bound = args::bind<2, 0>(foo, nullptr, 1);
    /// // calls foo(1, 2.f, nullptr);
    /// bound(2.f);
    /// \endcode
    template<std::size_t... ArgIndices, class Fn, class... Ts>
    inline constexpr detail::binder<std::decay_t<Fn>, std::tuple<Ts...>, ArgIndices...>
    bind(Fn&& function, Ts&&... values)
    {
        static_assert(
            sizeof...(ArgIndices) == sizeof...(Ts),
            "you must provide the same number of values to bind and argument indices");

        return { std::forward<Fn>(function), std::forward<Ts>(values)... };
    }

} // namespace args

#endif