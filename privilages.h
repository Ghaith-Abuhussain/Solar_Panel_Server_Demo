/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   privilages.h
 * Author: mohammad
 *
 * Created on June 4, 2022, 11:08 AM
 */
#include <iostream>
#include <time.h>
#include <string.h>
#include <sstream>
#include <iomanip> 
#include <vector>
#include <mysql/mysql.h>


#ifndef PRIVILAGES_H
#define PRIVILAGES_H

class privilages {
public:
    MYSQL         *mysql;
    MYSQL_STMT    *stmt;
    MYSQL_BIND    bind_param[5];
    MYSQL_BIND    bind[5];
    MYSQL_RES     *prepare_meta_result;
    unsigned long length[5];
    my_bool       is_null[5];
    
    MYSQL_TIME    created_t;
    MYSQL_TIME    deleted_t;
    MYSQL_TIME    updated_t;
    
    
    int id = -1;
    int user_id = -1;
    int priv_type;
    time_t date_time = 0;
    
    privilages(MYSQL *_mysql);
    virtual ~privilages();
    
    void Print_privilage();
    bool select_privilage_by_id_userid(int _id, int _userid);
    bool insert_update_privilage();
    bool delete_privilage();
    std::vector<privilages> get_all_user_privilages(int user_id);
    std::string to_String_X8(unsigned int v);
    std::string to_String_X4(int v);
    std::string to_String_X2(int v);
    std::string to_String_00(int v);
    char* StringToByteArray(std::string hex);
    std::string int2HexStrReverse2Byte(int v);
    std::string calcLength(int length);
    std::string Print_time(time_t now);
    time_t string_to_time_t(const char *date);
private:

};

#endif /* PRIVILAGES_H */

