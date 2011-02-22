// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_RDM_RungeKutta_hpp
#define CF_RDM_RungeKutta_hpp

////////////////////////////////////////////////////////////////////////////////

#include "Common/CLink.hpp"

#include "Solver/CIterativeSolver.hpp"

#include "RDM/LibRDM.hpp"

namespace CF {

namespace Mesh   { class CField2; }
namespace Solver { class CDiscretization; }

namespace RDM {

////////////////////////////////////////////////////////////////////////////////

/// RKRD iterative solver
/// @author Tiago Quintino
/// @author Willem Deconinck
class RDM_API RungeKutta : public Solver::CIterativeSolver {

public: // typedefs

  typedef boost::shared_ptr<RungeKutta> Ptr;
  typedef boost::shared_ptr<RungeKutta const> ConstPtr;

public: // functions

  /// Contructor
  /// @param name of the component
  RungeKutta ( const std::string& name );

  /// Virtual destructor
  virtual ~RungeKutta();

  /// Get the class name
  static std::string type_name () { return "RungeKutta"; }

  // functions specific to the RungeKutta component
  
  virtual void solve();
  
  Solver::CDiscretization& discretization_method();
  
private: // functions

  void config_domain();
  
private: // data

  /// CFL number
  CF::Real m_cfl;
  
  /// solution field pointer
  boost::weak_ptr<Mesh::CField2> m_solution;
  /// residual field pointer
  boost::weak_ptr<Mesh::CField2> m_residual;
  /// update_coeff field pointer
  boost::weak_ptr<Mesh::CField2> m_update_coeff;

};

////////////////////////////////////////////////////////////////////////////////

} // RDM
} // CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_RDM_RungeKutta_hpp
