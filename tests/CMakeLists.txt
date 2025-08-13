option(DAW_USE_SANITIZERS "Enable Sanitizers" ON)

add_library(daw_thunk_test_lib INTERFACE)
target_link_libraries(daw_thunk_test_lib INTERFACE daw::daw-thunk)

if(MSVC)
  target_compile_options(daw_thunk_test_lib
    INTERFACE
      -DDEBUG
      /permissive-
      /EHsc
      /arch:AVX2
      -DWINVER=0x0A00
      -D_WIN32_WINNT=0x0A00
  )
  if(DAW_USE_SANITIZERS)
    target_compile_options(daw_thunk_test_lib
      INTERFACE
        /fsanitize=address
        -fno-omit-frame-pointer
    )
  endif()
else(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(daw_thunk_test_lib INTERFACE -march=native)
  if( DAW_USE_SANITIZERS )
    target_compile_options(daw_thunk_test_lib INTERFACE -fsanitize=address)
    target_link_options(daw_thunk_test_lib INTERFACE -fsanitize=address)
  endif()
endif()

add_executable(daw_thunk src/daw_thunk_test.cpp)
target_link_libraries(daw_thunk PRIVATE daw_thunk_test_lib)
add_test(NAME daw_thunk_test COMMAND daw_thunk)

add_executable(signal src/signal_test.cpp)
target_link_libraries(signal PRIVATE daw_thunk_test_lib)

add_executable(stateful_currying src/stateful_currying_test.cpp)
target_link_libraries(stateful_currying PRIVATE daw_thunk_test_lib)
add_test(NAME stateful_currying_test COMMAND stateful_currying)
