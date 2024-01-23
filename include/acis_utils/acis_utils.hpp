/*******************************************************************/
/*    Copyright (c) 2022-2024 by GME Team.                         */
/*    All rights reserved.                                         */
/*******************************************************************/
/*****************************************************************/
/**
 * \file   acis_utils.cpp
 * \brief  ACIS环境初始化和结束函数
 *********************************************************************/

#include "acis/kernapi.hxx"
#include "acis/license.hxx"
#include "acis/bulletin.hxx"
#include "acis/spa_unlock_result.hxx"
#include "spatial_license.h"
#include "acis/acistype.hxx"
//#include "gme/kernel/gme_kernel.hxx"
#include <string>

static inline void unlock_license() {
    spa_unlock_result out = spa_unlock_products(SPATIAL_LICENSE);
    if(out.get_state() == SPA_UNLOCK_PASS_WARN) { /* insert your warning notification code here */
        ;
    } else if(out.get_state() != SPA_UNLOCK_PASS) { /* insert your error handling code here */
        ;
    }
}

// 返回初始化级别：1-基础，2-建模引擎，0-失败。
static inline int initialize_acis() {
    outcome result;

    int level = 0;
    // 基础初始化将创建线程本地存储和线程ID。

    if(initialize_base()) {  // 在初始化ACIS时传递该默认配置
        ++level;
        outcome result = api_start_modeller(0);
        if(result.ok()) {
            ++level;
            // 在此添加通用ACIS状态。
        }
    }
    if(level < 3) unlock_license();

#if(defined(GME_KERN_API) && !defined(GME_BASE_OPTION))
    // 关闭annotation的生成(目前未实现)
    option_header* annotations_option = find_option("annotations" );
    annotations_option->push(FALSE);
#endif

    //gme_set_logging(true);
    //gme_set_default_stream(get_default_stream());
    return level;
}

// 带审计的初始化，如需使用，请在SetUp()处进行替换
static inline int initialize_acis_debug() {
    outcome result;
    int level = 0;

    base_configuration config;
    config.enable_audit_leaks = TRUE;
    config.enable_audit_logs = TRUE;

    // 每次运行测试用例后可在Debug/mmgr.log或Release/mmgr.log处查看

    if(initialize_base(&config)) {  // 在初始化ACIS时传递该默认配置
        ++level;
        outcome result = api_start_modeller(0);
        if(result.ok()) {
            ++level;
        }
    }
    if(level < 3) unlock_license();

    //gme_set_logging(true);
    //gme_set_default_stream(get_default_stream());
    return level;
}

static inline void terminate_acis(int level) {
    //gme_set_logging(false);
    //gme_set_default_stream(nullptr);
    //gme_null_stream_root();

    // 终止ACIS
    if(level > 1) {
        // 终止之前必须先销毁该outcome对象，因为访问了线程本地存储。
        outcome result = api_stop_modeller();
        if(result.ok() != FALSE) {
            --level;
        }
    }

    if(level > 0) {
        if(terminate_base()) {
            --level;
        }
    }
}

static inline std::string process(outcome& result) {
    if(!result.ok()) {
        std::string err(find_err_mess(result.error_number()));
        std::string errW(err.begin(), err.end());
        return errW;
    }
    err_mess_type* warnings = nullptr;
    int nwarn = get_warnings(warnings);
    for(int i = 0; i < nwarn; ++i) {
        printf("Warning %d : %s\n", warnings[i], find_err_mess(warnings[i]));
    }
    return "";
}