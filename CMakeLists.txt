# ---- project setup -------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.5)
project(qintcomp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.6.0\\msvc2019_64")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ---- QT related setup ----------------------------------------------------------------------------
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Test Network )

qt_standard_project_setup()

include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${Qt6Core_INCLUDE_DIRS})
# ---- QT related setup ----------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# ---- sources -------------------------------------------------------------------------------------
set(PROJECT_SOURCES
        ui/editor.cpp
        ui/editor.ui
        core/IntComputer.h
)

# ---- executable ----------------------------------------------------------------------------------

add_executable(qintcomp
        main.cpp
        ${PROJECT_SOURCES}
)

target_link_libraries(
        qintcomp
        Qt6::Core
        Qt6::Widgets
)

#qt_finalize_executable(qintcomp)
# ---- testing -------------------------------------------------------------------------------------
enable_testing()

add_executable(intcomp_test
        tests/test_intcomp.cpp
)

include(GoogleTest)
gtest_discover_tests(intcomp_test)



if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

# ---- the following is qt creator generrated code, do we need that? -------------------------------
# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#         qt_add_executable(qintcomp
#                 MANUAL_FINALIZATION
#                 main.cpp
#                 ${PROJECT_SOURCES}
#
#         )
#
#         install(TARGETS qintcomp
#             BUNDLE DESTINATION .
#             LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#             RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#         )
# # Define target properties for Android with Qt 6 as:
# #    set_property(TARGET qintcomp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
# else()
#     if(ANDROID)
#         add_library(qintcomp SHARED
#             main.cpp
#             ${PROJECT_SOURCES}
#         )
# # Define properties for Android with Qt 5 after find_package() calls as:
# #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#     else()
#         add_executable(qintcomp
#             main.cpp
#             ${PROJECT_SOURCES}
#         )
#     endif()
# endif()
#
# target_link_libraries(qintcomp PRIVATE
#     Qt${QT_VERSION_MAJOR}::Widgets
#     Qt${QT_VERSION_MAJOR}::Network
#     Qt${QT_VERSION_MAJOR}::PrintSupport
# )
#
#
# # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# # If you are developing for iOS or macOS you should consider setting an
# # explicit, fixed bundle identifier manually though.
# if(${QT_VERSION} VERSION_LESS 6.1.0)
#   set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qintcomp)
# endif()
# set_target_properties(qintcomp PROPERTIES
#     ${BUNDLE_ID_OPTION}
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )
#
# include(GNUInstallDirs)
#
# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(qintcomp)
# endif()
