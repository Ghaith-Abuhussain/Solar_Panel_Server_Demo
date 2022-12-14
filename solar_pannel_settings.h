/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   solar_pannel_settings.h
 * Author: mohammad
 *
 * Created on June 4, 2022, 1:44 PM
 */
#include <iostream>
#include <time.h>
#include <string.h>
#include <sstream>
#include <iomanip> 
#include <vector>
#include <mysql/mysql.h>

#ifndef SOLAR_PANNEL_SETTINGS_H
#define SOLAR_PANNEL_SETTINGS_H

class solar_pannel_settings {
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
    int solar_pannel_id = -1;
    int min_current = -1;
    int max_current = -1;
    int min_voltage = -1;
    int max_voltage = -1;
    int min_temperature = -1;
    int max_temperature = -1;
    int min_humidity = -1;
    int max_humidity = -1;
    char server_ip[40] = "";
    int server_port = -1;
    time_t date_time = 0;
    
    solar_pannel_settings(MYSQL *_mysql);
    virtual ~solar_pannel_settings();
    void Print_solar_pannel_settings();
    bool select_solar_pannel_setting_by_id_solar_pannel_id(int _id, int _solar_pannel_id);
    bool select_solar_pannel_setting_by_solar_pannel_id(int _solar_pannel_id);
    bool insert_update_solar_pannel_setting();
    bool delete_solar_pannel_setting();
    std::string to_String_X8(unsigned int v);
    std::string to_String_X4(int v);
    std::string to_String_X2(int v);
    std::string to_String_00(int v);
    char* StringToByteArray(std::string hex);
    std::string int2HexStrReverse2Byte(int v);
    std::string calcLength(int length);
    std::string Print_time(time_t now);
    time_t string_to_time_t(const char *date);
    int get_solar_pannel_setting_insert_id();

private:

};

#endif /* SOLAR_PANNEL_SETTINGS_H */

