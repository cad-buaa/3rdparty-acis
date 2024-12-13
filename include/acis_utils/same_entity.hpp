/*******************************************************************/
/*    Copyright (c) 2022-2024 by GME Team.                         */
/*    All rights reserved.                                         */
/*******************************************************************/
/*****************************************************************/
/**
 * \file   same_entity.hpp
 * \brief  判断两个body是否在拓扑和几何意义上等价
 *
 * \author 黄浩勉
 * \date   June 2022
 *********************************************************************/
#pragma once
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>

#include "acis/acistol.hxx"
#include "acis/alltop.hxx"
#include "acis/boolapi.hxx"
#include "acis/cucuint.hxx"
#include "acis/curdef.hxx"
#include "acis/curve.hxx"
#include "acis/cusfint.hxx"
#include "acis/insanity_list.hxx"
#include "acis/intdef.hxx"
#include "acis/intrapi.hxx"
#include "acis/intsfsf.hxx"
#include "acis/kernapi.hxx"
#include "acis/point.hxx"
#include "acis/q_edge.hxx"
#include "acis/sfsfint.hxx"
#include "acis/sps3crtn.hxx"
#include "acis/surdef.hxx"
#include "acis/surface.hxx"
#include "acis/transfrm.hxx"

inline bool same_entity(const ENTITY* entity1, const ENTITY* entity2, bool check_topology = true, bool check_direction = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline bool same_body(const BODY* body1, const BODY* body2, bool check_topology = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline bool same_lump(const LUMP* lump1, const LUMP* lump2, bool check_topology = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline bool same_wire(const WIRE* wire1, const WIRE* wire2, bool check_topology = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline bool same_shellsame_shell(const SHELL* shell1, const SHELL* shell2, bool check_topology = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline bool same_shell(const SHELL* shell1, const SHELL* shell2, bool check_topology = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline bool same_face(const FACE* face1, const FACE* face2, bool check_topology = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline bool same_loop(const LOOP* loop1, const LOOP* loop2, bool check_topology = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline bool same_coedge(const COEDGE* coedge1, const COEDGE* coedge2, bool check_topology = true, bool check_direction = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline bool same_edge(const EDGE* edge1, const EDGE* edge2, bool check_topology = true, bool check_direction = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline bool same_vertex(const VERTEX* vertex1, const VERTEX* vertex2, bool check_topology = true, ENTITY_LIST* ill_entity_list = nullptr, ENTITY_LIST* failed_entity_list = nullptr);
inline void deal_with_insanity_data(insanity_data* data, std::string& err_info, ENTITY*& ill_entity);
inline bool same_curve_curve_int(const curve_curve_int* inter1, const curve_curve_int* inter2, double tol = SPAresabs);
inline bool same_bs3_curve(const bs3_curve cur1, const bs3_curve cur2, double tol = SPAresabs);
inline bool same_curve_surf_int(const curve_surf_int* gme_inters, const curve_surf_int* acis_inters, double tol = SPAresabs);
inline logical TestIntRel(const surf_surf_int* ssi_acis, const surf_surf_int* ssi_gme);
inline bool same_surf_surf_int(const surf_surf_int* gme_inters, const surf_surf_int* acis_inters, double tol = SPAresabs);
inline void get_illegal_faces(BODY* body_gme, BODY* body_acis, ENTITY_LIST& illegal_faces1, ENTITY_LIST& illegal_faces2);

/*
 * @note 比较两个给定的实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param entity1
 * 给定的第一个entity实体
 * @param entity2
 * 给定的第二个entity实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param check_direction
 * 若给定的实体为edge或者coedge，是否考虑它们的方向
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_entity(const ENTITY* entity1, const ENTITY* entity2, bool check_topology, bool check_direction, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(entity1 == nullptr || entity2 == nullptr) {
        if(entity1 == nullptr && entity2 == nullptr) {
            return true;
        }

        return false;
    }
    std::string name1 = entity1->type_name();
    std::string name2 = entity2->type_name();
    // 两个entity的类型不同
    if(name1 != name2) {
        return false;
    }
    std::map<std::string, int> entity_type_map = {
      {"body",   1},
      {"lump",   2},
      {"shell",  3},
      {"face",   4},
      {"wire",   5},
      {"loop",   6},
      {"coedge", 7},
      {"edge",   8},
      {"vertex", 9},
    };
    int entity_type = entity_type_map[name1];
    switch(entity_type) {
        case 1:
            return same_body((BODY*)entity1, (BODY*)entity2, check_topology, ill_entity_list);
        case 2:
            return same_lump((LUMP*)entity1, (LUMP*)entity2, check_topology, ill_entity_list);
        case 3:
            return same_shell((SHELL*)entity1, (SHELL*)entity2, check_topology, ill_entity_list);
        case 4:
            return same_face((FACE*)entity1, (FACE*)entity2, check_topology, ill_entity_list);
        case 5:
            return same_wire((WIRE*)entity1, (WIRE*)entity2, check_topology, ill_entity_list);
        case 6:
            return same_loop((LOOP*)entity1, (LOOP*)entity2, check_topology, ill_entity_list);
        case 7:
            return same_coedge((COEDGE*)entity1, (COEDGE*)entity2, check_topology, check_direction, ill_entity_list);
        case 8:
            return same_edge((EDGE*)entity1, (EDGE*)entity2, check_topology, check_direction, ill_entity_list);
        case 9:
            return same_vertex((VERTEX*)entity1, (VERTEX*)entity2, check_topology, ill_entity_list);
        // 未实现的类型
        default:
            return false;
    }
}

/*
 * @note 比较两个给定的body实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param entity1
 * 给定的第一个body实体
 * @param entity2
 * 给定的第二个body实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_body(const BODY* body1, const BODY* body2, bool check_topology, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(body1 == nullptr || body2 == nullptr) {
        if(body1 == nullptr && body2 == nullptr)
            return true;
        else
            return false;
    }

    // 检查几何与拓扑合法性
    if(check_topology) {
        insanity_list* insanity_list1 = nullptr;
        insanity_list* insanity_list2 = nullptr;
        api_check_entity(body1, insanity_list1);
        api_check_entity(body2, insanity_list2);
        bool out_only1 = true;
        bool out_only2 = true;
        if(insanity_list1 != nullptr || insanity_list2 != nullptr) {
            while(insanity_list1 != nullptr) {
                insanity_data* data = insanity_list1->data();
                if(data && data->get_type() == ERROR_TYPE && data->get_insane_id() != 50036) {
                    out_only1 = false;
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list1 = insanity_list1->next();
            }
            while(insanity_list2 != nullptr) {
                insanity_data* data = insanity_list2->data();
                if(data && data->get_type() == ERROR_TYPE && data->get_insane_id() != 50036) {
                    out_only2 = false;
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list2 = insanity_list2->next();
            }
            if(!(out_only1 && out_only2)) return false;
        }
    }
    if(body1->transform() != nullptr || body2->transform() != nullptr) {
        if(body1->transform() != nullptr && body2->transform() != nullptr) {
            if(body1->transform()->transform() != body2->transform()->transform()) {
                if(failed_entity_list != nullptr) {
                    failed_entity_list->add((BODY*)body1);
                }
                return false;
            }
        }
        // 有一个body不存在变换
        else {
            if((body1->transform() == nullptr && !body2->transform()->identity()) || (body2->transform() == nullptr && !body1->transform()->identity())) {
                return false;
            }
        }
    }
    LUMP* lump1 = body1->lump();
    LUMP* lump2 = body2->lump();
    while(lump1 != nullptr && lump2 != nullptr) {
        lump1 = lump1->next();
        lump2 = lump2->next();
    }
    // body中lump的数量不一致
    if(lump1 != nullptr || lump2 != nullptr) {
        return false;
    }
    lump1 = body1->lump();
    lump2 = body2->lump();
    std::unordered_set<LUMP*> traversed_lump;
    while(lump1 != nullptr) {
        lump2 = body2->lump();
        bool temp_res = false;
        while(lump2 != nullptr) {
            if(traversed_lump.count(lump2) == 0) {
                // 上层检查拓扑后，下层不用再检查
                temp_res = same_lump(lump1, lump2, false);
                if(temp_res == true) {
                    traversed_lump.insert(lump2);
                    break;
                }
            }
            lump2 = lump2->next();
        }
        if(temp_res == false) {
            if(failed_entity_list != nullptr) {
                failed_entity_list->add(lump1);
            }
            return false;
        }
        lump1 = lump1->next();
    }
    return true;
}

/*
 * @note 比较两个给定的lump实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param entity1
 * 给定的第一个lump实体
 * @param entity2
 * 给定的第二个lump实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_lump(const LUMP* lump1, const LUMP* lump2, bool check_topology, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(lump1 == nullptr || lump2 == nullptr) {
        if(lump1 == nullptr && lump2 == nullptr)
            return true;
        else
            return false;
    }
    // 检查几何与拓扑合法性
    if(check_topology) {
        insanity_list* insanity_list1 = nullptr;
        insanity_list* insanity_list2 = nullptr;
        api_check_entity(lump1, insanity_list1);
        api_check_entity(lump2, insanity_list2);
        if(insanity_list1 != nullptr || insanity_list2 != nullptr) {
            while(insanity_list1 != nullptr) {
                insanity_data* data = insanity_list1->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list1 = insanity_list1->next();
            }
            while(insanity_list2 != nullptr) {
                insanity_data* data = insanity_list2->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list2 = insanity_list2->next();
            }
            return false;
        }
    }
    SHELL* shell1 = lump1->shell();
    SHELL* shell2 = lump2->shell();
    while(shell1 != nullptr && shell2 != nullptr) {
        shell1 = shell1->next();
        shell2 = shell2->next();
    }
    // lump中shell的数量不一致
    if(shell1 != nullptr || shell2 != nullptr) {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add((LUMP*)lump1);
        }
        return false;
    }
    shell1 = lump1->shell();
    shell2 = lump2->shell();
    std::unordered_set<SHELL*> traversed_shell;
    while(shell1 != nullptr) {
        shell2 = lump2->shell();
        bool temp_res = false;
        while(shell2 != nullptr) {
            if(traversed_shell.count(shell2) == 0) {
                temp_res = same_shell(shell1, shell2, false);
                if(temp_res == true) {
                    traversed_shell.insert(shell2);
                    break;
                }
            }
            shell2 = shell2->next();
        }
        if(temp_res == false) {
            if(failed_entity_list != nullptr) {
                failed_entity_list->add((SHELL*)shell1);
            }
            return false;
        }
        shell1 = shell1->next();
    }
    return true;
}

/*
 * @note 比较两个给定的wire实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param entity1
 * 给定的第一个wire实体
 * @param entity2
 * 给定的第二个wire实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_wire(const WIRE* wire1, const WIRE* wire2, bool check_topology, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(wire1 == nullptr || wire2 == nullptr) {
        if(wire1 == nullptr && wire2 == nullptr)
            return true;
        else
            return false;
    }
    // 检查几何与拓扑合法性
    if(check_topology) {
        insanity_list* insanity_list1 = nullptr;
        insanity_list* insanity_list2 = nullptr;
        api_check_entity(wire1, insanity_list1);
        api_check_entity(wire2, insanity_list2);
        if(insanity_list1 != nullptr || insanity_list2 != nullptr) {
            while(insanity_list1 != nullptr) {
                insanity_data* data = insanity_list1->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list1 = insanity_list1->next();
            }
            while(insanity_list2 != nullptr) {
                insanity_data* data = insanity_list2->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list2 = insanity_list2->next();
            }
            return false;
        }
    }
    ENTITY_LIST edge_list1, edge_list2;
    api_get_edges((ENTITY*)wire1, edge_list1);
    api_get_edges((ENTITY*)wire2, edge_list2);
    if(edge_list1.count() != edge_list2.count()) {
        ENTITY* entity1 = nullptr;
        ENTITY* entity2 = nullptr;
        api_deep_down_copy_entity((ENTITY*)wire1, entity1);
        api_deep_down_copy_entity((ENTITY*)wire2, entity2);
        WIRE* new_wire1 = (WIRE*)entity1;
        WIRE* new_wire2 = (WIRE*)entity2;
        api_clean_entity(new_wire1);
        api_clean_entity(new_wire2);
        edge_list1.clear();
        edge_list2.clear();
        api_get_edges((ENTITY*)new_wire1, edge_list1);
        api_get_edges((ENTITY*)new_wire2, edge_list2);
    }
    if(edge_list1.count() != edge_list2.count()) {
        return false;
    }
    std::vector<EDGE*> acis_edges_array;
    std::vector<EDGE*> gme_edges_array;
    for(int i = 0; i < edge_list1.count(); i++) {
        acis_edges_array.push_back((EDGE*)edge_list1[i]);
    }
    for(int i = 0; i < edge_list2.count(); i++) {
        gme_edges_array.push_back((EDGE*)edge_list2[i]);
    }
    std::vector<int> acis_failed_edges_array;
    for(int i = 0; i < acis_edges_array.size(); i++) {
        EDGE* acis_edge = acis_edges_array[i];
        bool res = false;
        for(int j = 0; j < gme_edges_array.size(); j++) {
            EDGE* gme_edge = gme_edges_array[j];
            res = same_edge(acis_edge, gme_edge, check_topology, false);
            if(res) {
                break;
            }
        }
        if(!res) {
            acis_failed_edges_array.push_back(i);
        }
    }
    if(acis_failed_edges_array.size() != 0) {
        return false;
    }
    std::vector<int> gme_failed_edges_array;
    for(int i = 0; i < gme_edges_array.size(); i++) {
        EDGE* gme_edge = gme_edges_array[i];
        bool res = false;
        for(int j = 0; j < acis_edges_array.size(); j++) {
            EDGE* acis_edge = acis_edges_array[j];
            res = same_edge(acis_edge, gme_edge, check_topology, false);
            if(res) {
                break;
            }
        }
        if(!res) {
            gme_failed_edges_array.push_back(i);
        }
    }
    if(gme_failed_edges_array.size() != 0) {
        return false;
    }
    // ENTITY_LIST coedge_list1, coedge_list2;
    // api_get_coedges((ENTITY*)wire1, coedge_list1);
    // api_get_coedges((ENTITY*)wire2, coedge_list2);
    //// wire中coedge的数量不一致
    // if(coedge_list1.count() != coedge_list2.count()) {
    //    // 判断edge是否可以合并
    //    ENTITY_LIST edge_list1, edge_list2;
    //    api_get_edges((ENTITY*)wire1, edge_list1);
    //    api_get_edges((ENTITY*)wire2, edge_list2);
    //    std::vector<EDGE*> edge_array1;
    //    std::vector<EDGE*> edge_array2;
    //    bool has_isolate = false;
    //    for(int i = 0; i < edge_list1.count(); i++) {
    //        ENTITY* edge_copy = nullptr;
    //        api_deep_down_copy_entity(edge_list1[i], edge_copy);
    //        EDGE* edge = (EDGE*)edge_copy;
    //        if(edge->geometry() == nullptr) {
    //            has_isolate = true;
    //        }
    //        edge_array1.push_back((EDGE*)edge);
    //    }
    //    for(int i = 0; i < edge_list2.count(); i++) {
    //        ENTITY* edge_copy = nullptr;
    //        api_deep_down_copy_entity(edge_list2[i], edge_copy);
    //        EDGE* edge = (EDGE*)edge_copy;
    //        if(edge->geometry() == nullptr) {
    //            has_isolate = true;
    //        }
    //        edge_array2.push_back((EDGE*)edge);
    //    }
    //    if(!has_isolate) {
    //        // 顺着遍历一次，再倒着遍历一次
    //        for(int i = 0; i < edge_array1.size() - 1; i++) {
    //            EDGE* edge1 = edge_array1[i];
    //            EDGE* edge2 = edge_array1[i + 1];
    //            // 构造求交包围盒
    //            SPAposition min_pt1, max_pt1;
    //            SPAposition min_pt2, max_pt2;
    //            SPAposition min_pt, max_pt;
    //            api_get_entity_box(edge1, min_pt1, max_pt1);
    //            api_get_entity_box(edge2, min_pt2, max_pt2);
    //            min_pt.set_x(std::min(min_pt1.x(), min_pt2.x()));
    //            min_pt.set_y(std::min(min_pt1.y(), min_pt2.y()));
    //            min_pt.set_z(std::min(min_pt1.z(), min_pt2.z()));
    //            max_pt.set_x(std::max(max_pt1.x(), max_pt2.x()));
    //            max_pt.set_y(std::max(max_pt1.y(), max_pt2.y()));
    //            max_pt.set_z(std::max(max_pt1.z(), max_pt2.z()));
    //            double x_length = max_pt.x() - min_pt.x();
    //            double y_length = max_pt.y() - min_pt.y();
    //            double z_length = max_pt.z() - min_pt.z();
    //            SPAinterval x_b = SPAinterval(min_pt.x() - 0.1 * x_length, max_pt.x() + 0.1 * x_length);
    //            SPAinterval y_b = SPAinterval(min_pt.y() - 0.1 * y_length, max_pt.y() + 0.1 * y_length);
    //            SPAinterval z_b = SPAinterval(min_pt.z() - 0.1 * z_length, max_pt.z() + 0.1 * z_length);
    //            SPAbox box = SPAbox(x_b, y_b, z_b);
    //            if(edge2->geometry()->type_name() == edge1->geometry()->type_name()) {
    //                curve* curve1 = edge1->geometry()->trans_curve();
    //                curve* curve2 = edge2->geometry()->trans_curve();
    //                if(edge2->start_pos() == edge1->end_pos()) {
    //                    curve_curve_int* info = int_cur_cur(*curve1, *curve2, box, SPAresabs);
    //                    if(info->high_rel == cur_cur_coin) {
    //                        edge1->set_end(edge2->end());
    //                        edge1->set_param_range(*(SPAinterval*)nullptr);
    //                        edge_array1.erase(edge_array1.begin() + i + 1);
    //                        i = i - 1;
    //                    }
    //                } else if(edge2->end_pos() == edge1->start_pos()) {
    //                    curve_curve_int* info = int_cur_cur(*curve1, *curve2, box, SPAresabs);
    //                    if(info->high_rel == cur_cur_coin) {
    //                        edge1->set_start(edge2->start());
    //                        edge1->set_param_range(*(SPAinterval*)nullptr);
    //                        edge_array1.erase(edge_array1.begin() + i + 1);
    //                        i = i - 1;
    //                    }
    //                }
    //            }
    //        }
    //        for(int i = 0; i < edge_array2.size() - 1; i++) {
    //            EDGE* edge1 = edge_array2[i];
    //            EDGE* edge2 = edge_array2[i + 1];
    //            // 构造求交包围盒
    //            SPAposition min_pt1, max_pt1;
    //            SPAposition min_pt2, max_pt2;
    //            SPAposition min_pt, max_pt;
    //            api_get_entity_box(edge1, min_pt1, max_pt1);
    //            api_get_entity_box(edge2, min_pt2, max_pt2);
    //            min_pt.set_x(std::min(min_pt1.x(), min_pt2.x()));
    //            min_pt.set_y(std::min(min_pt1.y(), min_pt2.y()));
    //            min_pt.set_z(std::min(min_pt1.z(), min_pt2.z()));
    //            max_pt.set_x(std::max(max_pt1.x(), max_pt2.x()));
    //            max_pt.set_y(std::max(max_pt1.y(), max_pt2.y()));
    //            max_pt.set_z(std::max(max_pt1.z(), max_pt2.z()));
    //            double x_length = max_pt.x() - min_pt.x();
    //            double y_length = max_pt.y() - min_pt.y();
    //            double z_length = max_pt.z() - min_pt.z();
    //            SPAinterval x_b = SPAinterval(min_pt.x() - 0.1 * x_length, max_pt.x() + 0.1 * x_length);
    //            SPAinterval y_b = SPAinterval(min_pt.y() - 0.1 * y_length, max_pt.y() + 0.1 * y_length);
    //            SPAinterval z_b = SPAinterval(min_pt.z() - 0.1 * z_length, max_pt.z() + 0.1 * z_length);
    //            SPAbox box = SPAbox(x_b, y_b, z_b);
    //            if(edge2->geometry()->type_name() == edge1->geometry()->type_name()) {
    //                curve* curve1 = edge1->geometry()->trans_curve();
    //                curve* curve2 = edge2->geometry()->trans_curve();
    //                if(edge2->start_pos() == edge1->end_pos()) {
    //                    curve_curve_int* info = int_cur_cur(*curve1, *curve2, box, SPAresabs);
    //                    if(info->high_rel == cur_cur_coin) {
    //                        edge1->set_end(edge2->end());
    //                        edge1->set_param_range(*(SPAinterval*)nullptr);
    //                        edge_array2.erase(edge_array2.begin() + i + 1);
    //                        i = i - 1;
    //                    }
    //                } else if(edge2->end_pos() == edge1->start_pos()) {
    //                    curve_curve_int* info = int_cur_cur(*curve1, *curve2, box, SPAresabs);
    //                    if(info->high_rel == cur_cur_coin) {
    //                        edge1->set_start(edge2->start());
    //                        edge1->set_param_range(*(SPAinterval*)nullptr);
    //                        edge_array2.erase(edge_array2.begin() + i + 1);
    //                        i = i - 1;
    //                    }
    //                }
    //            }
    //        }
    //        if(edge_array1.size() == 1 && edge_array2.size() == 1) {
    //            EDGE* edge1 = edge_array1[0];
    //            if(edge1->start_pos() == edge1->end_pos() && edge1->start() != edge1->end()) {
    //                edge1->set_end(edge1->start());
    //                edge1->set_param_range(*(SPAinterval*)nullptr);
    //            }
    //            EDGE* edge2 = edge_array2[0];
    //            if(edge2->start_pos() == edge2->end_pos() && edge2->start() != edge2->end()) {
    //                edge2->set_end(edge2->start());
    //                edge2->set_param_range(*(SPAinterval*)nullptr);
    //            }
    //            bool res = same_edge(edge1, edge2, false, false);
    //            if(res) {
    //                return true;
    //            }
    //        }
    //    }
    //    if(failed_entity_list != nullptr) {
    //        failed_entity_list->add((WIRE*)wire1);
    //    }
    //    return false;
    //}
    // COEDGE* child1 = nullptr;
    // COEDGE* child2 = nullptr;
    // std::unordered_set<COEDGE*> traversed_coedge;
    // while(((child1 = (COEDGE*)coedge_list1.next()) != nullptr)) {
    //    bool temp_res = false;
    //    while((child2 = (COEDGE*)coedge_list2.next()) != nullptr) {
    //        if(traversed_coedge.count(child2) == 0) {
    //            // wire中比较coedge不用考虑方向
    //            if(same_coedge(child1, child2, false, false)) {
    //                traversed_coedge.insert(child2);
    //                temp_res = true;
    //                break;
    //            }
    //        }
    //    }
    //    // 某条coedge找不到对应的coedge
    //    if(temp_res == false) {
    //        if(failed_entity_list != nullptr) {
    //            failed_entity_list->add(child1);
    //        }
    //        return false;
    //    }
    //    coedge_list2.init();
    //}
    return true;
}
/*
 * @note 比较两个给定的shell实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param shell1
 * 给定的第一个shell实体
 * @param shell2
 * 给定的第二个shell实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_shellsame_shell(const SHELL* shell1, const SHELL* shell2, bool check_topology, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(shell1 == nullptr || shell2 == nullptr) {
        if(shell1 == nullptr && shell2 == nullptr)
            return true;
        else
            return false;
    }
    // 检查几何与拓扑合法性
    if(check_topology) {
        insanity_list* insanity_list1 = nullptr;
        insanity_list* insanity_list2 = nullptr;
        api_check_entity(shell1, insanity_list1);
        api_check_entity(shell2, insanity_list2);
        if(insanity_list1 != nullptr || insanity_list2 != nullptr) {
            while(insanity_list1 != nullptr) {
                insanity_data* data = insanity_list1->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list1 = insanity_list1->next();
            }
            while(insanity_list2 != nullptr) {
                insanity_data* data = insanity_list2->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list2 = insanity_list2->next();
            }
            return false;
        }
    }
    // 检查face
    FACE* face1 = shell1->face();
    FACE* face2 = shell2->face();
    while(face1 != nullptr && face2 != nullptr) {
        face1 = face1->next();
        face2 = face2->next();
    }
    // shell中face的数量不一致
    if(face1 != nullptr || face2 != nullptr) {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add((SHELL*)shell1);
        }
        return false;
    }
    face1 = shell1->face();
    face2 = shell2->face();
    std::unordered_set<FACE*> traversed_face;
    while(face1 != nullptr) {
        face2 = shell2->face();
        bool temp_res = false;
        while(face2 != nullptr) {
            if(traversed_face.count(face2) == 0) {
                temp_res = same_face(face1, face2, false);
                if(temp_res == true) {
                    traversed_face.insert(face2);
                    break;
                }
            }
            face2 = face2->next();
        }
        if(temp_res == false) {
            if(failed_entity_list != nullptr) {
                failed_entity_list->add(face1);
            }
            return false;
        }
        face1 = face1->next();
    }
    // 检查wire
    WIRE* wire1 = shell1->wire();
    WIRE* wire2 = shell2->wire();
    while(wire1 != nullptr && wire2 != nullptr) {
        wire1 = wire1->next();
        wire2 = wire2->next();
    }
    if(wire1 != nullptr || wire2 != nullptr) {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add((SHELL*)shell1);
        }
        return false;
    }
    wire1 = shell1->wire();
    wire2 = shell2->wire();
    std::unordered_set<WIRE*> traversed_wire;
    while(wire1 != nullptr) {
        wire2 = shell2->wire();
        bool temp_res = false;
        while(wire2 != nullptr) {
            if(traversed_wire.count(wire2) == 0) {
                temp_res = same_wire(wire1, wire2, false);
                if(temp_res == true) {
                    traversed_wire.insert(wire2);
                    break;
                }
            }
            wire2 = wire2->next();
        }
        if(temp_res == false) {
            if(failed_entity_list != nullptr) {
                failed_entity_list->add(wire1);
            }
            return false;
        }
        wire1 = wire1->next();
    }
    return true;
}

/*
 * @note 比较两个给定的shell实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param entity1
 * 给定的第一个shell实体
 * @param entity2
 * 给定的第二个shell实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_shell(const SHELL* shell1, const SHELL* shell2, bool check_topology, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(shell1 == nullptr || shell2 == nullptr) {
        if(shell1 == nullptr && shell2 == nullptr)
            return true;
        else
            return false;
    }
    // 检查几何与拓扑合法性
    if(check_topology) {
        insanity_list* insanity_list1 = nullptr;
        insanity_list* insanity_list2 = nullptr;
        api_check_entity(shell1, insanity_list1);
        api_check_entity(shell2, insanity_list2);
        if(insanity_list1 != nullptr || insanity_list2 != nullptr) {
            while(insanity_list1 != nullptr) {
                insanity_data* data = insanity_list1->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list1 = insanity_list1->next();
            }
            while(insanity_list2 != nullptr) {
                insanity_data* data = insanity_list2->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list2 = insanity_list2->next();
            }
            return false;
        }
    }
    // 检查face
    FACE* face1 = shell1->face();
    FACE* face2 = shell2->face();
    while(face1 != nullptr && face2 != nullptr) {
        face1 = face1->next();
        face2 = face2->next();
    }
    // shell中face的数量不一致
    if(face1 != nullptr || face2 != nullptr) {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add((SHELL*)shell1);
        }
        return false;
    }
    face1 = shell1->face();
    face2 = shell2->face();
    std::unordered_set<FACE*> traversed_face;
    while(face1 != nullptr) {
        face2 = shell2->face();
        bool temp_res = false;
        while(face2 != nullptr) {
            if(traversed_face.count(face2) == 0) {
                temp_res = same_face(face1, face2, false);
                if(temp_res == true) {
                    traversed_face.insert(face2);
                    break;
                }
            }
            face2 = face2->next();
        }
        if(temp_res == false) {
            if(failed_entity_list != nullptr) {
                failed_entity_list->add(face1);
            }
            return false;
        }
        face1 = face1->next();
    }
    // 检查wire
    WIRE* wire1 = shell1->wire();
    WIRE* wire2 = shell2->wire();
    while(wire1 != nullptr && wire2 != nullptr) {
        wire1 = wire1->next();
        wire2 = wire2->next();
    }
    if(wire1 != nullptr || wire2 != nullptr) {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add((SHELL*)shell1);
        }
        return false;
    }
    wire1 = shell1->wire();
    wire2 = shell2->wire();
    std::unordered_set<WIRE*> traversed_wire;
    while(wire1 != nullptr) {
        wire2 = shell2->wire();
        bool temp_res = false;
        while(wire2 != nullptr) {
            if(traversed_wire.count(wire2) == 0) {
                temp_res = same_wire(wire1, wire2, false);
                if(temp_res == true) {
                    traversed_wire.insert(wire2);
                    break;
                }
            }
            wire2 = wire2->next();
        }
        if(temp_res == false) {
            if(failed_entity_list != nullptr) {
                failed_entity_list->add(wire1);
            }
            return false;
        }
        wire1 = wire1->next();
    }
    return true;
}

/*
 * @note 比较两个给定的face实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param entity1
 * 给定的第一个face实体
 * @param entity2
 * 给定的第二个face实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_face(const FACE* face1, const FACE* face2, bool check_topology, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(face1 == nullptr || face2 == nullptr) {
        if(face1 == nullptr && face2 == nullptr)
            return true;
        else
            return false;
    }
    // 检查几何与拓扑合法性
    if(check_topology) {
        insanity_list* insanity_list1 = nullptr;
        insanity_list* insanity_list2 = nullptr;
        api_check_entity(face1, insanity_list1);
        api_check_entity(face2, insanity_list2);
        if(insanity_list1 != nullptr || insanity_list2 != nullptr) {
            while(insanity_list1 != nullptr) {
                insanity_data* data = insanity_list1->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list1 = insanity_list1->next();
            }
            while(insanity_list2 != nullptr) {
                insanity_data* data = insanity_list2->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list2 = insanity_list2->next();
            }
            return false;
        }
    }
    LOOP* loop1 = face1->loop();
    LOOP* loop2 = face2->loop();
    // 检查loop的数量是否一致
    while(loop1 != nullptr && loop2 != nullptr) {
        loop1 = loop1->next();
        loop2 = loop2->next();
    }
    // loop的数量不一致
    if(loop1 != nullptr || loop2 != nullptr) {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add((FACE*)face1);
        }
        return false;
    }
    // 检查几何是否一致
    if(face1->geometry() != nullptr || face2->geometry() != nullptr) {
        if(face1->geometry() != nullptr && face2->geometry() != nullptr) {
            surface* tool_surface = face1->geometry()->trans_surface();
            surface* blank_surface = face2->geometry()->trans_surface();
            SPAposition min_pt1, max_pt1;
            SPAposition min_pt2, max_pt2;
            SPAposition min_pt, max_pt;
            // 构造求交包围盒
            api_get_entity_box((FACE*)face1, min_pt1, max_pt1);
            api_get_entity_box((FACE*)face2, min_pt2, max_pt2);
            min_pt.set_x(std::max(min_pt1.x(), min_pt2.x()));
            min_pt.set_y(std::max(min_pt1.y(), min_pt2.y()));
            min_pt.set_z(std::max(min_pt1.z(), min_pt2.z()));
            max_pt.set_x(std::min(max_pt1.x(), max_pt2.x()));
            max_pt.set_y(std::min(max_pt1.y(), max_pt2.y()));
            max_pt.set_z(std::min(max_pt1.z(), max_pt2.z()));
            double x_length = max_pt.x() - min_pt.x();
            double y_length = max_pt.y() - min_pt.y();
            double z_length = max_pt.z() - min_pt.z();
            SPAinterval x_b = SPAinterval(min_pt.x() - 0.1 * x_length, max_pt.x() + 0.1 * x_length);
            SPAinterval y_b = SPAinterval(min_pt.y() - 0.1 * y_length, max_pt.y() + 0.1 * y_length);
            SPAinterval z_b = SPAinterval(min_pt.z() - 0.1 * z_length, max_pt.z() + 0.1 * z_length);
            SPAbox box = SPAbox(x_b, y_b, z_b);
            surf_surf_int* surf_surf_info;
            if(strcmp(tool_surface->type_name(), "lawsur-spline") == 0 && strcmp(blank_surface->type_name(), "lawsur-spline") == 0) {
                surf_surf_info = int_surf_surf(*tool_surface, *blank_surface, 2 * SPAresabs, box);
            } else {
                surf_surf_info = int_surf_surf(*tool_surface, *blank_surface, SPAresabs, box);
            }
            ACIS_DELETE tool_surface;
            ACIS_DELETE blank_surface;
            // 不相交
            if(surf_surf_info == nullptr) {
                if(failed_entity_list != nullptr) {
                    failed_entity_list->add((FACE*)face1);
                }
                delete_surf_surf_ints(surf_surf_info);
                return false;
            }
            // 有多个相交元素
            if(surf_surf_info->next != nullptr) {
                if(failed_entity_list != nullptr) {
                    failed_entity_list->add((FACE*)face1);
                }
                delete_surf_surf_ints(surf_surf_info);
                return false;
            }
            // 不重合
            if(surf_surf_info->left_surf_rel[0] != surf_coincident) {
                if(failed_entity_list != nullptr) {
                    failed_entity_list->add((FACE*)face1);
                }
                delete_surf_surf_ints(surf_surf_info);
                return false;
            }
            delete_surf_surf_ints(surf_surf_info);
        } else {
            if(failed_entity_list != nullptr) {
                failed_entity_list->add((FACE*)face1);
            }
            return false;
        }
    }
    loop1 = face1->loop();
    loop2 = face2->loop();
    std::unordered_set<LOOP*> traversed_loop;
    while(loop1 != nullptr) {
        loop2 = face2->loop();
        bool temp_res = false;
        while(loop2 != nullptr) {
            if(traversed_loop.count(loop2) == 0) {
                temp_res = same_loop(loop1, loop2, false);
                if(temp_res == true) {
                    // 该face2中的loop找到了一个对应的face1的loop，之后的遍历略过该loop
                    traversed_loop.insert(loop2);
                    break;
                }
            }
            loop2 = loop2->next();
        }
        // 某个face1中的loop在face2中无法找到对应的loop
        if(temp_res == false) {
            if(failed_entity_list != nullptr) {
                failed_entity_list->add(loop1);
            }
            return false;
        }
        loop1 = loop1->next();
    }
    return true;
}

/*
 * @note 比较两个给定的loop实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param entity1
 * 给定的第一个loop实体
 * @param entity2
 * 给定的第二个loop实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_loop(const LOOP* loop1, const LOOP* loop2, bool check_topology, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(loop1 == nullptr || loop2 == nullptr) {
        if(loop1 == nullptr && loop2 == nullptr)
            return true;
        else
            return false;
    }
    // 检查loop拓扑
    if(check_topology) {
        insanity_list* insanity_list1 = nullptr;
        insanity_list* insanity_list2 = nullptr;
        api_check_entity(loop1, insanity_list1);
        api_check_entity(loop2, insanity_list2);
        if(insanity_list1 != nullptr || insanity_list2 != nullptr) {
            while(insanity_list1 != nullptr) {
                insanity_data* data = insanity_list1->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list1 = insanity_list1->next();
            }
            while(insanity_list2 != nullptr) {
                insanity_data* data = insanity_list2->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list2 = insanity_list2->next();
            }
            return false;
        }
    }
    COEDGE* child1 = loop1->start();
    COEDGE* child2 = loop2->start();
    COEDGE* start_coedge1 = child1;
    COEDGE* start_coedge2 = child2;
    // 找到两个loop对应的起始edge
    bool find_start_coedge = false;
    if(same_coedge(child1, child2, false)) {
        find_start_coedge = true;
    } else {
        child2 = child2->next();
        while(child2 != start_coedge2) {
            if(same_coedge(child1, child2, false)) {
                find_start_coedge = true;
                break;
            }
            child2 = child2->next();
        }
    }
    if(!find_start_coedge) {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add(start_coedge1);
        }
        return false;
    }
    start_coedge1 = child1;
    start_coedge2 = child2;
    child1 = child1->next();
    child2 = child2->next();
    bool res = true;
    while(child1 != start_coedge1 && child2 != start_coedge2) {
        res = same_coedge(child1, child2, false);
        if(!res) {
            if(failed_entity_list != nullptr) {
                failed_entity_list->add(child1);
            }
            return false;
        }
        child1 = child1->next();
        child2 = child2->next();
    }
    // 环的遍历不同时结束（环中coedge的数量不一致）
    if(child1 != start_coedge1 || child2 != start_coedge2) {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add((LOOP*)loop1);
        }
        return false;
    }
    return true;
}

/*
 * @note 比较两个给定的coedge实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param entity1
 * 给定的第一个coedge实体
 * @param entity2
 * 给定的第二个coedge实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param check_direction
 * 比较的时候是否考虑coedge的方向
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_coedge(const COEDGE* coedge1, const COEDGE* coedge2, bool check_topology, bool check_direction, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(coedge1 == nullptr || coedge2 == nullptr) {
        if(coedge1 == nullptr && coedge2 == nullptr)
            return true;
        else
            return false;
    }
    // 检查几何与拓扑合法性
    if(check_topology) {
        insanity_list* insanity_list1 = nullptr;
        insanity_list* insanity_list2 = nullptr;
        api_check_entity(coedge1, insanity_list1);
        api_check_entity(coedge2, insanity_list2);
        if(insanity_list1 != nullptr || insanity_list2 != nullptr) {
            while(insanity_list1 != nullptr) {
                insanity_data* data = insanity_list1->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list1 = insanity_list1->next();
            }
            while(insanity_list2 != nullptr) {
                insanity_data* data = insanity_list2->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list2 = insanity_list2->next();
            }
            return false;
        }
    }
    int closed1 = coedge1->edge()->closed();
    int closed2 = coedge2->edge()->closed();
    if(closed1 != closed2) {
        return false;
    }
    if(!closed1 && !closed2) {
        SPAposition coedge1_start_pos = coedge1->start()->geometry()->coords();
        SPAposition coedge2_start_pos = coedge2->start()->geometry()->coords();
        SPAposition coedge1_end_pos = coedge1->end()->geometry()->coords();
        SPAposition coedge2_end_pos = coedge2->end()->geometry()->coords();
        double point_tol = SPAresabs;
        // 如果是intcurve，考虑拟合容差值
        if((coedge1->edge()->geometry() != nullptr && strcmp(coedge1->edge()->geometry()->type_name(), "intcurve") == 0) || (coedge2->edge()->geometry() != nullptr && strcmp(coedge2->edge()->geometry()->type_name(), "intcurve") == 0)) {
            /*const intcurve* int1 = (const intcurve*)&coedge1->edge()->geometry()->equation();
            const intcurve* int2 = (const intcurve*)&coedge2->edge()->geometry()->equation();
            double length1 = coedge1->edge()->length();
            double length2 = coedge2->edge()->length();
            double tol1 = std::max(int1->fitol(), int1->fitol() / length1);
            double tol2 = std::max(int2->fitol(), int2->fitol() / length2);
            if(tol1 > 0) {
                point_tol += tol1;
            }
            if(tol2 > 0) {
                point_tol += tol2;
            }*/
            point_tol = SPAresfit;
        }
        if(!same_point(coedge1_start_pos, coedge2_start_pos, point_tol) || !same_point(coedge1_end_pos, coedge2_end_pos, point_tol)) {
            if(check_direction) {
                if(failed_entity_list != nullptr) {
                    failed_entity_list->add((COEDGE*)coedge1);
                }
                return false;
            } else {
                // 检查两条coedge是否方向相反
                if(!same_point(coedge1_start_pos, coedge2_end_pos, point_tol) || !same_point(coedge1_end_pos, coedge2_start_pos, point_tol)) {
                    if(failed_entity_list != nullptr) {
                        failed_entity_list->add((COEDGE*)coedge1);
                    }
                    return false;
                }
            }
        }
    }
    EDGE* edge1 = coedge1->edge();
    EDGE* edge2 = coedge2->edge();
    bool res = same_edge(edge1, edge2, false, false);
    if(res == false) {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add(edge1);
        }
    }
    return res;
}

/*
 * @note 比较两个给定的edge实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param entity1
 * 给定的第一个edge实体
 * @param entity2
 * 给定的第二个edge实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param direction
 * 比较的时候是否考虑edge的方向
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_edge(const EDGE* edge1, const EDGE* edge2, bool check_topology, bool check_direction, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(edge1 == nullptr || edge2 == nullptr) {
        if(edge1 == nullptr && edge2 == nullptr)
            return true;
        else
            return false;
    }
    // 检查几何与拓扑合法性
    if(check_topology) {
        insanity_list* insanity_list1 = nullptr;
        insanity_list* insanity_list2 = nullptr;
        api_check_entity(edge1, insanity_list1);
        api_check_entity(edge2, insanity_list2);
        if(insanity_list1 != nullptr || insanity_list2 != nullptr) {
            while(insanity_list1 != nullptr) {
                insanity_data* data = insanity_list1->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list1 = insanity_list1->next();
            }
            while(insanity_list2 != nullptr) {
                insanity_data* data = insanity_list2->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list2 = insanity_list2->next();
            }
            return false;
        }
    }
    // 检查端点情况
    bool vertex_check = false;
    int closed1 = edge1->closed();
    int closed2 = edge2->closed();
    if(closed1 != closed2) {
        return false;
    }
    if(!closed1 && !closed2) {
        SPAposition edge1_start_pos = edge1->start()->geometry()->coords();
        SPAposition edge2_start_pos = edge2->start()->geometry()->coords();
        SPAposition edge1_end_pos = edge1->end()->geometry()->coords();
        SPAposition edge2_end_pos = edge2->end()->geometry()->coords();
        double point_tol = SPAresabs;
        // 如果是intcurve，考虑拟合容差
        if((edge1->geometry() != nullptr && strcmp(edge1->geometry()->type_name(), "intcurve") == 0) || (edge2->geometry() != nullptr && strcmp(edge2->geometry()->type_name(), "intcurve") == 0)) {
            /*const intcurve* int1 = (const intcurve*)&edge1->geometry()->equation();
            const intcurve* int2 = (const intcurve*)&edge2->geometry()->equation();
            double length1 = edge1->length();
            double length2 = edge2->length();
            double tol1 = std::max(int1->fitol(), int1->fitol() / length1);
            double tol2 = std::max(int2->fitol(), int2->fitol() / length2);
            if(tol1 > 0) {
                point_tol += tol1;
            }
            if(tol2 > 0) {
                point_tol += tol2;
            }*/
            point_tol = SPAresfit;
        }
        if(same_point(edge1_start_pos, edge2_start_pos, point_tol) && same_point(edge1_end_pos, edge2_end_pos, point_tol)) {
            vertex_check = true;
        }
        if(!check_direction) {
            if((same_point(edge1_start_pos, edge2_end_pos, point_tol) && same_point(edge1_end_pos, edge2_start_pos, point_tol))) {
                vertex_check = true;
            }
        }
    } else if(closed1 && closed2) {
        vertex_check = true;
    }
    if(vertex_check) {
        // 孤立点情况
        if(edge1->geometry() == nullptr && edge2->geometry() == nullptr) {
            return true;
        } else if(edge1->geometry() == nullptr || edge2->geometry() == nullptr) {
            if(failed_entity_list != nullptr) {
                failed_entity_list->add((EDGE*)edge1);
            }
            return false;
        }
        // 检查edge的拓扑和几何是否合法
        curve_curve_int* curve_curve_info = nullptr;
        curve_curve_int* test_curve_curve_info = nullptr;
        api_inter_ed_ed((EDGE*)edge1, (EDGE*)edge2, curve_curve_info);  // 不能解耦，受限于模块依赖关系
        api_intersect_curves((EDGE*)edge1, (EDGE*)edge2, true, test_curve_curve_info);
        if(curve_curve_info != nullptr) {
            if(curve_curve_info->high_rel == cur_cur_coin) {
                delete_curve_curve_ints(curve_curve_info);
                delete_curve_curve_ints(test_curve_curve_info);
                return true;
            }
        }
        if(test_curve_curve_info != nullptr) {
            if(test_curve_curve_info->high_rel == cur_cur_coin) {
                delete_curve_curve_ints(curve_curve_info);
                delete_curve_curve_ints(test_curve_curve_info);
                return true;
            }
        }
        delete_curve_curve_ints(curve_curve_info);
        delete_curve_curve_ints(test_curve_curve_info);
        if(failed_entity_list != nullptr) {
            failed_entity_list->add((EDGE*)edge1);
        }
    } else {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add(edge1->start());
        }
    }
    return false;
}

/*
 * @note 比较两个给定的vertex实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param entity1
 * 给定的第一个vertex实体
 * @param entity2
 * 给定的第二个vertex实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
inline bool same_vertex(const VERTEX* vertex1, const VERTEX* vertex2, bool check_topology, ENTITY_LIST* ill_entity_list, ENTITY_LIST* failed_entity_list) {
    if(vertex1 == nullptr || vertex2 == nullptr) {
        if(vertex1 == nullptr && vertex2 == nullptr)
            return true;
        else
            return false;
    }
    // 检查几何与拓扑合法性
    if(check_topology) {
        insanity_list* insanity_list1 = nullptr;
        insanity_list* insanity_list2 = nullptr;
        api_check_entity(vertex1, insanity_list1);
        api_check_entity(vertex2, insanity_list2);
        if(insanity_list1 != nullptr || insanity_list2 != nullptr) {
            while(insanity_list1 != nullptr) {
                insanity_data* data = insanity_list1->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list1 = insanity_list1->next();
            }
            while(insanity_list2 != nullptr) {
                insanity_data* data = insanity_list2->data();
                if(data && data->get_type() == ERROR_TYPE) {
                    std::string err_info;
                    ENTITY* ill_entity = nullptr;
                    deal_with_insanity_data(data, err_info, ill_entity);
                    if(ill_entity_list != nullptr && ill_entity != nullptr) {
                        ill_entity_list->add(ill_entity);
                    }
                }
                insanity_list2 = insanity_list2->next();
            }
            return false;
        }
    }
    if(vertex1->geometry()->coords() != vertex2->geometry()->coords()) {
        if(failed_entity_list != nullptr) {
            failed_entity_list->add((VERTEX*)vertex1);
        }
        return false;
    }
    return true;
}

/*
 * @note 将错误信息处理为字符串并从控制台输出，同时返回给调用者
 * @return API执行的结果
 * @param data
 * 错误信息
 * @param err_info
 * 返回的字符串
 * @param ill_entity
 * 返回的不合法的entity
 */
inline void deal_with_insanity_data(insanity_data* data, std::string& err_info, ENTITY*& ill_entity) {
    std::map<std::string, int> entity_type_map = {
      {"body",   1},
      {"lump",   2},
      {"shell",  3},
      {"face",   4},
      {"wire",   5},
      {"loop",   6},
      {"coedge", 7},
      {"edge",   8},
      {"vertex", 9},
    };
    ENTITY* entity = data->get_ent();
    std::string entity_type_name = entity->type_name();
    int entity_type = entity_type_map[entity_type_name];
    err_info = data->get_message();
    switch(entity_type) {
        case 1: {
            err_info = "BODY error: " + err_info;
            std::cout << err_info << std::endl;
            BODY* body = (BODY*)entity;
            ill_entity = entity;
            break;
        }
        case 2: {
            err_info = "LUMP error: " + err_info;
            std::cout << err_info << std::endl;
            LUMP* lump = (LUMP*)entity;
            ill_entity = entity;
            break;
        }
        case 3: {
            err_info = "SHELL error: " + err_info;
            std::cout << err_info << std::endl;
            SHELL* shell = (SHELL*)entity;
            ill_entity = entity;
            break;
        }
        case 4: {
            err_info = "FACE error: " + err_info;
            std::cout << err_info << std::endl;
            FACE* face = (FACE*)entity;
            ill_entity = entity;
            break;
        }
        case 5: {
            err_info = "WIRE error: " + err_info;
            std::cout << err_info << std::endl;
            WIRE* wire = (WIRE*)entity;
            ill_entity = entity;
            break;
        }
        case 6: {
            err_info = "LOOP error: " + err_info;
            std::cout << err_info << std::endl;
            LOOP* loop = (LOOP*)entity;
            ill_entity = entity;
            break;
        }
        case 7: {
            err_info = "COEDGE error: " + err_info;
            std::cout << err_info << std::endl;
            COEDGE* coedge = (COEDGE*)entity;
            ill_entity = entity;
            break;
        }
        case 8: {
            err_info = "EDGE error: " + err_info;
            std::cout << err_info << std::endl;
            EDGE* edge = (EDGE*)entity;
            ill_entity = entity;
            break;
        }
        case 9: {
            err_info = "VERTEX error: " + err_info;
            std::cout << err_info << std::endl;
            VERTEX* vertex = (VERTEX*)entity;
            ill_entity = entity;
            break;
        }
        default:
            ill_entity = entity;
            break;
    }
}

/*
 * @note 比较两个给定的求交结果是否一致
 * @return API执行的结果
 * @param
 * 给定的第一个求交结果
 * @param entity2
 * 给定的第二个求交结果
 */
inline bool same_curve_curve_int(const curve_curve_int* inter1, const curve_curve_int* inter2, double tol) {
    const curve_curve_int* tmp1 = inter1;
    const curve_curve_int* tmp2 = inter2;
    while(tmp1 && tmp2) {
        if(fabs(tmp1->int_point.x() - tmp2->int_point.x()) >= tol || fabs(tmp1->int_point.y() - tmp2->int_point.y()) >= tol || fabs(tmp1->int_point.z() - tmp2->int_point.z()) >= tol || fabs(tmp1->param1 - tmp2->param1) >= tol ||
           fabs(tmp1->param2 - tmp2->param2) >= tol || tmp1->uv_set != tmp2->uv_set) {
            return false;
        }
        if(tmp1->uv_set != 0) {
            if(tmp1->uv != tmp2->uv) {
                return false;
            }
        }
        tmp1 = tmp1->next;
        tmp2 = tmp2->next;
    }
    if(tmp1 != nullptr || tmp2 != nullptr) {
        return false;
    }
    return true;
}

/*
 * @note 比较两个bs3_curve是否一致
 * @return API执行的结果
 * @param cur1 第一个bs3_curve
 * @param cur2 第二个bs3_curve
 */
inline bool same_bs3_curve(const bs3_curve cur1, const bs3_curve cur2, double tol) {
    if(cur1 == nullptr || cur2 == nullptr) {
        if(cur1 == nullptr && cur2 == nullptr)
            return true;
        else
            return false;
    }
    int dim1 = 0, dim2 = 0, deg1 = 0, deg2 = 0, nums_ctrlpts1 = 0, nums_ctrlpts2 = 0, num_knots1 = 0, num_knots2 = 0;
    SPAposition* ctrlpts1 = nullptr;
    SPAposition* ctrlpts2 = nullptr;
    double* weights1 = nullptr;
    double* weights2 = nullptr;
    double* knots1 = nullptr;
    double* knots2 = nullptr;
    logical rat1 = 0;
    logical rat2 = 0;

    bs3_curve_to_array(cur1, dim1, deg1, rat1, nums_ctrlpts1, ctrlpts1, weights1, num_knots1, knots1);
    bs3_curve_to_array(cur2, dim2, deg2, rat2, nums_ctrlpts2, ctrlpts2, weights2, num_knots2, knots2);
    if(dim1 != dim2) return false;
    if(deg1 != deg2) return false;
    if(rat1 != rat2) return false;
    if(nums_ctrlpts1 != nums_ctrlpts2) return false;
    for(int i = 0; i < nums_ctrlpts1; i++) {
        if(!same_point(ctrlpts1[i], ctrlpts2[i])) return false;
        if(rat1 == true) {
            if(abs(weights1[i] - weights2[i]) > tol) return false;
        }
    }
    if(num_knots1 != num_knots2) return false;
    for(int i = 0; i < num_knots1; i++) {
        if(abs(knots1[i] - knots2[i]) > tol) return false;
    }

    ACIS_DELETE[] ctrlpts1;
    ACIS_DELETE[] ctrlpts2;
    ACIS_DELETE[] weights1;
    ACIS_DELETE[] weights2;
    ACIS_DELETE[] knots1;
    ACIS_DELETE[] knots2;
    return true;
}

/*
 * @note 比较两个给定的求交结果是否一致
 * @return API执行的结果
 * @param	gme_inters
 * 给定的GME求交结果
 * @param	acis_inters
 * 给定的ACIS求交结果
 */
inline bool same_curve_surf_int(const curve_surf_int* gme_inters, const curve_surf_int* acis_inters, double tol) {
    // 交点参数对比
    while(gme_inters && acis_inters) {
        SPAposition gme_int = gme_inters->int_point;
        SPAposition acis_int = acis_inters->int_point;
        if(fabs(gme_int.x() - acis_int.x()) < SPAresabs && fabs(gme_int.y() - acis_int.y()) < SPAresabs && fabs(gme_int.z() - acis_int.z()) < SPAresabs && fabs(gme_inters->param - acis_inters->param) < SPAresabs &&
           fabs(gme_inters->surf_param.v - acis_inters->surf_param.v) < SPAresabs && fabs(gme_inters->surf_param.u - acis_inters->surf_param.u) < SPAresabs) {  // 比较gme和acis的每个交点
            if(gme_inters->high_rel == acis_inters->high_rel && gme_inters->low_rel == acis_inters->low_rel && gme_inters->tolerance == acis_inters->tolerance) {
            } else {
                return false;
            }
        } else {
            return false;
        }
        gme_inters = gme_inters->next;
        acis_inters = acis_inters->next;
    }
    if(gme_inters != nullptr || acis_inters != nullptr) {
        return false;
    }
    return true;
}

inline logical TestIntRel(const surf_surf_int* ssi_acis, const surf_surf_int* ssi_gme) {
    if(ssi_acis->int_type == ssi_gme->int_type && ssi_acis->left_surf_rel[0] == ssi_gme->left_surf_rel[0] && ssi_acis->left_surf_rel[1] == ssi_gme->left_surf_rel[1] && ssi_acis->right_surf_rel[0] == ssi_gme->right_surf_rel[0] &&
       ssi_acis->right_surf_rel[1] == ssi_gme->right_surf_rel[1]) {
        return TRUE;
    }
    return FALSE;
}

/*
 * @note 比较两个给定的求交结果是否一致
 * @return API执行的结果
 * @param	gme_inters
 * 给定的GME求交结果
 * @param	acis_inters
 * 给定的ACIS求交结果
 */
inline bool same_surf_surf_int(const surf_surf_int* gme_inters, const surf_surf_int* acis_inters, double tol) {
    // 交点参数对比
    while(gme_inters && acis_inters) {
        curve* gme_int = gme_inters->cur;
        curve* acis_int = acis_inters->cur;
        curve_curve_int* cci = int_cur_cur(*gme_int, *acis_int);
        SPAinterval interval1 = gme_inters->cur->param_range();   // 曲线1的参数范围
        SPAinterval interval2 = acis_inters->cur->param_range();  // 曲线2的参数范围
        // 自由曲线重合判断
        if(cci && cci->next && cci->next->next == nullptr && (cci->high_rel == curve_curve_rel::cur_cur_coin || cci->low_rel == curve_curve_rel::cur_cur_coin) &&
           (cci->next->high_rel == curve_curve_rel::cur_cur_coin || cci->next->low_rel == curve_curve_rel::cur_cur_coin) && is_equal(cci->param1, interval1.start_pt()) && is_equal(cci->next->param1, interval1.end_pt()) &&
           TestIntRel(gme_inters, acis_inters)) {
        } else {
            return false;
        }
        gme_inters = gme_inters->next;
        acis_inters = acis_inters->next;
    }
    if(gme_inters != nullptr || acis_inters != nullptr) {
        return false;
    }
    return true;
}

/*
 * @note 获取两个给定的face实体无法match的面
 * @return API执行的结果
 * @param body_gme
 * 给定的第一个face实体，一般为GME结果
 * @param body_acis
 * 给定的第二个face实体，一般为ACIS参照
 * @param illegal_faces1
 * 获取body_gme无法match body_acis的面
 * @param illegal_faces2
 * 获取body_acis无法match body_gme的面
 */
inline void get_illegal_faces(BODY* body_gme, BODY* body_acis, ENTITY_LIST& illegal_faces1, ENTITY_LIST& illegal_faces2) {
    ENTITY_LIST gme_faces, acis_faces;
    api_get_faces(body_gme, gme_faces);
    api_get_faces(body_acis, acis_faces);
    for(auto f1: gme_faces) {
        FACE* gme_face = (FACE*)f1;
        bool face_found = false;
        for(auto f2: acis_faces) {
            FACE* acis_face = (FACE*)f2;
            if(same_face(gme_face, acis_face)) {
                face_found = true;
                acis_faces.remove(f2);
                break;
            }
        }
        if(!face_found) illegal_faces1.add(gme_face);
    }
    for(auto f: acis_faces) {
        illegal_faces2.add(f);
    }
}
