// -*- C++ -*-
//
// Copyright (C) 1998, 1999, 2000, 2002  Los Alamos National Laboratory,
// Copyright (C) 1998, 1999, 2000, 2002  CodeSourcery, LLC
//
// This file is part of FreePOOMA.
//
// FreePOOMA is free software; you can redistribute it and/or modify it
// under the terms of the Expat license.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the Expat
// license for more details.
//
// You should have received a copy of the Expat license along with
// FreePOOMA; see the file LICENSE.
//
//-----------------------------------------------------------------------------
// Particle test 3: SpatialLayout, MP Remote Dynamic Attrib, 
//                  URM + MP Remote Brick Field
//-----------------------------------------------------------------------------

#include "particle_tests.h"


//-----------------------------------------------------------------------------
// The main routine for this test code
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  // Initialize POOMA and output stream, using Tester class

  startParticleTest(argc, argv, 
    "SpatialLayout: A=MPRemoteDynamic, F=URM+MPRemoteBrick");

  // Typedefs for what we are simulating here.

  typedef MultiPatch< DynamicTag, Remote<Dynamic> >    AttrEngineTag_t;
  typedef MultiPatch< GridTag, Remote<Brick> >         FieldEngineTag_t;
  typedef UniformRectilinearMesh<2>                    Mesh_t;

  typedef Field<Mesh_t,double,FieldEngineTag_t>    Field_t;
  typedef Field_t::Layout_t                            FieldLayout_t;
  typedef SpatialLayout<Mesh_t,FieldLayout_t>      ParLayout_t;
  typedef TestParTraits<AttrEngineTag_t,ParLayout_t>   ParTraits_t;
  typedef ParLayout_t::PointType_t                     PointType_t;

  // Specify the mesh parameters.

  Interval<2> meshDomain(12, 24);
  PointType_t meshOrigin(1.0, 2.0);
  PointType_t meshSpacing(0.5, 0.5);

  // Create a Mesh and Geometry.

  Mesh_t mesh(meshDomain, meshOrigin, meshSpacing);

  // Let things catch up

  Pooma::blockAndEvaluate();

  // The size of the mesh.

  Region<2,double> box;
  for (int d=0; d < 2; ++d)
    box[d] = Region<1>(meshOrigin(d),
		       meshOrigin(d) + 0.5 * (meshDomain[d].length() - 1));

  // Create a FieldLayout object.  We don't actually need a Field in
  // this example, though, just the layout.

  Loc<2> blocks(3, 4);
  FieldLayout_t flayout(meshDomain, blocks, DistributedTag());

  // Create a particle layout object.

  ParLayout_t layout(mesh, flayout);

  // Create a Particles object, using our special subclass.

  TestParticles<ParTraits_t> P;
  P.initialize(layout);

  // Run the tests.

  runParticleTest(P, box);

  // Return resulting error code and exit

  return 
    endParticleTest("SpatialLayout: A=MPRemoteDynamic, F=URM+MPRemoteBrick");
}

// ACL:rcsinfo
// ----------------------------------------------------------------------
// $RCSfile: particle_test3.cpp,v $   $Author: richard $
// $Revision: 1.11 $   $Date: 2004/11/01 18:17:00 $
// ----------------------------------------------------------------------
// ACL:rcsinfo
