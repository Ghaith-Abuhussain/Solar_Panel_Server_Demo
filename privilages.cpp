/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   privilages.cpp
 * Author: mohammad
 * 
 * Created on June 4, 2022, 11:08 AM
 */

#include "privilages.h"

privilages::privilages(MYSQL *_mysql) {
    mysql = _mysql;
}

privilages::~privilages() {
}

void privilages::Print_privilage()
{
    std::cout << "-------------------------\n";
    std::cout << "id = " << id << std::endl;
    std::cout << "user_id = " << user_id << std::endl;
    std::cout << "priv_type = " << priv_type << std::endl;
    std::cout << "datetime = " << Print_time(date_time) << std::endl;
}

bool privilages::select_privilage_by_id_userid(int _id, int _userid)
{
    int id_ = -1;
    int userid_ = -1;
    std::string statment = "SELECT *, Unix_Timestamp(date_time) FROM privilages WHERE id=? AND user_id=?";
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
            bind_param[1].buffer= (char *)&userid_;
            
            if (mysql_stmt_bind_param(stmt, bind_param))
            {
                fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            
            id_ = _id;
            userid_ = _userid;
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
            
            if (column_count != 5) /* validate column count */
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
            bind[1].buffer= (char *)&user_id;
            bind[1].is_null= &is_null[1];
            bind[1].length= &length[1];
            
            bind[2].buffer_type= MYSQL_TYPE_LONG;
            bind[2].buffer= (char *)&priv_type;
            bind[2].is_null= &is_null[2];
            bind[2].length= &length[2];
            
            bind[4].buffer_type= MYSQL_TYPE_LONG;
            bind[4].buffer= (char *)&date_time;
            bind[4].is_null= &is_null[4];
            bind[4].length= &length[4];
                        
            if (mysql_stmt_bind_result(stmt, bind))
            {
              fprintf(stderr, " mysql_stmt_bind_result() failed\n");
              fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
              return false;
            }
            if (mysql_stmt_store_result(stmt))
            {
              fprintf(stderr, " mysql_stmt_store_result() failed\n");
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

bool privilages::insert_update_privilage()
{
    std::string statment = "INSERT INTO `privilages` (`id`, `user_id`, `priv_type`, `date_time`) VALUES (?, ?, ?, FROM_UNIXTIME(?)) ON DUPLICATE KEY UPDATE id=VALUES(id), user_id=VALUES(user_id), priv_type=VALUES(priv_type), date_time=VALUES(date_time)";
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
            bind[1].buffer= (char *)&user_id;
            bind[1].is_null= &is_null[1];
            bind[1].length= &length[1];
            
            bind[2].buffer_type= MYSQL_TYPE_LONG;
            bind[2].buffer= (char *)&priv_type;
            bind[2].is_null= &is_null[2];
            bind[2].length= &length[2];
            
            bind[3].buffer_type= MYSQL_TYPE_LONG;
            bind[3].buffer= (char *)&date_time;
            is_null[3] = (date_time == -1);
            bind[3].is_null= &is_null[3];
            bind[3].length= &length[3];

            if (mysql_stmt_bind_param(stmt, bind))
            {
                fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
                return false;
            }
            int param_count= mysql_stmt_param_count(stmt);
            if (param_count != 4) /* validate parameter count */
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
                //fprintf(stderr, "privilages %d inserted\n", id);
            }
            if (mysql_stmt_affected_rows(stmt) != 1)
            {
                //fprintf(stderr, "privilages %d updated\n", id);
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

bool privilages::delete_privilage()
{
    user_id = -1;
    priv_type = -1;
    date_time = -1;
    int id_;
    std::string statment = "DELETE FROM `privilages` WHERE `id`= ?";
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
    id = 0;
    return true;
}

std::vector<privilages> privilages::get_all_user_privilages(int user_id)
{
    MYSQL_RES *result;
    int num_fields = 0;
    std::vector<privilages> readed_data;
    try
    {
        MYSQL_RES *result;
        int num_fields = 0;
        std::string select_query = "SELECT * FROM privilages WHERE user_id=" + std::to_string(user_id);
        if (mysql_query(mysql, select_query.c_str()))
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
            privilages current_privilages(mysql);
            current_privilages.id = row[0] ? std::stoi(row[0]) : -1;
            current_privilages.user_id = row[1] ? std::stoi(row[1]) : -1;
            current_privilages.priv_type = row[2] ? std::stoi(row[2]) : -1;
            current_privilages.date_time = row[3] ? string_to_time_t(row[3]) : -1;
            readed_data.push_back(current_privilages);
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

std::string privilages::to_String_X8(unsigned int v)
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

std::string privilages::to_String_X4(int v)
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

std::string privilages::to_String_X2(int v)
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

std::string privilages::to_String_00(int v)
{
	if (v < 10)
	{
		return "0" + std::to_string(v);
	}
	else
		return std::to_string(v);
}

char* privilages::StringToByteArray(std::string hex)
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

std::string privilages::int2HexStrReverse2Byte(int v)
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

std::string privilages::calcLength(int length)
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

std::string privilages::Print_time(time_t now)
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

time_t privilages::string_to_time_t(const char *date)
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