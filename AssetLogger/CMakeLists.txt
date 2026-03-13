cmake_minimum_required(VERSION 3.15)
project(AssetLogger VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── wxWidgets ────────────────────────────────────────────────────────────────
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# ── Sources ──────────────────────────────────────────────────────────────────
set(SOURCES
    AssetLoggerApp.cpp
    AssetLoggerFrame.cpp
)

set(HEADERS
    AssetLoggerApp.h
    AssetLoggerFrame.h
)

# ── Executable ───────────────────────────────────────────────────────────────
if(WIN32)
    add_executable(AssetLogger WIN32 ${SOURCES} ${HEADERS})
else()
    add_executable(AssetLogger ${SOURCES} ${HEADERS})
endif()

target_link_libraries(AssetLogger PRIVATE ${wxWidgets_LIBRARIES})

# On Windows, suppress the console window in release mode
if(MSVC)
    set_target_properties(AssetLogger PROPERTIES
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS"
    )
endif()
