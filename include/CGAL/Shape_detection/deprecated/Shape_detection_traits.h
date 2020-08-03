// Copyright (c) 2015 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-5.0.3/Shape_detection/include/CGAL/Shape_detection/deprecated/Shape_detection_traits.h $
// $Id: Shape_detection_traits.h 0779373 2020-03-26T13:31:46+01:00 Sébastien Loriot
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Sven Oesau, Yannick Verdie, Clément Jamin, Pierre Alliez
//

#ifndef CGAL_SHAPE_DETECTION_SHAPE_DETECTION_TRAITS_DEPRECATED_H
#define CGAL_SHAPE_DETECTION_SHAPE_DETECTION_TRAITS_DEPRECATED_H

#include <CGAL/license/Shape_detection.h>

// CGAL includes.
#include <CGAL/Search_traits_3.h>

// Deprecated -->
#define CGAL_DEPRECATED_HEADER "<CGAL/Shape_detection/deprecated/Shape_detection_traits.h>"
#define CGAL_REPLACEMENT_HEADER "<CGAL/Shape_detection/Efficient_RANSAC/Efficient_RANSAC_traits.h>"

#define CGAL_DEPRECATED_MESSAGE_DETAILS \
  "CGAL::Shape_detection_3::Shape_detection_traits<> has been replaced by the class "\
  "CGAL::Shape_detection::Efficient_RANSAC_traits<>."

#include <CGAL/internal/deprecation_warning.h>

namespace CGAL {
namespace Shape_detection {

  #ifdef DOXYGEN_NS
    namespace deprecated {
  #endif

  /*!
    \ingroup PkgShapeDetectionDEPR
    \brief A traits class to use with the shape detection
    classes `Efficient_RANSAC` and `deprecated::Region_growing`.

    \cgalModels `EfficientRANSACTraits`

    \tparam Gt a model of the concept `#Kernel` with `Gt::FT` being
    `float` or `double`.

    \tparam InputRange is a model of `Range` with random access iterators,
    providing input points and normals through the following two property maps.

    \tparam InputPointMap is a model of `ReadablePropertyMap` with
    `std::iterator_traits<Input_range::iterator>::%value_type` as key type and
    `Gt::Point_3` as value type.

    \tparam InputNormalMap is a model of `ReadablePropertyMap` with
    `std::iterator_traits<Input_range::iterator>::%value_type` as key type and
    `Gt::Vector_3` as value type.
  */
  template <class Gt,
            class InputRange,
            class InputPointMap,
            class InputNormalMap>
  struct Shape_detection_traits {
    ///
    typedef typename Gt::FT FT;
    ///
    typedef typename Gt::Point_3 Point_3;
    ///
    typedef typename Gt::Vector_3 Vector_3;
    ///
    typedef typename Gt::Sphere_3 Sphere_3;
    ///
    typedef typename Gt::Segment_3 Segment_3;
    ///
    typedef typename Gt::Line_3 Line_3;
    ///
    typedef typename Gt::Circle_2 Circle_2;
    ///
    typedef typename Gt::Plane_3 Plane_3;
    ///
    typedef typename Gt::Point_2 Point_2;
    ///
    typedef typename Gt::Vector_2 Vector_2;
    ///
    typedef InputRange Input_range;
    ///
    typedef InputPointMap Point_map;
    ///
    typedef InputNormalMap Normal_map;
    ///
    typedef CGAL::Search_traits_3<Gt> Search_traits;
    ///
    Shape_detection_traits(const Gt& gt = Gt())
    : m_gt(gt) { }

    typedef typename Gt::Construct_point_3 Construct_point_3;
    Construct_point_3 construct_point_3_object() const
    { return m_gt.construct_point_3_object(); }

    typedef typename Gt::Construct_vector_3 Construct_vector_3;
    Construct_vector_3 construct_vector_3_object() const
    { return m_gt.construct_vector_3_object(); }

    typedef typename Gt::Construct_point_2 Construct_point_2;
    Construct_point_2 construct_point_2_object() const
    { return m_gt.construct_point_2_object(); }

    typedef typename Gt::Construct_vector_2 Construct_vector_2;
    Construct_vector_2 construct_vector_2_object() const
    { return m_gt.construct_vector_2_object(); }

    typedef typename Gt::Construct_sphere_3 Construct_sphere_3;
    Construct_sphere_3 construct_sphere_3_object() const
    { return m_gt.construct_sphere_3_object(); }

