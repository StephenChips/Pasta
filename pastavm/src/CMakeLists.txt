ADD_LIBRARY (vmcore vm.h vm.c errlog.h)

ADD_EXECUTABLE(pastavm main.c)
TARGET_LINK_LIBRARIES(pastavm vmcore cjson)
