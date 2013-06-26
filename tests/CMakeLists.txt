ENABLE_TESTING()

INCLUDE_DIRECTORIES(..)
LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
LINK_LIBRARIES(should)

ADD_EXECUTABLE(test_simple test_simple.c)
ADD_EXECUTABLE(test_fixture test_fixture.c)
ADD_TEST(test_simple test_simple)
ADD_TEST(test_fixture test_fixture)
