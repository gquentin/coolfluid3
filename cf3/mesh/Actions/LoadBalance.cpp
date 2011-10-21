// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "coolfluid-packages.hpp"

#include "common/CBuilder.hpp"
#include "common/Log.hpp"

#include "common/PE/Comm.hpp"

#include "mesh/Actions/LoadBalance.hpp"
#include "mesh/CMesh.hpp"
#include "mesh/CRegion.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace mesh {
namespace Actions {

using namespace common;
using namespace common::PE;

////////////////////////////////////////////////////////////////////////////////

common::ComponentBuilder < LoadBalance, CMeshTransformer, LibActions> LoadBalance_Builder;

//////////////////////////////////////////////////////////////////////////////

LoadBalance::LoadBalance( const std::string& name )
: CMeshTransformer(name)
{

  properties()["brief"] = std::string("Construct global node and element numbering based on coordinates hash values");
  std::string desc;
  desc =
    "  Usage: LoadBalance Regions:array[uri]=region1,region2\n\n";
  properties()["description"] = desc;

#if defined (CF3_HAVE_PTSCOTCH)
  m_partitioner = build_component_abstract_type<CMeshTransformer>("CF.Mesh.PTScotch.CPartitioner","partitioner");
#elif defined (CF3_HAVE_ZOLTAN)
  m_partitioner = build_component_abstract_type<CMeshTransformer>("CF.Mesh.Zoltan.CPartitioner","partitioner");
  m_partitioner->configure_option("graph_package", std::string("PHG"));
#endif
  add_static_component(*m_partitioner);
}

/////////////////////////////////////////////////////////////////////////////

void LoadBalance::execute()
{

  CMesh& mesh = *m_mesh.lock();

  // balance if parallel run with multiple processors
  if( Comm::instance().is_active() && Comm::instance().size() > 1 )
  {

    CFinfo << "loadbalancing mesh:" << CFendl;

    CFinfo << "  + building joint node & element global numbering" << CFendl;

    // build global numbering and connectivity of nodes and elements (necessary for partitioning)
    build_component_abstract_type<CMeshTransformer>("CF.Mesh.Actions.CGlobalNumbering","glb_numbering")->transform(mesh);

    CFinfo << "  + building global node-element connectivity" << CFendl;

    build_component_abstract_type<CMeshTransformer>("CF.Mesh.Actions.CGlobalConnectivity","glb_connectivity")->transform(mesh);


    CFinfo << "  + partitioning and migrating" << CFendl;
    m_partitioner->transform(mesh);

    CFinfo << "  + growing overlap layer" << CFendl;
    build_component_abstract_type<CMeshTransformer>("CF.Mesh.Actions.GrowOverlap","grow_overlap")->transform(mesh);


    CFinfo << "  + deallocating unused connectivity" << CFendl;

    /// @todo check that this actually frees the memory
    //mesh.geometry().glb_elem_connectivity().resize(0);

  }
  else
  {
    /// @todo disable this when below is re-enabled
    CFinfo << "  + building joint node & element global numbering" << CFendl;
    build_component_abstract_type<CMeshTransformer>("CF.Mesh.Actions.CGlobalNumbering","glb_numbering")->transform(mesh);
  }


  /// @todo this has to be re-enabled
#if 0
  // Create global node indexes for nodes and elements
  // plus ranks which are necessary for CommPattern (both in serial and parallel)

  CFinfo << "creating continuous global node numbering" << CFendl;

  build_component_abstract_type<CMeshTransformer>("CF.Mesh.Actions.CGlobalNumberingNodes","glb_node_numbering")->transform(mesh);

  CFinfo << "creating continuous global element numbering" << CFendl;

  build_component_abstract_type<CMeshTransformer>("CF.Mesh.Actions.CGlobalNumberingElements","glb_elem_numbering")->transform(mesh);
#endif
}

//////////////////////////////////////////////////////////////////////////////


} // Actions
} // mesh
} // cf3