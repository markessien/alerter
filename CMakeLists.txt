cmake_minimum_required(VERSION 3.10)
project(TelexStart)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(wxWidgets_ROOT_DIR ${CMAKE_SOURCE_DIR}/third_party/wxWidgets)
find_package(wxWidgets COMPONENTS core base adv html xml xrc REQUIRED)

if(WIN32)
    set(RC_FILE ${CMAKE_SOURCE_DIR}/res/icon.rc)
    set(RC_COMPILED ${CMAKE_BINARY_DIR}/icon.res)
    add_custom_command(
        OUTPUT ${RC_COMPILED}
        COMMAND rc.exe /fo ${RC_COMPILED} ${RC_FILE}
        DEPENDS ${RC_FILE}
    )
endif()

set(XRC_FILE ${CMAKE_SOURCE_DIR}/res/resources.xrc)
set(XRC_CPP ${CMAKE_SOURCE_DIR}/res/resources.cpp)
add_custom_command(
    OUTPUT ${XRC_CPP}
    COMMAND ${wxWidgets_ROOT_DIR}/utils/wxrc/vc_x64_mswu/wxrc.exe ${XRC_FILE} --cpp-code --output=${XRC_CPP}
    DEPENDS ${XRC_FILE}
)

file(GLOB_RECURSE SOURCES "src/*.cpp" "res/resources.cpp")

add_executable(telexstart ${SOURCES} ${RC_COMPILED} ${XRC_CPP})

target_include_directories(telexstart PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party
    ${CMAKE_SOURCE_DIR}/res
    ${wxWidgets_INCLUDE_DIRS}
)

target_link_libraries(telexstart PRIVATE
    ${wxWidgets_LIBRARIES}
    wininet
    winmm
    ws2_32
    comctl32
    rpcrt4
)

set_target_properties(telexstart PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

if(MSVC)
    target_compile_definitions(telexstart PRIVATE WXUSING_LIBS __WXMSW__)
    target_compile_options(telexstart PRIVATE /EHsc)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(telexstart PRIVATE WXDEBUG)
        target_compile_options(telexstart PRIVATE /MTd /Zi /Od)
        target_link_options(telexstart PRIVATE /DEBUG)
        set_target_properties(telexstart PROPERTIES OUTPUT_NAME "telexstart_d")
    else()
        target_compile_options(telexstart PRIVATE /MT /O2)
    endif()
endif()

install(TARGETS telexstart
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)