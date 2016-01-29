// Addons for Eigen
#ifdef EIGEN_CORE_H
  #error This header must be included before the inclusion of any Eigen headers to integrate the implemented Eigen plugins
#endif
// Need to be defined before the inclusion of the Eigen headers
#define EIGEN_DENSEBASE_PLUGIN <Eigen_openma/Plugin/DenseBaseAddons.h>
#define EIGEN_VECTOROP_PLUGIN <Eigen_openma/Plugin/VectorOpAddons.h>
#include <Eigen_openma/Plugin/ForwardDeclarations.h>
#include <Eigen/Core>
#include <Eigen_openma/Plugin/Functors.h>