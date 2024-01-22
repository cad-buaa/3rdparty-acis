/* ORIGINAL: 3dt2.1/scmext/avh_utl.hxx */
// $Id: avh_utl.hxx,v 1.5 2001/01/11 18:18:08 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


// Advanced rendering husk includes
#include "render.hxx"
// ywoo 08Jan01: added the header file.
#include "scheme.hxx"
// ywoo: end

ScmObject make_Scm_Render_Arg(
	const char *arg_name, 
	Render_Arg &arg_value
	);
