/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_ACT_ICON_H
#define DM_ACT_ICON_H

#include "dm_default_icon.hxx"  // base class DM_default_icon  // base class DM_default_icon

/**
* @file dmacticon.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMICONAPI
 *
 * @{
 */

// Header for the DM_act_icon class.
//
// Purpose: The DM_act_icon class is used for drawing area constraint tag
// objects in deformable modeling.
//
// Description: The DM_act_icon is part of the dmicon library. This
// icon uses a DM_dbx_icon to depict an area constraint on a deformable surface.
//
// Related classes: The DM_def_icon_cmd_args and the DM_def_icon_query_args
// encapsulate many of the methods inhereted from the DM_default_icon interface.
// Thus the Set_state and Query methods can be used to perform casting, when
// different libraries of icons are mixed.
// The DM_def_icon_draw_args and the DM_def_icon_cmd_args can be broadcast to icons
// using the Deformable Modeling interface methods, such as DM_draw_all_icons.
// A DM_act_icon owns a DM_dbx_icon, providing an example of icon aggregation.
//
class DS_dmod;
class DM_dbx_icon;
class DM_default_icon_factory;
class DM_dbl_array;
/**
 * This class is used for drawing area constraint tag objects in deformable modeling.
 * <br>
 * <b>Role:</b> The <tt>DM_act_icon</tt> is part of the <tt>dmicon</tt> library. This icon uses a
 * <tt>DM_dbx_icon</tt> to depict an area constraint on a deformable surface.
 * <br><br>
 * <b>Related classes:</b> The <tt>DM_def_icon_cmd_args</tt> and the <tt>DM_def_icon_query_args</tt>
 * encapsulate many of the methods inherited from the <tt>DM_default_icon</tt> interface.
 * Thus the <tt>Set_state</tt> and <tt>Query</tt> methods can be used to perform casting, when different
 * libraries of icons are mixed. The <tt>DM_def_icon_draw_args</tt> and the <tt>DM_def_icon_cmd_args</tt>
 * can be broadcast to icons using the deformable modeling interface methods, such as
 * <tt>DM_draw_all_icons</tt>. A <tt>DM_act_icon</tt> owns a <tt>DM_dbx_icon</tt>, providing an example of icon
 * aggregation.
 *<br><br>
 * @see DM_dbx_icon
 */

class DECL_DM_ICON DM_act_icon : public DM_default_icon { // act

 private:

// Users should not call assignment operator or copy constructor -
// use Make_copy and Set_owner
   DM_act_icon&
   operator=                            // Assignment Operator
   (const DM_act_icon&);

   DM_dbx_icon* m_dbx_icon;

 protected:

  /**
   * Copy contructor.
   * <br><br>
   * <b>Role:</b> Should not call copy contructor; call <tt>Make_copy</tt> and <tt>Set_owner</tt> instead.
   */
   DM_act_icon(const DM_act_icon&) ;  // should not call copy constructor
                                      // call Make_copy & Set_owner
   virtual ~DM_act_icon();            // should call Lose

 public:
/**
 * Public constructor.
 */
    DM_act_icon();                                   // public constructor

/**
 * Draws this icon.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param draw_cmd
 * draw-command object.
 */
   virtual void Draw(                               // draw this icon
                  Spatial_abs_hurler& err_handler,             // error handler
                  const DM_icon_draw_args& draw_cmd)const;   // draw-command object

/**
 * Notification of owning tag object state change.
 * <br><br>
 * @param err_handler
 * error handler.
 */
   virtual void Tag_object_changed(                 // notification of owning tag object state change
                             Spatial_abs_hurler& err_handler);  // error handler

/**
 * Clone method.
 * <br><br>
 * @param err_handler
 * error handler.
 */
   virtual DM_icon* Make_copy(                      // clone method
                      Spatial_abs_hurler& err_handler)const;    // error handler
/**
 * Updates cached data.
 */
    virtual void Validate();                         // update cached data

// Override these to propagate to owned icons.
/**
 * Notification of owner completion, for icon initialization.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param new_dmod
 * icon owner ID.
 * @param new_tag
 * icon owner ID.
 */
   virtual void Set_owner(                          // of owner completion, for icon initialization
                   Spatial_abs_hurler& err_handler,             // error handler
                   DS_dmod* new_dmod,int new_tag);  // icon owner pair
/**
 * Sets discretization grid density.
 * <br><br>
 * @param density
 * density.
 */
   virtual void Set_grid(const int density[2]);             // set discretization grid density
/**
 * Queries discretization grid density.
 * <br><br>
 * @param density
 * density.
 */

   virtual void Get_grid(int density [2])const;              // query discretization grid density
/**
 * Sets icon draw width.
 * <br><br>
 * @param width
 * width.
 */
   virtual void Set_icon_width(double width);             // set icon draw width
/**
 * Sets draw option.
 * <br><br>
 * @param option
 * option.
 */
   virtual void Set_draw_option(int option);               // set draw options
/**
 * Marks cached data as bad.
 */
    virtual void Invalidate();                       // mark cahced data bad
/**
 * Queries color data.
 * <br><br>
 * @param arr_data
 * data array.
 */
   virtual void Get_colors(DM_dbl_array& arr_data) const;    // query color data
/**
 * Sets color data.
 * <br><br>
 * @param col_data
 * color data (r,g,b).
 * @param arr_size
 * array size,number of doubles.
 */

   virtual void Set_colors(const double* col_data,int arr_size);      // set color data
};
 /** @} */
#endif /*DM_ACT_ICON_H*/

