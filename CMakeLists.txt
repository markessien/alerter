cmake_minimum_required(VERSION 3.18)
project(TelexStart)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "res/resources.cpp" "src/*.cc")

add_executable(telexstart ${SOURCES})

target_include_directories(telexstart PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/res
)

target_link_libraries(telexstart PRIVATE
    httplib::httplib
    wx::core
    wx::base
    nlohmann_json::nlohmann_json
    gRPC::grpc++
    protobuf::libprotobuf
    # Windows-specific libraries needed by your original file
    wininet
    ws2_32
    rpcrt4
    comctl32
    gdiplus
)

if(MSVC)
    # This hides the console window for a GUI application
    set_target_properties(telexstart PROPERTIES WIN32_EXECUTABLE TRUE)

    # Use /EHsc for exception handling
    target_compile_options(telexstart PRIVATE /EHsc)
    
    # Configure settings for Debug vs. Release
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Use the Multithreaded-Debug DLL runtime
        target_compile_options(telexstart PRIVATE /MDd /Zi /Od)
        target_link_options(telexstart PRIVATE /DEBUG)
        set_target_properties(telexstart PROPERTIES OUTPUT_NAME "telexstart_d")
    else()
        # Use the Multithreaded DLL runtime for Release
        target_compile_options(telexstart PRIVATE /MD /O2)
    endif()
endif()