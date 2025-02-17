﻿///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef UTIL_BASELEXICAL_CAST_2020_06_28_H // NOLINT(llvm-header-guard)
  #define UTIL_BASELEXICAL_CAST_2020_06_28_H

  #include <cstddef>
  #include <cstdint>
  #include <iterator>

  namespace util {

  template<typename OutputIterator,
           const bool UpperCase,
           const std::uint_fast8_t BaseRepresentation>
  struct baselexical_cast_helper
  {
  private:
    using output_value_type = typename std::iterator_traits<OutputIterator>::value_type;

  public:
    static auto extract(output_value_type) noexcept -> output_value_type = delete;
  };

  template<typename OutputIterator,
           const bool UpperCase>
  struct baselexical_cast_helper<OutputIterator, UpperCase, 16U> // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  {
  private:
    using output_value_type = typename std::iterator_traits<OutputIterator>::value_type;

  public:
    static auto extract(output_value_type c) noexcept -> output_value_type
    {
      if(c <= static_cast<output_value_type>(9)) // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      {
        c =
          static_cast<output_value_type>
          (
            c + static_cast<output_value_type>('0')
          );
      }
      else if((c >= static_cast<output_value_type>(0xA)) && (c <= static_cast<output_value_type>(0xF))) // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      {
        c =
          static_cast<output_value_type>
          (
              static_cast<output_value_type>(UpperCase ? static_cast<output_value_type>('A') : static_cast<output_value_type>('a'))
            + static_cast<output_value_type>(c - static_cast<output_value_type>(0xA)) // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
          );
      }

      return c;
    }
  };

  template<typename OutputIterator,
           const bool UpperCase>
  struct baselexical_cast_helper<OutputIterator, UpperCase, 10U> // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  {
  private:
    using output_value_type = typename std::iterator_traits<OutputIterator>::value_type;

  public:
    static auto extract(output_value_type c) noexcept -> output_value_type
    {
      if(c <= static_cast<output_value_type>(9)) // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      {
        c =
          static_cast<output_value_type>
          (
            c + static_cast<output_value_type>('0')
          );
      }

      return c;
    }
  };

  template<typename UnsignedIntegerType,
           typename OutputIterator,
           const std::uint_fast8_t BaseRepresentation = 10U, // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
           const bool UpperCase = true>
  auto baselexical_cast(const UnsignedIntegerType& u, OutputIterator out_first) -> OutputIterator
  {
    using unsigned_integer_type = UnsignedIntegerType;
    using output_value_type     = typename std::iterator_traits<OutputIterator>::value_type;

    unsigned_integer_type x(u);

    auto index = static_cast<std::ptrdiff_t>(0);

    do
    {
      for(auto j = index; j >= static_cast<std::ptrdiff_t>(0); --j)
      {
        *(out_first + static_cast<std::ptrdiff_t>(j + 1)) = *(out_first + j);
      }

      ++index;

      auto c = static_cast<output_value_type>(x % static_cast<unsigned_integer_type>(BaseRepresentation));

      x = static_cast<unsigned_integer_type>(x / static_cast<unsigned_integer_type>(BaseRepresentation));

      *out_first = baselexical_cast_helper<OutputIterator, UpperCase, BaseRepresentation>::extract(c);
    }
    while(x != static_cast<unsigned_integer_type>(0U));

    return static_cast<OutputIterator>(out_first + index);
  }

  } // namespace util

#endif // UTIL_BASELEXICAL_CAST_2020_06_28_H
