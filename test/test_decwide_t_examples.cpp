///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <iomanip>
#include <iostream>

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>
#include <test/test_decwide_t_examples.h>

#if defined(__clang__)
  #if defined __has_feature && (__has_feature(thread_sanitizer) || __has_feature(address_sanitizer))
  #define DECWIDE_T_TEST_OPTION_REDUCE_TEST_DEPTH
  #endif
#elif defined(__GNUC__)
  #if defined(__SANITIZE_THREAD__) || defined(__SANITIZE_ADDRESS__) || defined(WIDE_DECIMAL_HAS_COVERAGE)
  #define DECWIDE_T_TEST_OPTION_REDUCE_TEST_DEPTH
  #endif
#endif

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::test_decwide_t_examples_part1__() -> bool // NOLINT(readability-identifier-naming)
#else
auto test_decwide_t_examples_part1__() -> bool // NOLINT(readability-identifier-naming)
#endif
{
  bool result_is_ok = true;

  result_is_ok &= math::wide_decimal::example000_multiply_nines           (); std::cout << "example000_multiply_nines           : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example000a_multiply_pi_squared     (); std::cout << "example000a_multiply_pi_squared     : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example001_roots_sqrt               (); std::cout << "example001_roots_sqrt               : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example001a_roots_seventh           (); std::cout << "example001a_roots_seventh           : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example001b_roots_almost_integer    (); std::cout << "example001b_roots_almost_integer    : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example001c_roots_sqrt_limb08       (); std::cout << "example001c_roots_sqrt_limb08       : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example001d_pow2_from_list          (); std::cout << "example001d_pow2_from_list          : " << std::boolalpha << result_is_ok << std::endl;
  #if defined(DECWIDE_T_TEST_OPTION_REDUCE_TEST_DEPTH)
  #if defined(WIDE_DECIMAL_HAS_COVERAGE)
  result_is_ok &= math::wide_decimal::example002_pi                       (); std::cout << "example002_pi                       : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example002a_pi_small_limb           (); std::cout << "example002a_pi_small_limb           : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example002b_pi_100k                 (); std::cout << "example002b_pi_100k                 : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example002c_pi_quintic              (); std::cout << "example002c_pi_quintic              : " << std::boolalpha << result_is_ok << std::endl;
  #endif
  #else
  result_is_ok &= math::wide_decimal::example002_pi                       (); std::cout << "example002_pi                       : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example002a_pi_small_limb           (); std::cout << "example002a_pi_small_limb           : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example002b_pi_100k                 (); std::cout << "example002b_pi_100k                 : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example002c_pi_quintic              (); std::cout << "example002c_pi_quintic              : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example002d_pi_limb08               (); std::cout << "example002d_pi_limb08               : " << std::boolalpha << result_is_ok << std::endl;
  #endif

  return result_is_ok;
}

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::test_decwide_t_examples_part2__() -> bool // NOLINT(readability-identifier-naming)
#else
auto test_decwide_t_examples_part2__() -> bool // NOLINT(readability-identifier-naming)
#endif
{
  bool result_is_ok = true;

  result_is_ok &= math::wide_decimal::example003_zeta                     (); std::cout << "example003_zeta                     : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example004_bessel_recur             (); std::cout << "example004_bessel_recur             : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example005_polylog_series           (); std::cout << "example005_polylog_series           : " << std::boolalpha << result_is_ok << std::endl;
  #if defined(DECWIDE_T_TEST_OPTION_REDUCE_TEST_DEPTH)
  #if defined(WIDE_DECIMAL_HAS_COVERAGE)
  result_is_ok &= math::wide_decimal::example006_logarithm                (); std::cout << "example006_logarithm                : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example007_catalan_series           (); std::cout << "example007_catalan_series           : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example008_bernoulli_tgamma         (); std::cout << "example008_bernoulli_tgamma         : " << std::boolalpha << result_is_ok << std::endl;
  #endif
  #else
  result_is_ok &= math::wide_decimal::example006_logarithm                (); std::cout << "example006_logarithm                : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example007_catalan_series           (); std::cout << "example007_catalan_series           : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example008_bernoulli_tgamma         (); std::cout << "example008_bernoulli_tgamma         : " << std::boolalpha << result_is_ok << std::endl;
  #endif
  result_is_ok &= math::wide_decimal::example009_boost_math_standalone    (); std::cout << "example009_boost_math_standalone    : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example009a_boost_math_standalone   (); std::cout << "example009a_boost_math_standalone   : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example009b_boost_math_standalone   (); std::cout << "example009b_boost_math_standalone   : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example010_hypergeometric_2f1       (); std::cout << "example010_hypergeometric_2f1       : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example010a_hypergeometric_1f1      (); std::cout << "example010a_hypergeometric_1f1      : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example011_trig_trapezoid_integral  (); std::cout << "example011_trig_trapezoid_integral  : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example012_rational_floor_ceil      (); std::cout << "example012_rational_floor_ceil      : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example013_embeddable_sqrt          (); std::cout << "example013_embeddable_sqrt          : " << std::boolalpha << result_is_ok << std::endl;
  result_is_ok &= math::wide_decimal::example013a_embeddable_agm          (); std::cout << "example013a_embeddable_agm          : " << std::boolalpha << result_is_ok << std::endl;

  return result_is_ok;
}
