# CMake generated Testfile for 
# Source directory: /home/dongcai/Documents/pasta/pastavm
# Build directory: /home/dongcai/Documents/pasta/pastavm/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(MyTest "/home/dongcai/Documents/pasta/pastavm/build/test/Test")
subdirs("src")
subdirs("lib")
subdirs("test")
