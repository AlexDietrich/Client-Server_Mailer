cmake_minimum_required(VERSION 3.8)
project(V_SYS_TWMailer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_CLIENT MyClient/myclient.cpp)
add_executable(V_SYS_TWMailer_CLIENT ${SOURCE_FILES_CLIENT})

set(SOURCE_FILES_SERVER MyServer/myserver.cpp MyServer/ServerOperation.cpp MyServer/ServerOperation.h MyServer/DeleteMessage.cpp MyServer/DeleteMessage.h MyServer/ReadMessage.cpp MyServer/ReadMessage.h MyServer/ListMessage.cpp MyServer/ListMessage.h MyServer/SendMessage.cpp MyServer/SendMessage.h)
add_executable(V_SYS_TWMailer_SERVER ${SOURCE_FILES_SERVER})
