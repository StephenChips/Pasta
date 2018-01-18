INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/test)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/check)

ADD_EXECUTABLE(Test test.c)
ADD_EXECUTABLE(TestIns test_ins.c)
ADD_EXECUTABLE(TestRawcodeGen test_rawcode_gen.c testhelper.c)

TARGET_LINK_LIBRARIES(Test cjson codegen vmcore check)
TARGET_LINK_LIBRARIES(TestIns cjson codegen vmcore check)
TARGET_LINK_LIBRARIES(TestRawcodeGen codegen check)
