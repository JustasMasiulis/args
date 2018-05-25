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

#include <utility>
#include <type_traits>

namespace jm {

    namespace detail {

        template<std::size_t TargetIndex, std::size_t CurrentIndex = 0>
        struct nth_arg {
            template<class Discard, class... Rest>
            static decltype(auto) get(Discard&&, Rest&&... rest)
            {
                return nth_arg<TargetIndex, CurrentIndex + 1>::get(
                    std::forward<Rest>(rest)...);
            }
        };

        template<std::size_t TargetIndex>
        struct nth_arg<TargetIndex, TargetIndex> {
            template<class Target, class... Rest>
            static decltype(auto) get(Target&& target, Rest&&...)
            {
                return std::forward<Target>(target);
            }
        };

        template<class Fn, std::size_t... ArgumentIndices>
        struct argument_reorderer {
            Fn _fn;

        public:
            template<class Function>
            argument_reorderer(Function&& fn) : _fn(std::forward<Function>(fn))
            {}

            template<class... Args>
            decltype(auto) operator()(Args&&... args)
            {
                return _fn(nth_arg<ArgumentIndices>::get(std::forward<Args>(args)...)...);
            }
        };

    } // namespace detail


    template<std::size_t... ArgumentIndices, class Fn>
    inline detail::argument_reorderer<std::decay_t<Fn>, ArgumentIndices...>
    reorder_args(Fn&& function)
    {
        return { std::forward<Fn>(function) };
    }

} // namespace jm

#endif