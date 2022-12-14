/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   solar_pannel_settings.cpp
 * Author: mohammad
 * 
 * Created on June 4, 2022, 1:44 PM
 */

#include "solar_pannel_settings.h"

solar_pannel_settings::solar_pannel_settings(MYSQL *_mysql) {
    mysql = _mysql;
}

solar_pannel_settings::~solar_pannel_settings() {
}

void solar_pannel_settings::Print_solar_pannel_settings()
{
    std::cout << "-------------------------\n";
    std::cout << "id = " << id << std::endl;
    std::cout << "solar_pannel_id = " << solar_pannel_id << std::endl;
    std::cout << "min_current = " << min_current << std::endl;
    std::cout << "max_current = " << max_current << std::endl;
    std::cout << "min_voltage = " << min_voltage << std::endl;
    std::cout << "max_voltage = " << max_voltage << std::endl;
    std::cout << "min_temperature = " << min_temperature << std::endl;
    std::cout << "max_temperature = " << max_temperature << std::endl;
    std::cout << "min_humidity = " << min_humidity << std::endl;
    std::cout << "max_humidity = " << max_humidity << std::endl;
    std::cout << "server_ip = " << server_ip << std::endl;
    std::cout << "server_port = " << server_port << std::endl;
    std::cout << "datetime = " << Print_time(date_time) << std::endl;
}

