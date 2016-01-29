#ifndef __openma_maths_h
#define __openma_maths_h

#if defined(_MSC_VER)
  // Disable warning for too long decorated name
  // See: https://msdn.microsoft.com/en-us/library/074af4b6.aspx
  // NOTE: This warning is raised when complex ma::maths expressions are used.
  #pragma warning(disable : 4503)
#endif

// Addons for Eigen
#ifdef EIGEN_CORE_H
  #error The header maconfig.h must be included before the inclusion of any Eigen headers to integrate the implemented Eigen plugins
#endif
// Need to be defined before the inclusion of the Eigen headers
#define EIGEN_DENSEBASE_PLUGIN <Eigen_openma/Plugin/DenseBaseAddons.h>
#define EIGEN_VECTOROP_PLUGIN <Eigen_openma/Plugin/VectorOpAddons.h>
#include <Eigen_openma/Plugin/ForwardDeclarations.h>
#include <Eigen/Core>
#include <Eigen_openma/Plugin/Functors.h>

#include <utility> // std::declval
#define OPENMA_MATHS_DECLVAL_NESTED(xpr) \
  std::declval<const typename ma::maths::Nested<xpr>::type>()

#include "openma/base/macros.h" // _OPENMA_CONSTEXPR, _OPENMA_NOEXCEPT

#include "openma/maths/forwarddeclarations.h"
#include "openma/maths/traits.h"    
#include "openma/maths/xprbase.h"
#include "openma/maths/arraybase.h"
#include "openma/maths/array.h"
#include "openma/maths/map.h"
#include "openma/maths/returnbyvalue.h"
#include "openma/maths/blockop.h"
#include "openma/maths/unaryop.h"
#include "openma/maths/binaryop.h"
#include "openma/maths/utils.h"

#endif // __openma_maths_h