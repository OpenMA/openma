#ifndef customTypeTest_def_h
#define customTypeTest_def_h

#include <Eigen/Eigen>

class MyVectorType : public Eigen::VectorXd
{
public:
    MyVectorType(void):Eigen::VectorXd() {}
    // This constructor allows you to construct MyVectorType from Eigen expressions
    template<typename OtherDerived>
    MyVectorType(const Eigen::MatrixBase<OtherDerived>& other)
        : Eigen::VectorXd(other)
    { }
    // This method allows you to assign Eigen expressions to MyVectorType
    template<typename OtherDerived>
    MyVectorType& operator=(const Eigen::MatrixBase <OtherDerived>& other)
    {
        this->Eigen::VectorXd::operator=(other);
        return *this;
    }
};

// ------------------------------------------------------------------------- //

template<typename Scalar>
struct CwiseNullifyNaN {
  CwiseNullifyNaN() {};
  const Scalar operator()(const Scalar& x) const { return std::isnan(x) ? 0. : x; };
};

template<typename Scalar>
struct ScalarNanSumOp {
  ScalarNanSumOp() {};
  const Scalar operator()(const Scalar& a, const Scalar& b) const { return std::isnan(b) ? a : a+b; };
};

template<typename Scalar>
struct ScalarNanCountOp {
  ScalarNanCountOp() {};
  const Scalar operator()(const Scalar& a, const Scalar& b) const { return std::isnan(b) ? a : a+Scalar(1); };
};


// ------------------------------------------------------------------------- //

template<unsigned N> using MyArrayBase = Eigen::Array<double, Eigen::Dynamic, N>;
using Index = Eigen::Index;

template <Index N>
class MyArrayType : public MyArrayBase<N>
{
  
public:
  inline static const typename MyArrayBase<N>::ConstantReturnType Ones(Index samples)
  {
    return MyArrayBase<N>::Ones(samples,N);
  }
  inline static const typename MyArrayBase<N>::ConstantReturnType Zero(Index samples)
  {
    return MyArrayBase<N>::Zero(samples,N);
  }
  
  MyArrayType(): MyArrayBase<N>() {}
  MyArrayType(Index samples): MyArrayBase<N>(samples,N) {}
  
  // This constructor allows you to construct MyArrayType from Eigen expressions
  template<typename OtherDerived>
  MyArrayType(const Eigen::ArrayBase<OtherDerived>& other)
  : Eigen::Array<double, Eigen::Dynamic, N>(other)
  {};
  
  // This method allows you to assign Eigen expressions to MyArrayType
  template<typename OtherDerived>
  MyArrayType& operator=(const Eigen::ArrayBase<OtherDerived>& other)
  {
    this->MyArrayBase<N>::operator=(other);
    return *this;
  }
  
  inline bool hasOcclusion()
  {
    return this->isNaN().any();
  }
  
  inline auto myMean() -> decltype(std::declval<MyArrayBase<N>>().unaryExpr(CwiseNullifyNaN<double>()).colwise().sum() / std::declval<MyArrayBase<N>>().isFinite().colwise().count().template cast<double>())
  {
    return this->unaryExpr(CwiseNullifyNaN<double>()).colwise().sum() / this->isFinite().colwise().count().template cast<double>();
  }
};

#endif // customTypeTest_def_h