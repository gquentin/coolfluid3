// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_Solver_Burgers2D_hpp
#define CF_Solver_Burgers2D_hpp

//#include <Eigen/Dense>
#include "Mesh/LibMesh.hpp"
#include "RDM/LibRDM.hpp"
#include "Math/MatrixTypes.hpp"

/////////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace RDM {

///////////////////////////////////////////////////////////////////////////////////////

class RDM_API Burgers2D
{

public: // functions
  /// Constructor
  Burgers2D ( );

  /// Destructor
  ~Burgers2D();

  /// Get the class name
  static std::string type_name () { return "Burgers2D"; }

  /// Number of equations in this physical model
  static const Uint nb_eqs = 1u;

  /// @returns the number of equations
  Uint nbeqs() const { return nb_eqs; }

  /// Function to compute the burgers flux
  static Real flux(const RealVector2 & coord, const Real & u, const RealVector2 & gradu);

  /// Function to compute the operator L(N) for Burgers
  static Real Lu(const RealVector2 & coord, const Real & u, const RealVector2 & gradN);
};

////////////////////////////////////////////////////////////////////////////////////

} // RDM
} // CF

/////////////////////////////////////////////////////////////////////////////////////

#endif // CF_Solver_RotationAdv2D_hpp
