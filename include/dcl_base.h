﻿/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DECL_BASE

#if defined( __baseutil ) || defined( __SpaACIS ) || defined( __SPAAcisDs ) || defined( __baseutil_mmgr)
#define BUILDING_LOCAL_FILE
#endif

#include "importexport.h"
#include "acis/gme/base/gme_base.hxx"

#ifdef IMPORT_EXPORT_SYMBOLS
# ifdef BUILDING_LOCAL_FILE
#  define DECL_BASE EXPORT_SYMBOL
# else
#  ifndef GME_BASE_DLL
#   define DECL_BASE IMPORT_SYMBOL
#  else
#   define DECL_BASE EXPORT_SYMBOL
#  endif
# endif
#else
# define DECL_BASE
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
#    define spa_lib_name "baseutil"
#   else
#    define spa_lib_name "SpaACIS"
#   endif
#   if defined( _DEBUG ) && !defined( SPA_INTERNAL_BUILD )
#    pragma comment( lib, CONCAT( spa_lib_name, "d.lib" ) )
#   else
#    pragma comment( lib, CONCAT( spa_lib_name, ".lib" ) )
#   endif
#  endif
# endif
#endif

#undef BUILDING_LOCAL_FILE
#undef spa_lib_name

#endif /* DECL_BASE */
