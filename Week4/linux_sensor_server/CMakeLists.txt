cmake_minimum_required(VERSION 3.16)

project(linux_sensor_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(linux_sensor_server
	src/main.cpp
	src/SensorReader.cpp
	src/DataProcessor.cpp
	src/TcpServer.cpp
	src/Logger.cpp
)

add_executable(linux_sensor_client
	Client/TCP_Client.cpp
)

target_include_directories(linux_sensor_server
	PRIVATE
		${PROJECT_SOURCE_DIR}/include
)

add_custom_target(run_server
	COMMAND $<TARGET_FILE:linux_sensor_server>
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	USES_TERMINAL
)

add_custom_target(run_client
	COMMAND $<TARGET_FILE:linux_sensor_client>
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	USES_TERMINAL
)

add_custom_target(run_all
	COMMAND sh -c "$<TARGET_FILE:linux_sensor_server> & server_pid=$$!; trap 'kill $$server_pid 2>/dev/null' EXIT INT TERM; sleep 1; $<TARGET_FILE:linux_sensor_client>"
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	USES_TERMINAL
)

target_link_libraries(linux_sensor_server
	PRIVATE
		Threads::Threads
)
