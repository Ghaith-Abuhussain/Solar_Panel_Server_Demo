/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   solar_pannels.cpp
 * Author: mohammad
 * 
 * Created on June 4, 2022, 11:52 AM
 */

#include "solar_pannels.h"

solar_pannels::solar_pannels(MYSQL *_mysql) {
    mysql = _mysql;
}

solar_pannels::~solar_pannels() {
}

void solar_pannels::Print_solar_pannel()
{
    std::cout << "-------------------------\n";
    std::cout << "id = " << id << std::endl;
    std::cout << "solar_pannel_imei = " << solar_pannel_imei << std::endl;
    std::cout << "current_measure = " << current_measure << std::endl;
    std::cout << "voltage_measure = " << voltage_measure << std::endl;
    std::cout << "temperature_measure = " << temperature_measure << std::endl;
    std::cout << "humidity_measure = " << humidity_measure << std::endl;
    std::cout << "state = " << state << std::endl;
    std::cout << "datetime = " << Print_time(date_time) << std::endl;
}

int solar_pannels::get_solar_pannels_insert_id()
{
    MYSQL_RES *result;
    int num_fields = 0;
    int max_id = -1;
    try
    {
        MYSQL_RES *result;
        int num_fields = 0;
        std::string statment = "SELECT MAX(id) FROM solar_pannels";
        if (mysql_query(mysql, statment.c_str()))
        {
            return -1;
        }
    }
    catch(...)
    {
        return -1;
    }
    
    try
    {
        result = mysql_store_result(mysql);
        if (result == NULL)
        {
            return -1;
        }
    }
    catch(...)
    {
        return -1;
    }
    
    try
    {
        num_fields = mysql_num_fields(result);
    }
    catch(...)
    {
        return -1;
    }
    
    try
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)))
        {
            max_id = row[0] ? std::stoi(row[0]) : 0;
        }
        mysql_next_result(mysql);
    }
    catch(...)
    {
        return -1;
    }
    
    try
    {
        mysql_free_result(result);
    }
    catch(...)
    {
        return -1;
    }
    
    return (max_id + 1);
}

