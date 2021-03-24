# Generic test that uses libs
add_executable(intro 
    "into/main.cpp")
target_link_libraries(
  intro
  PRIVATE project_options
          project_warnings
)
if(USE_VCPKG)
    find_package(spdlog CONFIG REQUIRED)
    find_package(docopt CONFIG REQUIRED)
    target_link_libraries(intro
        PRIVATE
            spdlog::spdlog
            docopt
            fmt::fmt
    )
else()
    target_link_libraries(
        intro
        PRIVATE
            CONAN_PKG::docopt.cpp
            CONAN_PKG::fmt
            CONAN_PKG::spdlog
    )
endif()