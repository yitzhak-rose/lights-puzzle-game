# Copy the entire resources directory into the build output directory so the program can open files using "resources/..."
# Using file(COPY ...) preserves subdirectory layout and works regardless of working directory.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

