// $Id: rs_opts.hxx,v 1.1 2002/07/11 14:41:26 btomas Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    declare procedures for working with class resurface_options in scheme
//
//----------------------------------------------------------------------

#ifndef rs_typ_hxx
#define rs_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class resurface_options;

//======================================================================

logical is_Scm_Resurface_Opt(ScmObject);
resurface_options *get_Scm_Resurface_Opt(ScmObject);
ScmObject make_Scm_Resurface_Opt(resurface_options *eq);

//======================================================================
#endif