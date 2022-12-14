/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   all_measurements_table.h
 * Author: mohammad
 *
 * Created on June 4, 2022, 2:40 PM
 */
#include <iostream>
#include <time.h>
#include <string.h>
#include <sstream>
#include <iomanip> 
#include <vector>
#include <mysql/mysql.h>

#ifndef ALL_MEASUREMENTS_TABLE_H
#define ALL_MEASUREMENTS_TABLE_H

class all_measurements_table {
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
    int voltage_measure = -1;
    int current_measure = -1;
    int temperature_measure = -1;
    int humidity_measure = -1;
    int alarm_state = -1;
    char alarm_types[40] = "";
    int record_index = -1;
    time_t date_time = 0;

    all_measurements_table(MYSQL *_mysql);
    virtual ~all_measurements_table();
    void Print_measurement();
    bool select_measurement_by_id_solar_pannel_id(int _id, int _solar_pannel_id);
    std::vector<all_measurements_table> select_measurement_by_solar_pannel_id(int _solar_pannel_id);
    int get_measurment_insert_id();
    int get_measurment_record_id(int solar_panel_id);
    bool insert_update_measurement();
    bool delete_measurement();
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

#endif /* ALL_MEASUREMENTS_TABLE_H */

