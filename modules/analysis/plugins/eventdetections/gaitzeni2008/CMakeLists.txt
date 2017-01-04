SET(OPENMA_ANALYSIS_GAITZENI2008PLUGIN_SRCS
  plugins/eventdetections/gaitzeni2008/gaitzeni2008coordinate.cpp
  plugins/eventdetections/gaitzeni2008/gaitzeni2008velocity.cpp
  plugins/eventdetections/gaitzeni2008/gaitzeni2008plugin.cpp
)

SET(OPENMA_ANALYSIS_PLUGIN_NAME "GaitZeni2008Plugin" CACHE INTERNAL "")
SET(OPENMA_ANALYSIS_PLUGIN_SRCS ${OPENMA_ANALYSIS_GAITZENI2008PLUGIN_SRCS} CACHE INTERNAL "")

