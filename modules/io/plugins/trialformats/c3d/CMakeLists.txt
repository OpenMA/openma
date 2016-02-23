SET(OPENMA_IO_C3DPLUGIN_SRCS
  plugins/trialformats/c3d/c3ddatastream.cpp
  plugins/trialformats/c3d/c3dhandler.cpp
  plugins/trialformats/c3d/c3dplugin.cpp
)

SET(OPENMA_IO_PLUGIN_NAME "C3DPlugin" CACHE INTERNAL "")
SET(OPENMA_IO_PLUGIN_SRCS ${OPENMA_IO_C3DPLUGIN_SRCS} CACHE INTERNAL "")

