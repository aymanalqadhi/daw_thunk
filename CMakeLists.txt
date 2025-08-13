cmake_minimum_required(VERSION 3.24)

project(daw-thunk
  VERSION "0.1.0"
  DESCRIPTION "Cross platform thunking abstraction"
  HOMEPAGE_URL "https://github.com/beached/thunk"
  LANGUAGES C CXX
)

# ##############################################################################
# Target

add_library(${PROJECT_NAME} INTERFACE)
add_library(daw::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/daw/daw_thunk.h
      include/daw/thunk/daw_thunk_posix.h
      include/daw/thunk/daw_thunk_win.h
      include/daw/thunk/impl/thunk_arch.h
      include/daw/thunk/impl/thunk_impl.h
      include/daw/thunk/impl/thunk_win_x64.h
      include/daw/thunk/impl/thunk_sysv_x86.h
      include/daw/thunk/impl/thunk_sysv_x64.h
      include/daw/thunk/impl/thunk_win_x86.h
      include/daw/thunk/impl/thunk_aapcs64.h
      include/daw/thunk/impl/daw_function_traits.h
      include/daw/daw_erased_callable.h
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

# ##############################################################################
# Testing

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if(DAW_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

# ##############################################################################
# Installation

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# install targets
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_Targets

  FILE_SET HEADERS
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT DawDevelopment
  RUNTIME
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT DawRuntime
  LIBRARY
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT DawRuntime
    NAMELINK_COMPONENT PjDevelopment
  ARCHIVE
    DESTINATION  "${CMAKE_INSTALL_BINDIR}"
    COMPONENT DawDevelopment
)

# install exports
install(
  EXPORT      ${PROJECT_NAME}_Targets
  FILE        ${PROJECT_NAME}Targets.cmake
  NAMESPACE   daw::
  DESTINATION lib/cmake/${PROJECT_NAME}
)

# ##############################################################################
# CMAKe Package config

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"

  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"

  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"

  DESTINATION lib/cmake/${PROJECT_NAME}
  COMPONENT DawDevelopment
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/daw DESTINATION include)