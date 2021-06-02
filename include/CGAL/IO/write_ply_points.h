// Copyright (c) 2015  Geometry Factory
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v5.2.2/Point_set_processing_3/include/CGAL/IO/write_ply_points.h $
// $Id: write_ply_points.h c253679 2020-04-18T16:27:58+02:00 Sébastien Loriot
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s) : Simon Giraudot

#ifndef CGAL_WRITE_PLY_POINTS_H
#define CGAL_WRITE_PLY_POINTS_H

#include <CGAL/license/Point_set_processing_3.h>

#include <CGAL/config.h>

#include <tuple>

#include <CGAL/IO/PLY.h>
#include <CGAL/property_map.h>
#include <CGAL/point_set_processing_assertions.h>
#include <CGAL/Iterator_range.h>

#include <CGAL/boost/graph/Named_function_parameters.h>
#include <CGAL/boost/graph/named_params_helper.h>

#include <boost/version.hpp>

#include <iostream>
#include <iterator>

namespace CGAL {

#ifdef DOXYGEN_RUNNING // Document some parts from Stream_support here for convenience
  /**
     \ingroup PkgPointSetProcessing3IOPly

     Generates a %PLY property handler to write 3D points. Points are
     written as 3 %PLY properties of type `FT` and named `x`, `y` and
     `z`. `FT` is `float` if the points use
     `CGAL::Simple_cartesian<float>` and `double` otherwise.

     \sa `write_ply_points_with_properties()`

     \tparam PointMap the property map used to store points.
  */
  template <typename PointMap>
  std::tuple<PointMap, PLY_property<FT>, PLY_property<FT>, PLY_property<FT> >
  make_ply_point_writer(PointMap point_map);

  /**
     \ingroup PkgPointSetProcessing3IOPly

     Generates a %PLY property handler to write 3D normal
     vectors. Vectors are written as 3 %PLY properties of type `FT`
     and named `nx`, `ny` and `nz`. `FT` is `float` if the vectors use
     `CGAL::Simple_cartesian<float>` and `double` otherwise.

     \sa `write_ply_points_with_properties()`

     \tparam VectorMap the property map used to store vectors.
  */
  template <typename VectorMap>
  std::tuple<VectorMap, PLY_property<FT>, PLY_property<FT>, PLY_property<FT> >
  make_ply_normal_writer(VectorMap normal_map);
#endif

  /// \cond SKIP_IN_MANUAL

namespace internal {

  namespace PLY {


  } // namespace PLY

} // namespace internal

