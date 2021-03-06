SET(MADSPIN_DATA_DIR ${LV_PLUGINS_DATA_DIR}/actor/actor_madspin)
SET(MADSPIN_DATA_DIR_FULL ${LV_PLUGINS_DATA_DIR_FULL}/actor/actor_madspin)

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${LIBVISUAL_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
)

LINK_DIRECTORIES(
  ${LIBVISUAL_LIBRARY_DIRS}
)

ADD_DEFINITIONS(-DSTAR_DIR="${MADSPIN_DATA_DIR_FULL}")

SET(actor_madspin_SOURCES
  madspin.c
)

SET(actor_madspin_IMAGES
  star1.bmp
  star2.bmp
)

ADD_LIBRARY(actor_madspin MODULE ${actor_madspin_SOURCES})
#-avoid-version

TARGET_LINK_LIBRARIES(actor_madspin
  ${LIBVISUAL_LIBRARIES}
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
)

INSTALL(TARGETS actor_madspin LIBRARY DESTINATION ${LV_ACTOR_PLUGIN_DIR})
INSTALL(FILES ${actor_madspin_IMAGES} DESTINATION ${MADSPIN_DATA_DIR})