bool solar_pannels::select_solar_pannel_by_id(int _id)
{
    int id_ = -1;
    std::string statment = "SELECT *, Unix_Timestamp(date_time) FROM solar_pannels WHERE id=?";
    if (mysql != NULL)   
    {
        stmt= mysql_stmt_init(mysql);
        if (!stmt)
        {
            fprintf(stderr, " mysql_stmt_init(), out of memory\n");
            return false;
        }
        if (!mysql_stmt_prepare(stmt,statment.c_str(),strlen(statment.c_str())))
        {
            memset(bind_param, 0, sizeof(bind));

            bind_param[0].buffer_type= MYSQL_TYPE_LONG;
            bind_param[0].buffer= (char *)&id_;
            
            if (mysql_stmt_bind_param(stmt, bind_param))
            {
                fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            
            id_ = _id;
            int param_count= mysql_stmt_param_count(stmt);
            if (param_count != 1) /* validate parameter count */
            {
                fprintf(stderr, " invalid parameter count returned by MySQL\n");
                return false;
            }

            /* Fetch result set meta information */
            prepare_meta_result = mysql_stmt_result_metadata(stmt);
            if (!prepare_meta_result)
            {
                fprintf(stderr, " mysql_stmt_result_metadata(), returned no meta information\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            int column_count= mysql_num_fields(prepare_meta_result);
            
            if (column_count != 9) /* validate column count */
            {
                fprintf(stderr, " invalid column count returned by MySQL\n");
                return false;
            }
            
            if (mysql_stmt_execute(stmt))
            {
                fprintf(stderr, " mysql_stmt_execute(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            memset(bind, 0, sizeof(bind));
            memset(is_null, 0, sizeof(is_null));

            bind[0].buffer_type= MYSQL_TYPE_LONG;
            bind[0].buffer= (char *)&id;
            bind[0].is_null= &is_null[0];
            bind[0].length= &length[0];
            
            bind[1].buffer_type= MYSQL_TYPE_STRING;
            bind[1].buffer= (char *)&solar_pannel_imei;
            bind[1].buffer_length = 64;
            bind[1].is_null= &is_null[1];
            bind[1].length= &length[1];
            
            bind[2].buffer_type= MYSQL_TYPE_LONG;
            bind[2].buffer= (char *)&current_measure;
            bind[2].is_null= &is_null[2];
            bind[2].length= &length[2];
            
            bind[3].buffer_type= MYSQL_TYPE_LONG;
            bind[3].buffer= (char *)&voltage_measure;
            bind[3].is_null= &is_null[3];
            bind[3].length= &length[3];
            
            bind[4].buffer_type= MYSQL_TYPE_LONG;
            bind[4].buffer= (char *)&temperature_measure;
            bind[4].is_null= &is_null[4];
            bind[4].length= &length[4];
            
            bind[5].buffer_type= MYSQL_TYPE_LONG;
            bind[5].buffer= (char *)&humidity_measure;
            bind[5].is_null= &is_null[5];
            bind[5].length= &length[5];
            
            bind[6].buffer_type= MYSQL_TYPE_LONG;
            bind[6].buffer= (char *)&state;
            bind[6].is_null= &is_null[6];
            bind[6].length= &length[6];
            
            bind[8].buffer_type= MYSQL_TYPE_LONG;
            bind[8].buffer= (char *)&date_time;
            bind[8].is_null= &is_null[8];
            bind[8].length= &length[8];
                        
            if (mysql_stmt_bind_result(stmt, bind))
            {
              fprintf(stderr, " mysql_stmt_bind_result() failed\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              return false;
            }
            if (mysql_stmt_store_result(stmt))
            {
              fprintf(stderr, " mysql_stmt_store_result() failed solar_pannels\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              return false;
            }
            while (!mysql_stmt_fetch(stmt));
            mysql_free_result(prepare_meta_result);
            mysql_stmt_free_result(stmt);
        }
        else
        {
            return false;
        }
        mysql_stmt_close(stmt);
    }
    return true;
}

bool solar_pannels::select_solar_pannel_by_imei(std::string _imei)
{
    char imei_[40];
    std::string statment = "SELECT *, Unix_Timestamp(date_time) FROM solar_pannels WHERE solar_pannel_imei=?";
    if (mysql != NULL)   
    {
        stmt= mysql_stmt_init(mysql);
        if (!stmt)
        {
            fprintf(stderr, " mysql_stmt_init(), out of memory\n");
            return false;
        }
        if (!mysql_stmt_prepare(stmt,statment.c_str(),strlen(statment.c_str())))
        {
            memset(bind_param, 0, sizeof(bind));

            bind_param[0].buffer_type= MYSQL_TYPE_STRING;
            bind_param[0].buffer= (char *)imei_;
            bind_param[0].buffer_length= strlen(_imei.c_str());
            
            if (mysql_stmt_bind_param(stmt, bind_param))
            {
                fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            
            strcpy(imei_, _imei.c_str());
            int param_count= mysql_stmt_param_count(stmt);
            if (param_count != 1) /* validate parameter count */
            {
                fprintf(stderr, " invalid parameter count returned by MySQL\n");
                return false;
            }

            /* Fetch result set meta information */
            prepare_meta_result = mysql_stmt_result_metadata(stmt);
            if (!prepare_meta_result)
            {
                fprintf(stderr, " mysql_stmt_result_metadata(), returned no meta information\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            int column_count= mysql_num_fields(prepare_meta_result);
            
            if (column_count != 9) /* validate column count */
            {
                fprintf(stderr, " invalid column count returned by MySQL\n");
                return false;
            }
            
            if (mysql_stmt_execute(stmt))
            {
                fprintf(stderr, " mysql_stmt_execute(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            memset(bind, 0, sizeof(bind));
            memset(is_null, 0, sizeof(is_null));

            bind[0].buffer_type= MYSQL_TYPE_LONG;
            bind[0].buffer= (char *)&id;
            bind[0].is_null= &is_null[0];
            bind[0].length= &length[0];
            
            bind[1].buffer_type= MYSQL_TYPE_STRING;
            bind[1].buffer= (char *)&solar_pannel_imei;
            bind[1].buffer_length = 64;
            bind[1].is_null= &is_null[1];
            bind[1].length= &length[1];
            
            bind[2].buffer_type= MYSQL_TYPE_LONG;
            bind[2].buffer= (char *)&current_measure;
            bind[2].is_null= &is_null[2];
            bind[2].length= &length[2];
            
            bind[3].buffer_type= MYSQL_TYPE_LONG;
            bind[3].buffer= (char *)&voltage_measure;
            bind[3].is_null= &is_null[3];
            bind[3].length= &length[3];
            
            bind[4].buffer_type= MYSQL_TYPE_LONG;
            bind[4].buffer= (char *)&temperature_measure;
            bind[4].is_null= &is_null[4];
            bind[4].length= &length[4];
            
            bind[5].buffer_type= MYSQL_TYPE_LONG;
            bind[5].buffer= (char *)&humidity_measure;
            bind[5].is_null= &is_null[5];
            bind[5].length= &length[5];
            
            bind[6].buffer_type= MYSQL_TYPE_LONG;
            bind[6].buffer= (char *)&state;
            bind[6].is_null= &is_null[6];
            bind[6].length= &length[6];
            
            bind[8].buffer_type= MYSQL_TYPE_LONG;
            bind[8].buffer= (char *)&date_time;
            bind[8].is_null= &is_null[8];
            bind[8].length= &length[8];
                        
            if (mysql_stmt_bind_result(stmt, bind))
            {
              fprintf(stderr, " mysql_stmt_bind_result() failed\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              return false;
            }
            if (mysql_stmt_store_result(stmt))
            {
              fprintf(stderr, " mysql_stmt_store_result() failed solar_pannels\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              return false;
            }
            while (!mysql_stmt_fetch(stmt));
            mysql_free_result(prepare_meta_result);
            mysql_stmt_free_result(stmt);
        }
        else
        {
            return false;
        }
        mysql_stmt_close(stmt);
    }
    return true;
}

bool solar_pannels::insert_update_solar_pannel()
{
    std::string statment = "INSERT INTO `solar_pannels` (`id`, `solar_pannel_imei`, `current_measure`, `voltage_measure`, `temperature_measure`, `humidity_measure`, `state`, `date_time`) VALUES (?, ?, ?, ?, ?, ?, ?, FROM_UNIXTIME(?)) ON DUPLICATE KEY UPDATE id=VALUES(id), solar_pannel_imei=VALUES(solar_pannel_imei), current_measure=VALUES(current_measure), voltage_measure=VALUES(voltage_measure), temperature_measure=VALUES(temperature_measure), humidity_measure=VALUES(humidity_measure), state=VALUES(state), date_time=VALUES(date_time)";
    if (mysql != NULL) 
    {
        stmt= mysql_stmt_init(mysql);
        if (!stmt)
        {
            fprintf(stderr, " mysql_stmt_init(), out of memory\n");
            return false;
        }
        if (!mysql_stmt_prepare(stmt,statment.c_str(),strlen(statment.c_str())))
        {
            memset(bind, 0, sizeof(bind));
            memset(is_null, 0, sizeof(is_null));
            memset(length, 0, sizeof(length));
            
            bind[0].buffer_type= MYSQL_TYPE_LONG;
            bind[0].buffer= (char *)&id;
            bind[0].is_null= &is_null[0];
            bind[0].length= &length[0];
            
            bind[1].buffer_type= MYSQL_TYPE_STRING;
            bind[1].buffer= (char *)solar_pannel_imei;
            bind[1].buffer_length = 40;
            bind[1].is_null= &is_null[1];
            length[1] = strlen(solar_pannel_imei);
            bind[1].length= &length[1];
            
            bind[2].buffer_type= MYSQL_TYPE_LONG;
            bind[2].buffer= (char *)&current_measure;
            bind[2].is_null= &is_null[2];
            bind[2].length= &length[2];
            
            bind[3].buffer_type= MYSQL_TYPE_LONG;
            bind[3].buffer= (char *)&voltage_measure;
            bind[3].is_null= &is_null[3];
            bind[3].length= &length[3];
            
            bind[4].buffer_type= MYSQL_TYPE_LONG;
            bind[4].buffer= (char *)&temperature_measure;
            bind[4].is_null= &is_null[4];
            bind[4].length= &length[4];
            
            bind[5].buffer_type= MYSQL_TYPE_LONG;
            bind[5].buffer= (char *)&humidity_measure;
            bind[5].is_null= &is_null[5];
            bind[5].length= &length[5];
            
            bind[6].buffer_type= MYSQL_TYPE_LONG;
            bind[6].buffer= (char *)&state;
            bind[6].is_null= &is_null[6];
            bind[6].length= &length[6];
            
            bind[7].buffer_type= MYSQL_TYPE_LONG;
            bind[7].buffer= (char *)&date_time;
            is_null[7] = (date_time == -1);
            bind[7].is_null= &is_null[7];
            bind[7].length= &length[7];

            if (mysql_stmt_bind_param(stmt, bind))
            {
                fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            int param_count= mysql_stmt_param_count(stmt);
            if (param_count != 8) /* validate parameter count */
            {
                fprintf(stderr, " invalid parameter count returned by MySQL\n");
                return false;
            }
            if (mysql_stmt_execute(stmt))
            {
                fprintf(stderr, " mysql_stmt_execute(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            if (mysql_stmt_affected_rows(stmt) == 1)
            {
                //fprintf(stderr, "solar_pannel %d inserted\n", id);
            }
            if (mysql_stmt_affected_rows(stmt) != 1)
            {
                //fprintf(stderr, "solar_pannel %d updated\n", id);
            }
        }
        else
        {
            return false;
        }
        mysql_stmt_close(stmt);
    }
    return true;
}

bool solar_pannels::delete_solar_pannel()
{
    strcpy(solar_pannel_imei, "");
    current_measure = -1;
    voltage_measure = -1;
    temperature_measure = -1;
    humidity_measure = -1;
    state = -1;
    date_time = 0;
    int id_;
    std::string statment = "DELETE FROM `solar_pannels` WHERE `id`= ?";
    if (mysql != NULL)   
    {
        stmt= mysql_stmt_init(mysql);
        if (!stmt)
        {
            fprintf(stderr, " mysql_stmt_init(), out of memory\n");
            return false;
        }
        if (!mysql_stmt_prepare(stmt,statment.c_str(),strlen(statment.c_str())))
        {
            memset(bind_param, 0, sizeof(bind));
            bind_param[0].buffer_type= MYSQL_TYPE_SHORT;
            bind_param[0].buffer= (char *)&id_;
            if (mysql_stmt_bind_param(stmt, bind_param))
            {
                fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            id_ = id;
            int param_count= mysql_stmt_param_count(stmt);
            if (param_count != 1) /* validate parameter count */
            {
                fprintf(stderr, " invalid parameter count returned by MySQL\n");
                return false;
            }
            /* Fetch result set meta information */
            if (mysql_stmt_execute(stmt))
            {
                fprintf(stderr, " mysql_stmt_execute(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            if (mysql_stmt_affected_rows(stmt) != 1)
            {
                return false;
            }    
        }
        else
        {
            return false;
        }
        mysql_stmt_close(stmt);
    }
    id = -1;
    return true;
}

std::vector<solar_pannels> solar_pannels::read_all_solar_pannels(MYSQL *mysql)
{
    MYSQL_RES *result;
    int num_fields = 0;
    std::vector<solar_pannels> readed_data;
    try
    {
        MYSQL_RES *result;
        int num_fields = 0;
        if (mysql_query(mysql, "SELECT * FROM solar_pannels"))
        {
            return readed_data;
        }
    }
    catch(...)
    {
        return readed_data;
    }
    
    try
    {
        result = mysql_store_result(mysql);
        if (result == NULL)
        {
            return readed_data;
        }
    }
    catch(...)
    {
        return readed_data;
    }
    
    try
    {
        num_fields = mysql_num_fields(result);
    }
    catch(...)
    {
        return readed_data;
    }
    
    try
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)))
        {
            solar_pannels current_solar_pannel(mysql);
            current_solar_pannel.id = row[0] ? std::stoi(row[0]) : -1;
            strcpy(current_solar_pannel.solar_pannel_imei, row[1] ? row[1] : "");
            current_solar_pannel.current_measure = row[2] ? std::stoi(row[2]) : -1;
            current_solar_pannel.voltage_measure = row[3] ? std::stoi(row[3]) : -1;
            current_solar_pannel.temperature_measure = row[4] ? std::stoi(row[4]) : -1;
            current_solar_pannel.humidity_measure = row[5] ? std::stoi(row[5]) : -1;
            current_solar_pannel.state = row[6] ? std::stoi(row[6]) : -1;
            current_solar_pannel.date_time = row[7] ? string_to_time_t(row[7]) : -1;
            readed_data.push_back(current_solar_pannel);
        }
        mysql_next_result(mysql);
    }
    catch(...)
    {
        return readed_data;
    }
    
    try
    {
        mysql_free_result(result);
    }
    catch(...)
    {
        return readed_data;
    }
    
    return readed_data;
    
}

std::string solar_pannels::to_String_X8(unsigned int v)
{
	v &= 0xffffffff;
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(8)
		<< std::hex << v;
	std::string v_temp_string = stream.str();
	std::string v_ = "        ";
	// s.Substring(6, 2) + s.Substring(4, 2) + s.Substring(2, 2) + s.Substring(0, 2)
	v_[0] = toupper(v_temp_string[6]);
	v_[1] = toupper(v_temp_string[7]);
	//
	v_[2] = toupper(v_temp_string[4]);
	v_[3] = toupper(v_temp_string[5]);
	//
	v_[4] = toupper(v_temp_string[2]);
	v_[5] = toupper(v_temp_string[3]);
	//
	v_[6] = toupper(v_temp_string[0]);
	v_[7] = toupper(v_temp_string[1]);
	//
	return v_;
}

std::string solar_pannels::to_String_X4(int v)
{
	v &= 0xffff;
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(4)
		<< std::hex << v;
	std::string v_temp_string = stream.str();
	std::string v_ = "    ";
	// s.Substring(2, 2) + s.Substring(0, 2
	v_[0] = toupper(v_temp_string[2]);
	v_[1] = toupper(v_temp_string[3]);
	//
	v_[2] = toupper(v_temp_string[0]);
	v_[3] = toupper(v_temp_string[1]);
	return v_;
}

std::string solar_pannels::to_String_X2(int v)
{
	v &= 0xff;
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(2)
		<< std::hex << v;
	std::string v_temp_string = stream.str();
	std::string v_ = "  ";
	for (int i = 0; v_temp_string[i] != '\0'; i++)
	{
		v_[i] = toupper(v_temp_string[i]);
	}
	return v_;
}

std::string solar_pannels::to_String_00(int v)
{
	if (v < 10)
	{
		return "0" + std::to_string(v);
	}
	else
		return std::to_string(v);
}

char* solar_pannels::StringToByteArray(std::string hex)
{
	std::string temp = "";
	char *val = new char[hex.length() + 1];
	int x[10000] = { 0 };
	std::stringstream ss;
	for (int i = 0; hex[i] != '\0'; i += 2)
	{
		temp += hex[i];
		temp += hex[i + 1];
		ss << std::hex << temp;
		ss >> x[i / 2];
		val[i / 2] = x[i / 2];
		temp = "";
		ss.clear();
	}
	return val;
}

std::string solar_pannels::int2HexStrReverse2Byte(int v)
{
	v &= 0xffff;
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(4)
		<< std::hex << v;
	std::string v_temp_string = stream.str();
	std::string v_ = "    ";

	for (int i = 0; v_temp_string[i] != '\0'; i++)
		v_[i] = toupper(v_temp_string[(i + 2) % 4]);
	return v_;
}

std::string solar_pannels::calcLength(int length)
{
	length &= 0xffff;
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(4)
		<< std::hex << length;
	std::string v_temp_string = stream.str();
	std::string v_ = "    ";
	for (int i = 0; v_temp_string[i] != '\0';i++)
		v_[i] = toupper(v_temp_string[(i + 2) % 4]);
	return v_;
}

std::string solar_pannels::Print_time(time_t now)
{
    if(now == 0) return "000000000000";
    tm *my_time = localtime(&now);
    std::string date = "";
    date += std::to_string(my_time->tm_year + 1900);
    date += "-";
    date += to_String_00(my_time->tm_mon + 1);
    date += "-";
    date += to_String_00(my_time->tm_mday);
    date += " ";
    date += to_String_00(my_time->tm_hour);
    date += ":";
    date += to_String_00(my_time->tm_min);
    date += ":";
    date += to_String_00(my_time->tm_sec);
    return date;

}

time_t solar_pannels::string_to_time_t(const char *date)
{
    struct tm datetime;
    int yy = 0, month = 0, dd = 0, hh = 0, mm = 0, ss = 0;
    sscanf(date, "%d-%d-%d %d:%d:%d", &yy, &month, &dd, &hh, &mm, &ss);
    datetime.tm_year = yy - 1900;
    datetime.tm_mon = month - 1;
    datetime.tm_mday = dd;
    datetime.tm_hour = hh + 1;
    datetime.tm_min = mm;
    datetime.tm_sec = ss;
    datetime.tm_isdst = -1;

    return mktime(&datetime);
}

