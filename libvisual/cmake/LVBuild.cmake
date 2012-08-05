# Build helper functions and macros

MACRO(LV_GENERATE_ORC_SOURCES SOURCE TARGET_HEADER TARGET_SOURCE INCLUDES)
  ADD_CUSTOM_COMMAND(
    OUTPUT  ${TARGET_HEADER}
    COMMAND ${ORC_COMPILER_EXECUTABLE} -o ${TARGET_HEADER} --header ${SOURCE}
    MAIN_DEPENDENCY ${SOURCE}
  )
  ADD_CUSTOM_COMMAND(
    OUTPUT  ${TARGET_SOURCE}
    COMMAND ${ORC_COMPILER_EXECUTABLE} -o ${TARGET_SOURCE} --implementation ${SOURCE} --include ${INCLUDES}
    DEPENDS ${HEADER}
    MAIN_DEPENDENCY ${SOURCE}
  )
ENDMACRO()
