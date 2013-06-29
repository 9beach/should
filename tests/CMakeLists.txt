INCLUDE_DIRECTORIES(${SHOULD_SOURCE_DIR})
LINK_DIRECTORIES(${SHOULD_BINARY_DIR}/should)

ADD_EXECUTABLE(test_simple test_simple.c)
ADD_EXECUTABLE(test_fixture test_fixture.c)

TARGET_LINK_LIBRARIES(test_simple should)
TARGET_LINK_LIBRARIES(test_fixture should)

ADD_TEST(test_simple test_simple)
ADD_TEST(test_fixture test_fixture)
