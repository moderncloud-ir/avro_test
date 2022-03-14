cmake_minimum_required(VERSION 3.5)

project(avro_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#for boost.log
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)


find_package (Threads REQUIRED)
find_package(cpp-jwt REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

# ______________________Boost__________________________
find_package(Boost 1.73 REQUIRED COMPONENTS
  date_time
  system
  filesystem
  iostreams
  iostreams
  thread
  serialization
  program_options
  log
  )


add_executable(avro_test main.cpp)
file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/cpx_array.json
  ${CMAKE_CURRENT_SOURCE_DIR}/cpx_doubles.json
  ${CMAKE_CURRENT_SOURCE_DIR}/cpx_strings.json
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME}
  Boost::date_time
  Boost::system
  Boost::filesystem
  Boost::iostreams
  Boost::thread
  Boost::log
  #    Boost::mpi
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_SYSTEM_LIBRARY}
  ${MPI_CXX_LIBRARIES}
  ${Boost_LIBRARIES}

  ${CMAKE_THREAD_LIBS_INIT}
  ${LIBBSONCXX_LIBRARIES}
  ${LIBMONGOCXX_LIBRARIES}

  pthread
  # stl filesystem
  stdc++fs

  avro
  avrocpp
  avrocpp_s
  cpp-jwt::cpp-jwt

  )
