﻿///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2017 - 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PARALLEL_FOR_2017_12_18_H // NOLINT(llvm-header-guard)
  #define PARALLEL_FOR_2017_12_18_H

  #include <algorithm>
  #include <thread>
  #include <vector>

  namespace my_concurrency
  {
    template<typename index_type,
             typename callable_function_type>
    void parallel_for(index_type             start,
                      index_type             end,
                      callable_function_type parallel_function)
    {
      // Estimate the number of threads available.
      static const auto number_of_threads_hint = static_cast<unsigned>(std::thread::hardware_concurrency());

      static const auto number_of_threads = static_cast<unsigned>(((number_of_threads_hint == 0U) ? 4U : number_of_threads_hint)); // NOLINT(altera-id-dependent-backward-branch)

      // Set the size of a slice for the range functions.
      const auto n =
        static_cast<index_type>
        (
          static_cast<index_type>(end - start) + static_cast<index_type>(1)
        );

      const auto slice =
        (std::max)
        (
          static_cast<index_type>(std::round(static_cast<double>(n) / static_cast<double>(number_of_threads))),
          static_cast<index_type>(1)
        );

      // Inner loop.
      const auto launch_range =
        [&parallel_function](index_type index_lo, index_type index_hi)
        {
          for(index_type i = index_lo; i < index_hi; ++i) // NOLINT(altera-id-dependent-backward-branch)
          {
            parallel_function(i);
          }
        };

      // Create the thread pool and launch the jobs.
      std::vector<std::thread> pool;

      pool.reserve(number_of_threads);

      auto i1 = start;
      auto i2 = (std::min)(static_cast<index_type>(start + slice), end);

      for(auto i = static_cast<index_type>(0U); ((static_cast<index_type>(i + 1) < static_cast<index_type>(number_of_threads)) && (i1 < end)); ++i) // NOLINT(altera-id-dependent-backward-branch)
      {
        pool.emplace_back(launch_range, i1, i2);

        i1 = i2;

        i2 = (std::min)(static_cast<index_type>(i2 + slice), end);
      }

      if(i1 < end)
      {
        pool.emplace_back(launch_range, i1, end);
      }

      // Wait for the jobs to finish.
      for(std::thread& thread_in_pool : pool)
      {
        if(thread_in_pool.joinable())
        {
          thread_in_pool.join();
        }
      }
    }

    // Provide a serial version for easy comparison.
    template<typename index_type,
             typename callable_function_type>
    void sequential_for(index_type             start,
                        index_type             end,
                        callable_function_type sequential_function)
    {
      for(index_type i = start; i < end; ++i)
      {
        sequential_function(i);
      }
    }
  } // namespace my_concurrency

#endif // PARALLEL_FOR_2017_12_18_H