  /// \endcond


/**
   \ingroup PkgPointSetProcessing3IOPly
   Saves the range of `points` with properties to a
   .ply stream. %PLY is either ASCII or binary depending on the value
   of `CGAL::get_mode(stream)`.

   Properties are handled through a variadic list of property
   handlers. A `PropertyHandler` can either be:

   - A `std::pair<PropertyMap, PLY_property<T> >` if the user wants
   to write a scalar value T as a %PLY property (for example, writing
   an `int` variable as an `int` %PLY property).

   - A `std::tuple<PropertyMap, PLY_property<T>...>` if the
   user wants to write a complex object as several %PLY
   properties. In that case, a specialization of `Output_rep` must
   be provided for `PropertyMap::value_type` that handles both ASCII
   and binary output (see `CGAL::get_mode()`).

   \sa `make_ply_point_writer()`
   \sa `make_ply_normal_writer()`

   \cgalRequiresCPP11

   \tparam PointRange is a model of `ConstRange`. The value type of
   its iterator is the key type of the `PropertyMap` objects provided
   within the `PropertyHandler` parameter.
   \tparam PropertyHandler handlers to recover properties.

   \return `true` on success.
*/
template < typename PointRange,
           typename ... PropertyHandler>
bool
write_ply_points_with_properties(
  std::ostream& stream, ///< output stream.
  const PointRange& points, ///< input point range.
  PropertyHandler&& ... properties) ///< parameter pack of property handlers
{
  CGAL_point_set_processing_precondition(points.begin() != points.end());

  if(!stream)
  {
    std::cerr << "Error: cannot open file" << std::endl;
    return false;
  }

  // Write header
  stream << "ply" << std::endl
         << ((get_mode(stream) == IO::BINARY) ? "format binary_little_endian 1.0" : "format ascii 1.0") << std::endl
         << "comment Generated by the CGAL library" << std::endl
         << "element vertex " << points.size() << std::endl;

  internal::PLY::output_property_header (stream, std::forward<PropertyHandler>(properties)...);

  stream << "end_header" << std::endl;


  // Write positions + normals
  for(typename PointRange::const_iterator it = points.begin(); it != points.end(); it++)
  {
    internal::PLY::output_properties (stream, it, std::forward<PropertyHandler>(properties)...);
  }

  return ! stream.fail();
}

/**
   \ingroup PkgPointSetProcessing3IOPly
   Saves the range of `points` (positions + normals, if available) to
   a .ply stream. %PLY is either ASCII or binary depending on the
   value of `CGAL::get_mode(stream)`.

   \tparam PointRange is a model of `ConstRange`. The value type of
   its iterator is the key type of the named parameter `point_map`.

   \param stream output stream.
   \param points input point range.
   \param np an optional sequence of \ref bgl_namedparameters "Named Parameters" among the ones listed below

   \cgalNamedParamsBegin
     \cgalParamNBegin{point_map}
       \cgalParamDescription{a property map associating points to the elements of the point range}
       \cgalParamType{a model of `ReadablePropertyMap` with value type `geom_traits::Point_3`}
       \cgalParamDefault{`CGAL::Identity_property_map<geom_traits::Point_3>`}
     \cgalParamNEnd

     \cgalParamNBegin{normal_map}
       \cgalParamDescription{a property map associating normals to the elements of the poing range}
       \cgalParamType{a model of `ReadablePropertyMap` with value type `geom_traits::Vector_3`}
       \cgalParamDefault{If this parameter is omitted, normals are not written in the output stream.}
     \cgalParamNEnd

     \cgalParamNBegin{geom_traits}
       \cgalParamDescription{an instance of a geometric traits class}
       \cgalParamType{a model of `Kernel`}
       \cgalParamDefault{a \cgal Kernel deduced from the point type, using `CGAL::Kernel_traits`}
     \cgalParamNEnd
   \cgalNamedParamsEnd

   \return true on success.
   \cgalRequiresCPP11
*/
template <typename PointRange,
          typename NamedParameters>
bool
write_ply_points(
  std::ostream& stream,
  const PointRange& points,
  const NamedParameters& np)
{
  using parameters::choose_parameter;
  using parameters::get_parameter;

  // basic geometric types
  typedef typename CGAL::GetPointMap<PointRange, NamedParameters>::type PointMap;
  typedef typename Point_set_processing_3::GetNormalMap<PointRange, NamedParameters>::type NormalMap;

  bool has_normals = !(boost::is_same<NormalMap,
                       typename Point_set_processing_3::GetNormalMap<PointRange, NamedParameters>::NoMap>::value);

  PointMap point_map = choose_parameter<PointMap>(get_parameter(np, internal_np::point_map));
  NormalMap normal_map = choose_parameter<NormalMap>(get_parameter(np, internal_np::normal_map));

  if (has_normals)
    return write_ply_points_with_properties(
      stream, points,
      make_ply_point_writer(point_map),
      make_ply_normal_writer(normal_map));
  // else
  return write_ply_points_with_properties(
    stream, points,
    make_ply_point_writer(point_map));
}

/// \cond SKIP_IN_MANUAL
// variant with default NP
template <typename PointRange>
bool
write_ply_points(
  std::ostream& stream,
  const PointRange& points)
{
  return write_ply_points
    (stream, points, CGAL::Point_set_processing_3::parameters::all_default(points));
}

#ifndef CGAL_NO_DEPRECATED_CODE
// deprecated API
template < typename ForwardIterator,
           typename PointMap,
           typename VectorMap >
CGAL_DEPRECATED_MSG("you are using the deprecated V1 API of CGAL::write_ply_points_and_normals(), please update your code")
bool
write_ply_points_and_normals(
  std::ostream& stream, ///< output stream.
  ForwardIterator first, ///< first input point.
  ForwardIterator beyond, ///< past-the-end input point.
  PointMap point_map, ///< property map: value_type of OutputIterator -> Point_3.
  VectorMap normal_map) ///< property map: value_type of OutputIterator -> Vector_3.
{
  CGAL::Iterator_range<ForwardIterator> points (first, beyond);
  return write_ply_points
    (stream, points,
     CGAL::parameters::point_map (point_map).
     normal_map (normal_map));
}

// deprecated API
template <typename ForwardIterator,
          typename VectorMap
>
CGAL_DEPRECATED_MSG("you are using the deprecated V1 API of CGAL::write_ply_points_and_normals(), please update your code")
bool
write_ply_points_and_normals(
  std::ostream& stream, ///< output stream.
  ForwardIterator first, ///< first input point.
  ForwardIterator beyond, ///< past-the-end input point.
  VectorMap normal_map) ///< property map: value_type of OutputIterator -> Vector_3.
{
  CGAL::Iterator_range<ForwardIterator> points (first, beyond);
  return write_ply_points
    (stream, points,
     CGAL::parameters::normal_map (normal_map));
}

// deprecated API
template < typename ForwardIterator,
           typename PointMap >
CGAL_DEPRECATED_MSG("you are using the deprecated V1 API of CGAL::write_ply_points(), please update your code")
bool
write_ply_points(
  std::ostream& stream, ///< output stream.
  ForwardIterator first, ///< first input point.
  ForwardIterator beyond, ///< past-the-end input point.
  PointMap point_map) ///< property map: value_type of OutputIterator -> Point_3.
{
  CGAL::Iterator_range<ForwardIterator> points (first, beyond);
  return write_ply_points
    (stream, points,
     CGAL::parameters::point_map(point_map));
}

// deprecated API
template < typename ForwardIterator >
CGAL_DEPRECATED_MSG("you are using the deprecated V1 API of CGAL::write_ply_points(), please update your code")
bool
write_ply_points(
  std::ostream& stream, ///< output stream.
  ForwardIterator first, ///< first input point.
  ForwardIterator beyond) ///< past-the-end input point.
{
  CGAL::Iterator_range<ForwardIterator> points (first, beyond);
  return write_ply_points
    (stream, points);
}
#endif // CGAL_NO_DEPRECATED_CODE
/// \endcond


} //namespace CGAL

#endif // CGAL_WRITE_PLY_POINTS_H
