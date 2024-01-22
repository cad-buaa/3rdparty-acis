/* ORIGINAL: acis2.1/kerndata/geom/allcurve.hxx */
/* $Id: allcurve.hxx,v 1.10 2000/12/26 18:46:50 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Modifications:
// al 26Oct05 : Added helix header.

#if !defined( ALLCURVE_HDR_DEF )
#define  ALLCURVE_HDR_DEF

// Haul in all the currently-defined curve types

#include "curve.hxx"

#include "straight.hxx"
#include "ellipse.hxx"
#include "intcurve.hxx"
#include "degenerate.hxx"
#include "helix.hxx"
#ifndef NO_MESH_CLASSES
#include "compcurv.hxx"
#endif
#include "undefc.hxx"

#endif
