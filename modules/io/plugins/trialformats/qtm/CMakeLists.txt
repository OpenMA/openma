SET(OPENMA_IO_QTMPLUGIN_SRCS
  plugins/trialformats/qtm/qtmhandler.cpp
  plugins/trialformats/qtm/qtminternals.cpp
  plugins/trialformats/qtm/qtmplugin.cpp
)

SET(OPENMA_IO_PLUGIN_NAME "QTMPlugin" CACHE INTERNAL "")
SET(OPENMA_IO_PLUGIN_SRCS ${OPENMA_IO_QTMPLUGIN_SRCS} CACHE INTERNAL "")

