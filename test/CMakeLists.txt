option(CPP_STARTER_USE_SAMPLE_TESTS "Enable sample tests" OFF)

# Automatically enable catch2 to generate ctest targets
if(USE_VCPKG)
	message(STATUS "using vcpkg")
	find_package(Catch2 CONFIG REQUIRED)
	include(Catch)
else()
	message(STATUS "using conan")
	if(CONAN_CATCH2_ROOT_DEBUG)
	  include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
	else()
	  include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
	endif()
endif()


add_library(catch_main STATIC catch_main.cpp)
if(USE_VCPKG)
	target_link_libraries(catch_main 
		PUBLIC 
			Catch2::Catch2
	)
else()
	target_link_libraries(catch_main PUBLIC CONAN_PKG::catch2)
endif()
target_link_libraries(catch_main PRIVATE project_options)
target_compile_definitions(catch_main 
	PUBLIC 
		CATCH_CONFIG_ENABLE_BENCHMARKING
)

find_package(OpenSSL REQUIRED)


add_executable(aoc_tests
	"aoc_2020_19_1_tests.cpp"
	"test_constants.h"
	"enum_flag.cpp" 
	"aoc2015/day1.cpp" 
	"aoc2015/day2.cpp"
	"aoc2015/day3.cpp"
	"aoc2015/day4.cpp"
	"aoc2015/day5.cpp" 
	"aoc2015/day6.cpp"
	"aoc2020/y2020d20.cpp" 
	"aoc2015/day7.cpp" 
	"aoc2015/day8.cpp")
target_link_libraries(aoc_tests
	PRIVATE
		aoc2020
		project_warnings
		project_options
		catch_main
		OpenSSL::Crypto
)

catch_discover_tests(
	  aoc_tests
	  TEST_PREFIX
	  "unittests."
	  REPORTER
	  xml
	  OUTPUT_DIR
	  .
	  OUTPUT_PREFIX
	  "unittests."
	  OUTPUT_SUFFIX
	  .xml
)

if(CPP_STARTER_USE_SAMPLE_TESTS)
	add_executable(tests 
		"sample/tests.cpp"
	)
	target_link_libraries(tests 
		PRIVATE 
			project_warnings 
			project_options 
			catch_main
	)

	# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
	# to whatever you want, or use different for different binaries
	catch_discover_tests(
	  tests
	  TEST_PREFIX
	  "unittests."
	  REPORTER
	  xml
	  OUTPUT_DIR
	  .
	  OUTPUT_PREFIX
	  "unittests."
	  OUTPUT_SUFFIX
	  .xml)

	# Add a file containing a set of constexpr tests
	add_executable(constexpr_tests 
		"sample/constexpr_tests.cpp"
	)
	target_link_libraries(constexpr_tests 
		PRIVATE 
			project_options 
			project_warnings 
			catch_main
	)

	catch_discover_tests(
	  constexpr_tests
	  TEST_PREFIX
	  "constexpr."
	  REPORTER
	  xml
	  OUTPUT_DIR
	  .
	  OUTPUT_PREFIX
	  "constexpr."
	  OUTPUT_SUFFIX
	  .xml)

	# Disable the constexpr portion of the test, and build again this allows us to have an executable that we can debug when
	# things go wrong with the constexpr testing
	add_executable(relaxed_constexpr_tests 
		sample/constexpr_tests.cpp
	)
	target_link_libraries(relaxed_constexpr_tests 
		PRIVATE 
			project_options 
			project_warnings 
			catch_main
	)
	target_compile_definitions(relaxed_constexpr_tests 
		PRIVATE 
			-DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE
	)

	catch_discover_tests(
	  relaxed_constexpr_tests
	  TEST_PREFIX
	  "relaxed_constexpr."
	  REPORTER
	  xml
	  OUTPUT_DIR
	  .
	  OUTPUT_PREFIX
	  "relaxed_constexpr."
	  OUTPUT_SUFFIX
	  .xml)
endif()