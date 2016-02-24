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

#ifndef SFCGAL_CONTAINS_ALGORITHM
#define SFCGAL_CONTAINS_ALGORITHM

#include <SFCGAL/config.h>

namespace SFCGAL {
class Geometry;
class LineString;
class PolyhedralSurface;
class TriangulatedSurface;
namespace detail {
template <int Dim> class GeometrySet;
template <int Dim> struct PrimitiveHandle;
}

namespace algorithm {
class SurfaceGraph;

struct NoValidityCheck;

/**
 * Containment test on 3D geometries. Currently for PolyhedralSurface in PolyhedralSurface and Point in PolyhedralSurface only.
 * @pre ga and gb are valid geometries
 * @ingroup public_api
 */
SFCGAL_API bool contains3D( const Geometry& ga, const Geometry& gb );

/**
 * Containment test on 3D geometries. Currently for PolyhedralSurface in PolyhedralSurface and Point in PolyhedralSurface only.
 * @pre ga and gb are valid geometries
 * @ingroup detail
 * @warning the validity is assumed, no actual check is done
 */
SFCGAL_API bool contains3D( const Geometry& ga, const Geometry& gb, NoValidityCheck );

/**
 * Intersection test on GeometrySet
 * @ingroup detail
 */
bool convex_polyhedra_containment3D( const PolyhedralSurface&a, const Geometry& b );

bool convex_point_containment3D( const PolyhedralSurface& a, const Geometry& b );

}
} // end namespace algorithm
#endif
