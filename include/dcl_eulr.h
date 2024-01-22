﻿/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DECL_EULR

#if defined( __euler ) || defined( __SpaACIS ) || defined( __SPAAcisDs )
#define BUILDING_LOCAL_FILE
#endif

#include "importexport.h"
#include "acis/gme/euler/gme_euler.hxx"

#ifdef IMPORT_EXPORT_SYMBOLS
# ifdef BUILDING_LOCAL_FILE
#  define DECL_EULR EXPORT_SYMBOL
# else
#  define DECL_EULR IMPORT_SYMBOL
# endif
#else
# define DECL_EULR
#endif

/* force link in VC++ */

#if !defined( CONCAT )
#define CONCAT2(a,b) a ## b
#define CONCAT(a,b) CONCAT2(a,b)
#endif

#ifndef SPA_NO_AUTO_LINK
# ifndef BUILDING_LOCAL_FILE
#  if defined(_MSC_VER)
#   if defined( SPA_INTERNAL_BUILD ) || defined( NOBIGLIB )
#    define spa_lib_name "euler"
#   else
#    define spa_lib_name "SpaACIS"
#   endif
#   if defined( _DEBUG ) && !defined( SPA_INTERNAL_BUILD )
#    pragma comment( lib, CONCAT( spa_lib_name, "d.lib" ))
#   else
#    pragma comment( lib, CONCAT( spa_lib_name, ".lib" ))
#   endif
#  endif
# endif
#endif

#undef BUILDING_LOCAL_FILE
#undef spa_lib_name

#endif /* DECL_EULR */