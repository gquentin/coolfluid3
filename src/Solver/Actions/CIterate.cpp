// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <iomanip>

#include "Common/Log.hpp"
#include "Common/OptionT.hpp"
#include "Common/CBuilder.hpp"
#include "Common/Foreach.hpp"
#include "Common/Log.hpp"
#include "Common/FindComponents.hpp"
#include "Common/Core.hpp"
#include "Common/CEnv.hpp"

#include "Math/MathConsts.hpp"

#include "Solver/Actions/CIterate.hpp"
#include "Solver/Actions/CCriterion.hpp"

namespace CF {
namespace Solver {
namespace Actions {

using namespace Common;
using namespace Math::MathConsts;

Common::ComponentBuilder < CIterate, CAction, LibActions > CIterate_Builder;

////////////////////////////////////////////////////////////////////////////////

CIterate::CIterate( const std::string& name  ) :
  CAction ( name ),
  m_iter(0),
  m_verbose(false),
  m_max_iter(Uint_max())
{
  mark_basic();
  properties()["brief"] = std::string("Iterator object");
  std::string description =
  "This object handles iterations\n"
  "It can have one or more stop criteria\n";
  properties()["description"] = description;

  properties().add_option( OptionT<bool>::create("verbose","Verbose","Print iteration number",m_verbose))
    ->link_to(&m_verbose);

  properties().add_option< OptionT<Uint> >("max_iter","Max Iterations","Maximal number of iterations",m_max_iter)
    ->link_to(&m_max_iter);
}

////////////////////////////////////////////////////////////////////////////////

CIterate::~CIterate()
{
}

////////////////////////////////////////////////////////////////////////////////

void CIterate::execute ()
{
  m_iter=0;
  bool exit_iterations = false;
  while( m_iter != m_max_iter)
  {
    // check if any criterion are met and abort if so
    boost_foreach(CCriterion& stop_criterion, find_components<CCriterion>(*this))
    {
      if (stop_criterion())
      {
        exit_iterations = true;
        break;
      }
    }
    if (exit_iterations)  break;

    if (m_verbose)
      CFinfo << uri().path() << "[" << m_iter << "]" << CFendl;

    // call all actions and action links inside this component
    boost_foreach(Component& child, children())
    {
      if (CAction::Ptr action = child.follow()->as_ptr<CAction>())
        action->execute();
    }

    // update the iteration
    ++m_iter;
  }
}

////////////////////////////////////////////////////////////////////////////////

} // Actions
} // Solver
} // CF
