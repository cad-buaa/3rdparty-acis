/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( POINT_IN_FACE_ENUM )
#define POINT_IN_FACE_ENUM

/**
* @file ptfcenum.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup INTRAPICONTAINMENT
 *
 * @{
 */
/**
* Specifies whether a point lies inside, outside, or on a face.
* @param point_unknown_face
* unknown
* @param point_inside_face
* point lies inside face
* @param point_boundary_face
* point lies on face
* @param point_outside_face
* point lies outside face
* @param point_unset_face
* point has not yet been tested. Not for external use.
**/

enum point_face_containment {
	point_unknown_face,
	point_inside_face,
	point_boundary_face,
	point_outside_face,
    point_unset_face
};

/**
 * 指明一个面是位于Body内部、外部或边界。
 * @param face_unknown 未知
 * @param face_inside 在Body内部
 * @param face_boundary_double_double 在Body边界上，且自身和重叠面都是DOUBLE_SIDED
 * @param face_boundary_double_single 在Body边界上，且自身是DOUBLE_SIDED，重叠面是SINGLE_SIDED
 * @param face_boundary_single_double 在Body边界上，且自身是SINGLE_SIDED，重叠面是DOUBLE_SIDED
 * @param face_boundary_single_single_same_normal 在Body边界上，且自身和重叠面都是SINGLE_SIDED，且两个面的法向量相同
 * @param face_boundary_single_single_oppo_normal 在Body边界上，且自身和重叠面都是SINGLE_SIDED，且两个面的法向量相反
 * @param face_outside 在Body外部
 */
enum face_containment{
        face_unknown,
        face_inside,
        face_boundary_double_double,
        face_boundary_double_single,
        face_boundary_single_double,
        face_boundary_single_single_same_normal,
        face_boundary_single_single_oppo_normal,
        face_outside,

};

/**
 * 指明一个edge是位于Body内部、外部或边界。
 */
enum edge_containment{
        edge_unknown,
        edge_inside,
        edge_boundary,
        edge_outside
};

// Result for classify_loop_box.

// tbrv
/**
 * @nodoc
 */

enum region_type {
	region_unknown,
	region_bounded,
	region_unbounded,
	region_multiple
};

/** @} */
/**
 * \addtogroup ACISQUERIES
 *
 * @{
 */
/**
* Specifies the type of a loop classified by api_loop_type.
* @param loop_unknown
* Unable to determine loop type.
* @param loop_periphery
* Closed loop bounding "outside" of face.
* @param loop_hole
* Closed loop bounding interior "hole" in face.
* @param loop_separation
* Not Used
* @param loop_u_separation
* Loop which crosses the "v" seam on a surface, an odd
* number of times, separating the surface in the u direction.
* @param loop_v_separation
* Loop which crosses the "u" seam on a surface, an odd
* number of times, separating the surface in the v direction.
* @param loop_uv_separation
* Loop which crosses both the "v"  and "u" seams on a
*surface, an odd number of times,  separating the surface in both the u and v
* directions.
**/
enum loop_type {
	loop_unknown,
	loop_periphery,
	loop_hole,
	loop_separation,
	loop_u_separation,
	loop_v_separation,
	loop_uv_separation
};

#define is_loop_separation(a)  (a==loop_separation || a==loop_u_separation || a==loop_v_separation || a==loop_uv_separation)
/** @} */
#endif
