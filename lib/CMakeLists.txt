message(STATUS "== BUILDING LIBRARIES ==")

set(CMAKE_INSTALL_PREFIX ${DESTDIR}/target/lib)

add_subdirectory(bento4)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DESTDIR}/target/lib)