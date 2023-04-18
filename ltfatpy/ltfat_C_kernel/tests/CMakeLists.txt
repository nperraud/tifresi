add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/test.c)
target_link_libraries(test ltfat ${CMAKE_EXE_LINKER_FLAGS})
target_compile_definitions(test PUBLIC LTFAT_DOUBLE PUBLIC)
install(TARGETS test RUNTIME DESTINATION ${LTFATPY_SOURCE_DIR}/bin)

add_executable(test_signal ${CMAKE_CURRENT_SOURCE_DIR}/test_signal.c)
target_link_libraries(test_signal ltfat ${CMAKE_EXE_LINKER_FLAGS})
target_compile_definitions(test_signal PUBLIC LTFAT_DOUBLE PUBLIC)
install(TARGETS test_signal RUNTIME DESTINATION ${LTFATPY_SOURCE_DIR}/bin)