cmake_minimum_required(VERSION 3.2)

aux_source_directory(./threadpool/ THREADPOOL_FILES)
aux_source_directory(./timer/ TIMER_FILES)

add_executable(threadpool ${THREADPOOL_FILES})
add_executable(timer ${TIMER_FILES})

target_link_libraries(threadpool core)
target_link_libraries(timer core)
