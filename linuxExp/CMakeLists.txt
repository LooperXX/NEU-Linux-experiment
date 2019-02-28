cmake_minimum_required(VERSION 3.12)
project(linuxExp)

set(CMAKE_CXX_STANDARD 11)

add_executable(mywebserver mywebserver.cpp)
add_executable(myls_L myls_L.cpp)
add_executable(myls_R myls_R.cpp)
add_executable(mycp mycp.cpp)
add_executable(mymice mymice.cpp)
add_executable(mysignal mysignal.cpp)
add_executable(mysignal2 mysignal2.cpp)
add_executable(myserver2 myserver2.cpp)