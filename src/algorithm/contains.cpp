/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include <map>
#include <sstream>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/algorithm/contains.h>
#include <SFCGAL/algorithm/connection.h>
#include <SFCGAL/algorithm/covers.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/detail/triangulate/triangulateInGeometrySet.h>
#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/Envelope.h>
#include <SFCGAL/Exception.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/PolyhedralSurface.h>

#include <CGAL/box_intersection_d.h>

#include <SFCGAL/detail/Point_inside_polyhedron.h>

using namespace SFCGAL::detail;

namespace SFCGAL {
namespace algorithm {

bool convex_point_containment3D( const PolyhedralSurface& a, const Geometry& b )
{
  std::auto_ptr< detail::MarkedPolyhedron > polyhedron = a.clone()->toPolyhedron_3<Kernel, detail::MarkedPolyhedron >();

  if ( polyhedron->is_closed() )
  {
    Point_inside_polyhedron<MarkedPolyhedron, Kernel> is_in_poly( *polyhedron );

    GeometrySet<3> geometry( b );
    GeometrySet<3>::PointCollection points = geometry.points();

    bool status = true;
    for (GeometrySet<3>::PointCollection::iterator it = points.begin(); it != points.end(); ++it)
    {
      Kernel::Point_3 point = it->primitive();

      if ( is_in_poly( it->primitive() ) == CGAL::ON_UNBOUNDED_SIDE )
      {
        status = false;
      }
    }

    return status;
  }
  else
  {
    std::cout << "SFCGAL::algorithm::contains() - containment test can't performed on open polyhedra" << std::endl;
    return false;
  }

  return false;
}

bool convex_polyhedra_containment3D( const PolyhedralSurface& a, const Geometry& b )
{
  std::auto_ptr< detail::MarkedPolyhedron > polyhedron = a.clone()->toPolyhedron_3<Kernel, detail::MarkedPolyhedron >();

  if ( polyhedron->is_closed() )
  {
    Point_inside_polyhedron<MarkedPolyhedron, Kernel> is_in_poly( *polyhedron );

    GeometrySet<3> geometry( b );
    GeometrySet<3>::SurfaceCollection surfaces = geometry.surfaces();

    bool status = true;
    for (GeometrySet<3>::SurfaceCollection::iterator it = surfaces.begin(); it != surfaces.end(); ++it)
    {
      Kernel::Triangle_3 triangle = it->primitive();

      for(int i = 0; i < 3; i++)
      {
        if ( is_in_poly(  it->primitive().vertex(i) ) == CGAL::ON_UNBOUNDED_SIDE )
          status = false;
      }
    }

    return status;
  }
  else
  {
    std::cout << "given polyhedra isn't closed, can't perform containment test" << std::endl;
    return false;
  }

  return false;
}

bool contains3D( const Geometry& ga, const Geometry& gb )
{
  //with validity check
  SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( ga );
  SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( gb );

  //std::cout << " reference is " << ga.geometryType().c_str() << std::endl;
  //std::cout << " target is " << gb.geometryType().c_str() << std::endl;

  switch ( ga.geometryTypeId() )
  {
    case TYPE_POLYHEDRALSURFACE:
      switch ( gb.geometryTypeId() )
      {
        case TYPE_POLYHEDRALSURFACE:
          return convex_polyhedra_containment3D( ga.as<PolyhedralSurface>(), gb);
        case TYPE_POINT:
          return convex_point_containment3D( ga.as<PolyhedralSurface>(), gb);
        default:
          std::cerr << "SFCGAL::algorithm::contains::reference geometry must be of type PolyhedralSurface or Point." << std::endl;
          return false;
      }
      break;
    default:
      std::cerr << "SFCGAL::algorithm::contains::reference geometry must be of type PolyhedralSurface or Point." << std::endl;
      return false;
  }

  return false;
}

bool contains3D( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
  switch ( ga.geometryTypeId() )
  {
    case TYPE_POLYHEDRALSURFACE:
      switch ( gb.geometryTypeId() )
      {
        case TYPE_POLYHEDRALSURFACE:
          return convex_polyhedra_containment3D( ga.as<PolyhedralSurface>(), gb);
        case TYPE_POINT:
          return convex_point_containment3D( ga.as<PolyhedralSurface>(), gb);
        default:
          std::cerr << "SFCGAL::algorithm::contains::reference geometry must be of type PolyhedralSurface or Point." << std::endl;
          return false;
      }
      break;
    default:
      std::cerr << "SFCGAL::algorithm::contains::reference geometry must be of type PolyhedralSurface or Point." << std::endl;
      return false;
  }

  return false;
}

} // end namespace algorithm
} // end namespace SFCGAL
