// This file is part of libigl, a simple c++ geometry processing library.
// 
// Copyright (C) 2013 Alec Jacobson <alecjacobson@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#include "doublearea.h"
#include "edge_lengths.h"
#include "sort.h"
#include <cassert>
#include <iostream>

template <typename DerivedV, typename DerivedF, typename DeriveddblA>
IGL_INLINE void igl::doublearea( 
  const Eigen::PlainObjectBase<DerivedV> & V, 
  const Eigen::PlainObjectBase<DerivedF> & F, 
  Eigen::PlainObjectBase<DeriveddblA> & dblA)
{
  const int dim = V.cols();
  // Only support triangles
  assert(F.cols() == 3);
  const int m = F.rows();
  // Compute edge lengths
  Eigen::PlainObjectBase<DerivedV> l;
  // "Lecture Notes on Geometric Robustness" Shewchuck 09, Section 3.1
  // http://www.cs.berkeley.edu/~jrs/meshpapers/robnotes.pdf
  switch(dim)
  {
    case 3:
    {
      dblA = Eigen::PlainObjectBase<DeriveddblA>::Zero(m,1);
      for(int d = 0;d<3;d++)
      {
        Eigen::Matrix<typename DerivedV::Scalar, DerivedV::RowsAtCompileTime, 2> Vd(V.rows(),2);
        Vd << V.col(d), V.col((d+1)%3);
        Eigen::PlainObjectBase<DeriveddblA> dblAd;
        doublearea(Vd,F,dblAd);
        dblA.array() += dblAd.array().square();
      }
      dblA = dblA.array().sqrt().eval();
      break;
    }
    case 2:
    {
      dblA.resize(m,1);
      for(int f = 0;f<m;f++)
      {
        auto r = V.row(F(f,0))-V.row(F(f,2));
        auto s = V.row(F(f,1))-V.row(F(f,2));
        dblA(f) = r(0)*s(1) - r(1)*s(0);
      }
      break;
    }
    default:
    {
      edge_lengths(V,F,l);
      return doublearea(l,dblA);
    }
  }
}

template <typename Derivedl, typename DeriveddblA>
IGL_INLINE void igl::doublearea( 
  const Eigen::PlainObjectBase<Derivedl> & ul,
  Eigen::PlainObjectBase<DeriveddblA> & dblA)
{
  using namespace Eigen;
  using namespace std;
  // Only support triangles
  assert(ul.cols() == 3);
  // Number of triangles
  const int m = ul.rows();
  Eigen::PlainObjectBase<Derivedl> l;
  MatrixXi _;
  sort(ul,2,false,l,_);
  // semiperimeters
  Matrix<typename Derivedl::Scalar,Dynamic,1> s = l.rowwise().sum()*0.5;
  assert(s.rows() == m);
  // resize output
  dblA.resize(l.rows(),1);
  for(int i = 0;i<m;i++)
  {
    //// Heron's formula for area
    //const typename Derivedl::Scalar arg = 
    //  s(i)*(s(i)-l(i,0))*(s(i)-l(i,1))*(s(i)-l(i,2));
    //assert(arg>=0);
    //dblA(i) = 2.0*sqrt(arg);
    // Kahan's Heron's formula
    const typename Derivedl::Scalar arg = 
      (l(i,0)+(l(i,1)+l(i,2)))*
      (l(i,2)-(l(i,0)-l(i,1)))*
      (l(i,2)+(l(i,0)-l(i,1)))*
      (l(i,0)+(l(i,1)-l(i,2)));
    dblA(i) = 2.0*0.25*sqrt(arg);
    assert( l(i,2) - (l(i,0)-l(i,1)) && "FAILED KAHAN'S ASSERTION");
    assert(dblA(i) == dblA(i) && "DOUBLEAREA() PRODUCED NaN");
  }
}

#ifndef IGL_HEADER_ONLY
// Explicit template specialization
// generated by autoexplicit.sh
template void igl::doublearea<Eigen::Matrix<float, -1, 3, 1, -1, 3>, Eigen::Matrix<unsigned int, -1, -1, 1, -1, -1>, Eigen::Matrix<double, -1, 1, 0, -1, 1> >(Eigen::PlainObjectBase<Eigen::Matrix<float, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<unsigned int, -1, -1, 1, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&);
template void igl::doublearea<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 1, 0, -1, 1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&);
template void igl::doublearea<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 1, 0, -1, 1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&);
template void igl::doublearea<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> >&);
template void igl::doublearea<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 1, 0, -1, 1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&);
#endif
