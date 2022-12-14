/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   users.h
 * Author: mohammad
 *
 * Created on June 3, 2022, 2:44 PM
 */
#include <iostream>
#include <time.h>
#include <string.h>
#include <sstream>
#include <iomanip> 
#include <vector>
#include <mysql/mysql.h>

#ifndef USERS_H
#define USERS_H

class users {
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
    char username[64] = "";
    char password[255] = "";
    time_t date_time = 0;
    
    
    users(MYSQL *_mysql);
    virtual ~users();
    
    void Print_user();
    bool select_user_by_name(std::string _username);
    bool insert_update_user();
    bool delete_user();
    std::string to_String_X8(unsigned int v);
    std::string to_String_X4(int v);
    std::string to_String_X2(int v);
    std::string to_String_00(int v);
    char* StringToByteArray(std::string hex);
    std::string int2HexStrReverse2Byte(int v);
    std::string calcLength(int length);
    std::string Print_time(time_t now);
private:

};

#endif /* USERS_H */

