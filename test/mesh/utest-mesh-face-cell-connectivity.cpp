// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Tests cf3::mesh::CFaceCellConnectivity"

#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>

#include "common/Log.hpp"
#include "common/Core.hpp"
#include "common/CRoot.hpp"
#include "common/FindComponents.hpp"

#include "Tools/Testing/TimedTestFixture.hpp"

#include "mesh/CMesh.hpp"
#include "mesh/CElements.hpp"
#include "mesh/Geometry.hpp"
#include "mesh/CRegion.hpp"
#include "mesh/CMeshReader.hpp"
#include "mesh/CSimpleMeshGenerator.hpp"
#include "mesh/CFaceCellConnectivity.hpp"
#include "mesh/ConnectivityData.hpp"

using namespace boost;
using namespace boost::assign;
using namespace cf3;
using namespace cf3::mesh;
using namespace cf3::common;
using namespace cf3::Tools;

////////////////////////////////////////////////////////////////////////////////

struct FaceCellConnectivity_Fixture //: public Testing::TimedTestFixture
{
  /// common setup for each test case
  FaceCellConnectivity_Fixture()
  {
     // uncomment if you want to use arguments to the test executable
     //int*    argc = &boost::unit_test::framework::master_test_suite().argc;
     //char*** argv = &boost::unit_test::framework::master_test_suite().argv;

  }

  /// common tear-down for each test case
  ~FaceCellConnectivity_Fixture()
  {
  }

  /// possibly common functions used on the tests below


  /// common values accessed by all tests goes here
  static CMesh::Ptr m_mesh;
};

CMesh::Ptr FaceCellConnectivity_Fixture::m_mesh;
////////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_SUITE( FaceCellConnectivity_TestSuite, FaceCellConnectivity_Fixture )

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( Constructors )
{
  CFaceCellConnectivity::Ptr c = allocate_component<CFaceCellConnectivity>("faces_to_cells");
  BOOST_CHECK_EQUAL(c->name(),"faces_to_cells");
  BOOST_CHECK_EQUAL(CFaceCellConnectivity::type_name(), "CFaceCellConnectivity");
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( create_mesh )
{

 // create meshreader
 // CMeshReader::Ptr meshreader = build_component_abstract_type<CMeshReader>("CF.Mesh.Neu.CReader","meshreader");
 // boost::filesystem::path fp_source ("quadtriag.neu");
 // m_mesh = meshreader->create_mesh_from(fp_source);

  m_mesh = Core::instance().root().create_component_ptr<CMesh>("mesh");
  Uint scale = 2;
  std::vector<Real> lengths  = list_of(4.)(2.);
  std::vector<Uint> nb_cells = list_of(scale*2u)(scale*2u);
  CSimpleMeshGenerator& mesh_gen = Core::instance().root().create_component<CSimpleMeshGenerator>("mesh_gen");
  mesh_gen.configure_option("mesh",m_mesh->uri());
  mesh_gen.configure_option("lengths",lengths);
  mesh_gen.configure_option("nb_cells",nb_cells);
  mesh_gen.execute();
  BOOST_CHECK(true);
}
////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( face_elem_connectivity )
{

  // create and setup node to elements connectivity
  CFaceCellConnectivity::Ptr c = m_mesh->create_component_ptr<CFaceCellConnectivity>("face_cell_connectivity");
  c->setup( find_component<CRegion>(*m_mesh) );

  BOOST_CHECK_EQUAL(c->connectivity().size() , 40u);

  CFinfo << "nodes of face 0 : ";
  boost_foreach(const Uint node, c->face_nodes(0) )
    CFinfo << "  " << node;
  CFinfo << CFendl;
  // Output whole node to elements connectivity
  //CFinfo << c->connectivity() << CFendl;
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////
