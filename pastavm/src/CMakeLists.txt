ADD_LIBRARY (vmcore vm.h vm.c errlog.h heap.h heap.c)
ADD_LIBRARY (codegen rawcode.c rawcode.h heap.h heap.c)
TARGET_LINK_LIBRARIES(vmcore cjson)

ADD_EXECUTABLE(pastavm main.c)
TARGET_LINK_LIBRARIES(pastavm vmcore cjson)
