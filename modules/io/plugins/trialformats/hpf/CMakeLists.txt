SET(OPENMA_IO_HPFPLUGIN_SRCS
  plugins/trialformats/hpf/hpfhandler.cpp
  plugins/trialformats/hpf/hpfplugin.cpp
)

SET(OPENMA_IO_PLUGIN_NAME "HPFPlugin" CACHE INTERNAL "")
SET(OPENMA_IO_PLUGIN_SRCS ${OPENMA_IO_HPFPLUGIN_SRCS} CACHE INTERNAL "")

