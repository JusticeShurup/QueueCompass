# CMakeList.txt: проект CMake для QueueCompass; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.


add_library(stb_image INTERFACE)
add_library(stb_image_write INTERFACE)
target_include_directories(stb_image INTERFACE ./deps)
target_include_directories(stb_image_write INTERFACE ./deps)
add_executable (QueueCompass "main.cpp" "MyQueue.h" "deps/stb_image.h" "PathFinder.h" "PathFinder.cpp" )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/maps/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/maps)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET QueueCompass PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
