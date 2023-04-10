
find_package(Doxygen)

if(NOT ${DOXYGEN_FOUND})
    message(STATUS "Doxygen is not found.")
endif()

set(doc_api ${CMAKE_CURRENT_BINARY_DIR}/api/Doxyfile)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/api/Doxyfile.in ${doc_api} @ONLY)

add_custom_target(
        documentation
        COMMAND ${DOXYGEN_EXECUTABLE} ${doc_api}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/api
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
)

install(
        DIRECTORY ${PROJECT_BINARY_DIR}/docs/api/html
        DESTINATION share/doc/${CMAKE_PROJECT_NAME}/api
)