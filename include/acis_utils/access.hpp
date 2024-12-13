/*******************************************************************/
/*    Copyright (c) 2022-2024 by GME Team.                         */
/*    All rights reserved.                                         */
/*******************************************************************/
/*****************************************************************/
/**
 * \file   access.hpp
 * \brief  ACIS模型存取
 *
 * \author Shivelino
 * \date   June 2024
 *********************************************************************/
#pragma once
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>

#include "acis/errorbase.hxx"
#include "acis/fileinfo.hxx"
#include "acis/kernapi.hxx"
#include "acis/lists.hxx"

enum class ACCESS_VERSION { R2, R5, R7, R32 };

template <ACCESS_VERSION version = ACCESS_VERSION::R2> void acis_api_set_version() {
#define _VERSION_CASE(_num)        \
    case ACCESS_VERSION::R##_num:  \
        api_save_version(_num, 0); \
        break

    switch(version) {
        _VERSION_CASE(2);
        _VERSION_CASE(5);
        _VERSION_CASE(7);
        _VERSION_CASE(32);  // newest
        default:
            break;
    }

#undef _VERSION_CASE
}

template <ACCESS_VERSION version = ACCESS_VERSION::R2> void acis_api_save_entity_list(const char* fname, const ENTITY_LIST& elist) {
    API_NOP_BEGIN;
    acis_api_set_version<version>();

    FileInfo fileinfo;
    fileinfo.set_units(1.0);
    fileinfo.set_product_id("acis_api_access");
    result = api_set_file_info((FileIdent | FileUnits), fileinfo);
    result = api_set_int_option("sequence_save_files", 1);

    FILE* save_file = fopen(fname, "wb");
    result = api_save_entity_list(save_file, true, elist);
    fclose(save_file);

    API_NOP_END
}

template <ACCESS_VERSION version = ACCESS_VERSION::R2> void acis_api_save_entity(const char* fname, ENTITY* entity) {
    acis_api_save_entity_list<version>(fname, ENTITY_LIST(entity));
}

template <ACCESS_VERSION version = ACCESS_VERSION::R2> void acis_api_restore_entity_list(const char* fname, ENTITY_LIST& elist) {
    API_BEGIN;
    acis_api_set_version<version>();

    FILE* save_file = fopen(fname, "rb");
    result = api_restore_entity_list(save_file, true, elist);
    fclose(save_file);

    API_END;
}

template <ACCESS_VERSION version = ACCESS_VERSION::R2> void acis_api_restore_entity(const char* fname, ENTITY*& entity) {
    ENTITY_LIST el;
    acis_api_restore_entity_list<version>(fname, el);
    entity = el[0];
}
