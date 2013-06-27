ENABLE_TESTING()

INCLUDE_DIRECTORIES(..)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../should)
LINK_LIBRARIES(should)

ADD_EXECUTABLE(test_simple test_simple.c)
ADD_EXECUTABLE(test_fixture test_fixture.c)
ADD_TEST(simple test_simple)
ADD_TEST(fixture test_fixture)