bool solar_pannel_settings::select_solar_pannel_setting_by_id_solar_pannel_id(int _id, int _solar_pannel_id)
{
    int id_ = -1;
    int solar_pannel_id_ = -1;
    std::string statment = "SELECT *, Unix_Timestamp(date_time) FROM solar_pannel_settings WHERE id=? AND solar_pannel_id=?";
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
            
            bind_param[1].buffer_type= MYSQL_TYPE_LONG;
            bind_param[1].buffer= (char *)&solar_pannel_id_;
            
            if (mysql_stmt_bind_param(stmt, bind_param))
            {
                fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            
            id_ = _id;
            solar_pannel_id_ = _solar_pannel_id;
            int param_count= mysql_stmt_param_count(stmt);
            if (param_count != 2) /* validate parameter count */
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
            
            if (column_count != 14) /* validate column count */
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
            
            bind[1].buffer_type= MYSQL_TYPE_LONG;
            bind[1].buffer= (char *)&solar_pannel_id;
            bind[1].is_null= &is_null[1];
            bind[1].length= &length[1];
            
            bind[2].buffer_type= MYSQL_TYPE_LONG;
            bind[2].buffer= (char *)&min_current;
            bind[2].is_null= &is_null[2];
            bind[2].length= &length[2];
            
            bind[3].buffer_type= MYSQL_TYPE_LONG;
            bind[3].buffer= (char *)&max_current;
            bind[3].is_null= &is_null[3];
            bind[3].length= &length[3];
            
            bind[4].buffer_type= MYSQL_TYPE_LONG;
            bind[4].buffer= (char *)&min_voltage;
            bind[4].is_null= &is_null[4];
            bind[4].length= &length[4];
            
            bind[5].buffer_type= MYSQL_TYPE_LONG;
            bind[5].buffer= (char *)&max_voltage;
            bind[5].is_null= &is_null[5];
            bind[5].length= &length[5];
            
            bind[6].buffer_type= MYSQL_TYPE_LONG;
            bind[6].buffer= (char *)&min_temperature;
            bind[6].is_null= &is_null[6];
            bind[6].length= &length[6];
            
            bind[7].buffer_type= MYSQL_TYPE_LONG;
            bind[7].buffer= (char *)&max_temperature;
            bind[7].is_null= &is_null[7];
            bind[7].length= &length[7];
            
            bind[8].buffer_type= MYSQL_TYPE_LONG;
            bind[8].buffer= (char *)&min_humidity;
            bind[8].is_null= &is_null[8];
            bind[8].length= &length[8];
            
            bind[9].buffer_type= MYSQL_TYPE_LONG;
            bind[9].buffer= (char *)&max_humidity;
            bind[9].is_null= &is_null[9];
            bind[9].length= &length[9];
            
            bind[10].buffer_type= MYSQL_TYPE_STRING;
            bind[10].buffer= (char *)&server_ip;
            bind[10].buffer_length = 40;
            bind[10].is_null= &is_null[10];
            bind[10].length= &length[10];
            
            bind[11].buffer_type= MYSQL_TYPE_LONG;
            bind[11].buffer= (char *)&server_port;
            bind[11].is_null= &is_null[11];
            bind[11].length= &length[11];
            
            bind[13].buffer_type= MYSQL_TYPE_LONG;
            bind[13].buffer= (char *)&date_time;
            bind[13].is_null= &is_null[13];
            bind[13].length= &length[13];
                        
            if (mysql_stmt_bind_result(stmt, bind))
            {
              fprintf(stderr, " mysql_stmt_bind_result() failed\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              return false;
            }
            if (mysql_stmt_store_result(stmt))
            {
              fprintf(stderr, " mysql_stmt_store_result() failed solar pannels settings 1\n");
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
int solar_pannel_settings::get_solar_pannel_setting_insert_id()
{
    MYSQL_RES *result;
    int num_fields = 0;
    int max_id = -1;
    try
    {
        MYSQL_RES *result;
        int num_fields = 0;
        std::string statment = "SELECT MAX(id) FROM solar_pannel_settings";
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
bool solar_pannel_settings::select_solar_pannel_setting_by_solar_pannel_id(int _solar_pannel_id)
{
    int solar_pannel_id_ = -1;
    std::string statment = "SELECT *, Unix_Timestamp(date_time) FROM solar_pannel_settings WHERE solar_pannel_id=?";
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
            bind_param[0].buffer= (char *)&solar_pannel_id_;
            
            if (mysql_stmt_bind_param(stmt, bind_param))
            {
                fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            
            solar_pannel_id_ = _solar_pannel_id;
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
            
            if (column_count != 14) /* validate column count */
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
            
            bind[1].buffer_type= MYSQL_TYPE_LONG;
            bind[1].buffer= (char *)&solar_pannel_id;
            bind[1].is_null= &is_null[1];
            bind[1].length= &length[1];
            
            bind[2].buffer_type= MYSQL_TYPE_LONG;
            bind[2].buffer= (char *)&min_current;
            bind[2].is_null= &is_null[2];
            bind[2].length= &length[2];
            
            bind[3].buffer_type= MYSQL_TYPE_LONG;
            bind[3].buffer= (char *)&max_current;
            bind[3].is_null= &is_null[3];
            bind[3].length= &length[3];
            
            bind[4].buffer_type= MYSQL_TYPE_LONG;
            bind[4].buffer= (char *)&min_voltage;
            bind[4].is_null= &is_null[4];
            bind[4].length= &length[4];
            
            bind[5].buffer_type= MYSQL_TYPE_LONG;
            bind[5].buffer= (char *)&max_voltage;
            bind[5].is_null= &is_null[5];
            bind[5].length= &length[5];
            
            bind[6].buffer_type= MYSQL_TYPE_LONG;
            bind[6].buffer= (char *)&min_temperature;
            bind[6].is_null= &is_null[6];
            bind[6].length= &length[6];
            
            bind[7].buffer_type= MYSQL_TYPE_LONG;
            bind[7].buffer= (char *)&max_temperature;
            bind[7].is_null= &is_null[7];
            bind[7].length= &length[7];
            
            bind[8].buffer_type= MYSQL_TYPE_LONG;
            bind[8].buffer= (char *)&min_humidity;
            bind[8].is_null= &is_null[8];
            bind[8].length= &length[8];
            
            bind[9].buffer_type= MYSQL_TYPE_LONG;
            bind[9].buffer= (char *)&max_humidity;
            bind[9].is_null= &is_null[9];
            bind[9].length= &length[9];
            
            bind[10].buffer_type= MYSQL_TYPE_STRING;
            bind[10].buffer= (char *)&server_ip;
            bind[10].buffer_length = 40;
            bind[10].is_null= &is_null[10];
            bind[10].length= &length[10];
            
            bind[11].buffer_type= MYSQL_TYPE_LONG;
            bind[11].buffer= (char *)&server_port;
            bind[11].is_null= &is_null[11];
            bind[11].length= &length[11];
            
            bind[13].buffer_type= MYSQL_TYPE_LONG;
            bind[13].buffer= (char *)&date_time;
            bind[13].is_null= &is_null[13];
            bind[13].length= &length[13];
                        
            if (mysql_stmt_bind_result(stmt, bind))
            {
              fprintf(stderr, " mysql_stmt_bind_result() failed\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              return false;
            }
            if (mysql_stmt_store_result(stmt))
            {
              fprintf(stderr, " mysql_stmt_store_result() failed solar_pannek_setting 2\n");
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

bool solar_pannel_settings::insert_update_solar_pannel_setting()
{
    std::string statment = "INSERT INTO `solar_pannel_settings` (`id`, `solar_pannel_id`, `min_current`, `max_current`, `min_voltage`, `max_voltage`, `min_temperature`, `max_temperature`, `min_humidity`, `max_humidity`, `server_ip`, `server_port`, `date_time`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, FROM_UNIXTIME(?)) ON DUPLICATE KEY UPDATE id=VALUES(id), solar_pannel_id=VALUES(solar_pannel_id), min_current=VALUES(min_current), max_current=VALUES(max_current), min_voltage=VALUES(min_voltage), max_voltage=VALUES(max_voltage), min_temperature=VALUES(min_temperature), max_temperature=VALUES(max_temperature), min_humidity=VALUES(min_humidity), max_humidity=VALUES(max_humidity), server_ip=VALUES(server_ip), server_port=VALUES(server_port), date_time=VALUES(date_time)";
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
            
            bind[1].buffer_type= MYSQL_TYPE_LONG;
            bind[1].buffer= (char *)&solar_pannel_id;
            bind[1].is_null= &is_null[1];
            bind[1].length= &length[1];
            
            bind[2].buffer_type= MYSQL_TYPE_LONG;
            bind[2].buffer= (char *)&min_current;
            bind[2].is_null= &is_null[2];
            bind[2].length= &length[2];
            
            bind[3].buffer_type= MYSQL_TYPE_LONG;
            bind[3].buffer= (char *)&max_current;
            bind[3].is_null= &is_null[3];
            bind[3].length= &length[3];
            
            bind[4].buffer_type= MYSQL_TYPE_LONG;
            bind[4].buffer= (char *)&min_voltage;
            bind[4].is_null= &is_null[4];
            bind[4].length= &length[4];
            
            bind[5].buffer_type= MYSQL_TYPE_LONG;
            bind[5].buffer= (char *)&max_voltage;
            bind[5].is_null= &is_null[5];
            bind[5].length= &length[5];
            
            bind[6].buffer_type= MYSQL_TYPE_LONG;
            bind[6].buffer= (char *)&min_temperature;
            bind[6].is_null= &is_null[6];
            bind[6].length= &length[6];
            
            bind[7].buffer_type= MYSQL_TYPE_LONG;
            bind[7].buffer= (char *)&max_temperature;
            bind[7].is_null= &is_null[7];
            bind[7].length= &length[7];
            
            bind[8].buffer_type= MYSQL_TYPE_LONG;
            bind[8].buffer= (char *)&min_humidity;
            bind[8].is_null= &is_null[8];
            bind[8].length= &length[8];
            
            bind[9].buffer_type= MYSQL_TYPE_LONG;
            bind[9].buffer= (char *)&max_humidity;
            bind[9].is_null= &is_null[9];
            bind[9].length= &length[9];
            
            bind[10].buffer_type= MYSQL_TYPE_STRING;
            bind[10].buffer= (char *)server_ip;
            bind[10].buffer_length = 40;
            bind[10].is_null= &is_null[10];
            length[10] = strlen(server_ip);
            bind[10].length= &length[10];
            
            bind[11].buffer_type= MYSQL_TYPE_LONG;
            bind[11].buffer= (char *)&server_port;
            bind[11].is_null= &is_null[11];
            bind[11].length= &length[11];
            
            bind[12].buffer_type= MYSQL_TYPE_LONG;
            bind[12].buffer= (char *)&date_time;
            is_null[12] = (date_time == -1);
            bind[12].is_null= &is_null[12];
            bind[12].length= &length[12];

            if (mysql_stmt_bind_param(stmt, bind))
            {
                fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            int param_count= mysql_stmt_param_count(stmt);
            if (param_count != 13) /* validate parameter count */
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
                //fprintf(stderr, "solar_pannel_settings %d inserted\n", id);
            }
            if (mysql_stmt_affected_rows(stmt) != 1)
            {
                //fprintf(stderr, "solar_pannel_settings %d updated\n", id);
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

bool solar_pannel_settings::delete_solar_pannel_setting()
{
    solar_pannel_id = -1;
    min_current = -1;
    max_current = -1;
    min_voltage = -1;
    max_voltage = -1;
    min_temperature = -1;
    max_temperature = -1;
    min_humidity = -1;
    max_humidity = -1;
    strcpy(server_ip, "");
    server_port = -1;
    date_time = 0;
    int id_;
    std::string statment = "DELETE FROM `solar_pannel_settings` WHERE `id`= ?";
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

std::string solar_pannel_settings::to_String_X8(unsigned int v)
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

std::string solar_pannel_settings::to_String_X4(int v)
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

std::string solar_pannel_settings::to_String_X2(int v)
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

std::string solar_pannel_settings::to_String_00(int v)
{
	if (v < 10)
	{
		return "0" + std::to_string(v);
	}
	else
		return std::to_string(v);
}

char* solar_pannel_settings::StringToByteArray(std::string hex)
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

std::string solar_pannel_settings::int2HexStrReverse2Byte(int v)
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

std::string solar_pannel_settings::calcLength(int length)
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

std::string solar_pannel_settings::Print_time(time_t now)
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

time_t solar_pannel_settings::string_to_time_t(const char *date)
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


