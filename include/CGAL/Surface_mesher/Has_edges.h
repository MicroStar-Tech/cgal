// Copyright (c) 2006-2007  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/releases/CGAL-5.0.2/Surface_mesher/include/CGAL/Surface_mesher/Has_edges.h $
// $Id: Has_edges.h 254d60f 2019-10-19T15:23:19+02:00 Sébastien Loriot
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
// 
//
// Author(s)     : Laurent RINEAU

#ifndef CGAL_SURFACE_MESHER_HAS_EDGES_H
#define CGAL_SURFACE_MESHER_HAS_EDGES_H

#include <CGAL/license/Surface_mesher.h>


namespace CGAL {
  namespace Surface_mesher {

    struct Has_edges {
      static bool has_edges() { return true; }
    };
    struct Has_no_edges {
      static bool has_edges() { return false; }
    };

  } // end namespace Surface_mesher
} // end namespace CGAL

#endif // CGAL_SURFACE_MESHER_HAS_EDGES_H
