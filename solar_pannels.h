/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   solar_pannels.h
 * Author: mohammad
 *
 * Created on June 4, 2022, 11:52 AM
 */
#include <iostream>
#include <time.h>
#include <string.h>
#include <sstream>
#include <iomanip> 
#include <vector>
#include <mysql/mysql.h>

#ifndef SOLAR_PANNELS_H
#define SOLAR_PANNELS_H

class solar_pannels {
public:
    MYSQL         *mysql;
    MYSQL_STMT    *stmt;
    MYSQL_BIND    bind_param[20];
    MYSQL_BIND    bind[20];
    MYSQL_RES     *prepare_meta_result;
    unsigned long length[20];
    my_bool       is_null[20];
    
    MYSQL_TIME    created_t;
    MYSQL_TIME    deleted_t;
    MYSQL_TIME    updated_t;
    
    
    int id = -1;
    char solar_pannel_imei[40] = "";
    int current_measure = -1;
    int voltage_measure = -1;
    int temperature_measure = -1;
    int humidity_measure = -1;
    int state = -1;
    time_t date_time = 0;
    
    solar_pannels(MYSQL *_mysql);
    virtual ~solar_pannels();
    void Print_solar_pannel();
    bool select_solar_pannel_by_id(int _id);
    bool select_solar_pannel_by_imei(std::string _imei);
    bool insert_update_solar_pannel();
    bool delete_solar_pannel();
    std::vector<solar_pannels> read_all_solar_pannels(MYSQL *mysql);
    std::string to_String_X8(unsigned int v);
    std::string to_String_X4(int v);
    std::string to_String_X2(int v);
    std::string to_String_00(int v);
    char* StringToByteArray(std::string hex);
    std::string int2HexStrReverse2Byte(int v);
    std::string calcLength(int length);
    std::string Print_time(time_t now);
    time_t string_to_time_t(const char *date);
    int get_solar_pannels_insert_id();
private:

};

#endif /* SOLAR_PANNELS_H */