    typedef typename Gt::Construct_line_3 Construct_line_3;
    Construct_line_3 construct_line_3_object() const
    { return m_gt.construct_line_3_object(); }

    typedef typename Gt::Construct_circle_2 Construct_circle_2;
    Construct_circle_2 construct_circle_2_object() const
    { return m_gt.construct_circle_2_object(); }

    typedef typename Gt::Construct_point_on_3 Construct_point_on_3;
    Construct_point_on_3 construct_point_on_3_object() const
    { return m_gt.construct_point_on_3_object(); }

    typedef typename Gt::Compute_x_2 Compute_x_2;
    Compute_x_2 compute_x_2_object() const
    { return m_gt.compute_x_2_object(); }

    typedef typename Gt::Compute_y_2 Compute_y_2;
    Compute_y_2 compute_y_2_object() const
    { return m_gt.compute_y_2_object(); }

    typedef typename Gt::Compute_x_3 Compute_x_3;
    Compute_x_3 compute_x_3_object() const
    { return m_gt.compute_x_3_object(); }

    typedef typename Gt::Compute_y_3 Compute_y_3;
    Compute_y_3 compute_y_3_object() const
    { return m_gt.compute_y_3_object(); }

    typedef typename Gt::Compute_z_3 Compute_z_3;
    Compute_z_3 compute_z_3_object() const
    { return m_gt.compute_z_3_object(); }

    typedef typename Gt::Compute_squared_length_3 Compute_squared_length_3;
    Compute_squared_length_3 compute_squared_length_3_object() const
    { return m_gt.compute_squared_length_3_object(); }

    typedef typename Gt::Compute_squared_length_2 Compute_squared_length_2;
    Compute_squared_length_2 compute_squared_length_2_object() const
    { return m_gt.compute_squared_length_2_object(); }

    typedef typename Gt::Construct_scaled_vector_3 Construct_scaled_vector_3;
    Construct_scaled_vector_3 construct_scaled_vector_3_object() const
    { return m_gt.construct_scaled_vector_3_object(); }

    typedef typename Gt::Construct_sum_of_vectors_3 Construct_sum_of_vectors_3;
    Construct_sum_of_vectors_3 construct_sum_of_vectors_3_object() const
    { return m_gt.construct_sum_of_vectors_3_object(); }

    typedef typename Gt::Construct_translated_point_3 Construct_translated_point_3;
    Construct_translated_point_3 construct_translated_point_3_object() const
    { return m_gt.construct_translated_point_3_object(); }

    typedef typename Gt::Compute_scalar_product_3 Compute_scalar_product_3;
    Compute_scalar_product_3 compute_scalar_product_3_object() const
    { return m_gt.compute_scalar_product_3_object(); }

    typedef typename Gt::Construct_cross_product_vector_3 Construct_cross_product_vector_3;
    Construct_cross_product_vector_3 construct_cross_product_vector_3_object() const
    { return m_gt.construct_cross_product_vector_3_object(); }

    typedef typename Gt::Construct_center_3 Construct_center_3;
    Construct_center_3 construct_center_3_object() const
    { return m_gt.construct_center_3_object(); }

    typedef typename Gt::Construct_center_2 Construct_center_2;
    Construct_center_2 construct_center_2_object() const
    { return m_gt.construct_center_2_object(); }

    typedef typename Gt::Compute_squared_radius_3 Compute_squared_radius_3;
    Compute_squared_radius_3 compute_squared_radius_3_object() const
    { return m_gt.compute_squared_radius_3_object(); }

    typedef typename Gt::Compute_squared_radius_2 Compute_squared_radius_2;
    Compute_squared_radius_2 compute_squared_radius_2_object() const
    { return m_gt.compute_squared_radius_2_object(); }

    typedef typename Gt::Collinear_2 Collinear_2;
    Collinear_2 collinear_2_object() const
    { return m_gt.collinear_2_object(); }

    typedef typename Gt::Intersect_3 Intersect_3;
    Intersect_3 intersection_3_object() const
    { return m_gt.intersection_3_object(); }

  private:
    Gt m_gt;
  };

  #ifdef DOXYGEN_NS
    } // namespace deprecated
  #endif

} // namespace Shape_detection
} // namespace CGAL

// --<

#endif // CGAL_SHAPE_DETECTION_SHAPE_DETECTION_TRAITS_DEPRECATED_H
