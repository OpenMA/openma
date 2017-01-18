SET(OPENMA_IO_BSFPLUGIN_SRCS
  plugins/trialformats/bsf/bsfhandler.cpp
  plugins/trialformats/bsf/bsfplugin.cpp
)

SET(OPENMA_IO_PLUGIN_NAME "BSFPlugin" CACHE INTERNAL "")
SET(OPENMA_IO_PLUGIN_SRCS ${OPENMA_IO_BSFPLUGIN_SRCS} CACHE INTERNAL "")

