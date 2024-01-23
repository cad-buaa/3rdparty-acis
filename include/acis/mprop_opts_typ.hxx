// $Id: adm_scmopts.hxx,v 1.1 2002/02/08 23:54:58 btomas Exp $
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
//    declare procedures for working with class adm_options in scheme
//
//----------------------------------------------------------------------

#ifndef mprop_opts_typ_hxx
#define mprop_opts_typ_hxx

#include "scheme.hxx"
#include "logical.h"
#include "mprop.hxx"

class mass_props;


//======================================================================

logical is_Scm_Mass_Props_Options(ScmObject);
mass_props_options* get_Scm_Mass_Props_Options(ScmObject);
ScmObject make_Scm_Mass_Props_Options(mass_props_options* mpo);

//======================================================================
#endif // mprop_opts_typ_hxx