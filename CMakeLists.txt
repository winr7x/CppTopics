cmake_minimum_required (VERSION 3.13.3) # 3.13.3 was released Jan 14, 2019

set(PROJECT_NAME MultithreadingIn20Minutes)

project (${PROJECT_NAME})

if(CMAKE_GENERATOR MATCHES "^Visual Studio.+")
  # message("##### CMAKE_GENERATOR is Visual Studio")
  add_compile_options(
    /Zi                # (Debug Information Format) 
                       # /Zi option produces a separate PDB file that contains all the symbolic debugging information for use with the debugger.
                       # The debugging information isn't included in the object files or executable, which makes them much smaller.
    /std:c++latest     # (Specify Language Standard Version)
                       # /std:c++14
                       # /std:c++17
                       # /std:c++20
                       # /std:c++latest
                       # /std:c11
                       # /std:c17
    /EHsc              # (Exception handling model)
                       # Arguments specify whether to apply catch(...) syntax to both structured and standard C++ exceptions,
                       # whether extern "C" code is assumed to throw exceptions, and whether to optimize away certain noexcept checks.
                       # s - Enables standard C++ stack unwinding. Catches only standard C++ exceptions when you use catch(...) syntax. 
                       # c - When used with /EHs, the compiler assumes that functions declared as extern "C" never throw a C++ exception.
                       # It has no effect when used with /EHa (that is, /EHca is equivalent to /EHa). /EHc is ignored if /EHs or /EHa aren't specified.
    /W4                # /W4 displays level 1, level 2, and level 3 warnings, and all level 4 (informational) warnings that aren't off by default.
                       # We recommend that you use this option to provide lint-like warnings.
                       # For a new project, it may be best to use /W4 in all compilations.
                       # This option helps ensure the fewest possible hard-to-find code defects.
  )
elseif(CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
  # message("##### CMAKE_GENERATOR is MinGW")
  add_compile_options(
    -g                 # generates debug information to be used by GDB debugger
                       # -g0 - no debug information
                       # -g1 - minimal debug information
                       # -g  - default debug information
                       # -g3 - maximal debug information
    -std=c++20         # Determine the language standard.
                       # c++03 or gnu++03
                       # c++11 or gnu++11
                       # c++14 or gnu++14
                       # c++17 or gnu++17
                       # c++20 or gnu++20
    -Wall              # This enables all the warnings about constructions that some users consider questionable,
                       # and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros.
                       # This also enables some language-specific warnings
    -Wextra            # This enables some extra warning flags that are not enabled by -Wall.
    -pedantic          # Issue all the warnings demanded by strict ISO C and ISO C++
  )
else()
  message(FATAL_ERROR "Unexpected cmake generator ${CMAKE_GENERATOR}")
endif()

add_executable(1_main                                         MultithreadingIn20Minutes_1/1_main.cc)
add_executable(2_vector_of_threads_main                       MultithreadingIn20Minutes_1/2_vector_of_threads_main.cc)
add_executable(3_functors_main                                MultithreadingIn20Minutes_1/3_functors_main.cc)
add_executable(4_lambda_main                                  MultithreadingIn20Minutes_1/4_lambda_main.cc)
add_executable(5_async_main                                   MultithreadingIn20Minutes_1/5_async_main.cc)
add_executable(6_async_functor_main                           MultithreadingIn20Minutes_1/6_async_functor_main.cc)
add_executable(7_async_lambda_main                            MultithreadingIn20Minutes_1/7_async_lambda_main.cc)

add_executable(1_race_condition_main                          MultithreadingIn20Minutes_2/1_race_condition_main.cc)
add_executable(2_no_lock_main                                 MultithreadingIn20Minutes_2/2_no_lock_main.cc)
add_executable(3_lock_unlock_main                             MultithreadingIn20Minutes_2/3_lock_unlock_main.cc)
add_executable(4_lock_guard_main                              MultithreadingIn20Minutes_2/4_lock_guard_main.cc)
add_executable(5_unique_lock_main                             MultithreadingIn20Minutes_2/5_unique_lock_main.cc)
add_executable(6_shared_lock_main                             MultithreadingIn20Minutes_2/6_shared_lock_main.cc)
add_executable(7_dead_lock_main                               MultithreadingIn20Minutes_2/7_dead_lock_main.cc)
add_executable(8_multiple_locks_before_cpp17_main             MultithreadingIn20Minutes_2/8_multiple_locks_before_cpp17_main.cc)

add_executable(WhyStdRef                    WhyStdRef/main.cpp)
