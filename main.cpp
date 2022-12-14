#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include <libwebsockets.h>
#include <fstream>
#include <cstdlib>
#include <mysql/mysql.h>
#include <nlohmann/json.hpp>
#include <bits/stl_vector.h>

#include "MessageParser.h"
#include "users.h"
#include "privilages.h"
#include "solar_pannels.h"
#include "solar_pannel_settings.h"
#include "all_measurements_table.h"
using namespace std;
MYSQL                           *mysql;
#define PORT 27015


using json = nlohmann::json;
using namespace std;

/*
 * 
 */
int current_id = 0;

struct device
{
    int id;
    int socket;
    char IMEI[16];
    char file_name[64];
    MessageParser* parser = new MessageParser();
    std::ofstream outfile;
};
device solar_pannels_collection_card[256];

pthread_t my_thread[256];
struct device devices[256];
pthread_t connection_thread;
pthread_t Http_Server_Thread;

int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[1024] = {0};
char *hello = "Hello from server";
struct lws *web_socket;
std::string params_from_webpages = "";


HexDealer* hex_dealer = new HexDealer();
MessageParser* parser_general = new MessageParser();

users user(NULL);
privilages privilage(NULL);
solar_pannels solar_pannel(NULL);
solar_pannel_settings solar_pannel_setting(NULL);
all_measurements_table measurments(NULL);

std::string valid_imeis[256] = {"123456789123456", "123456789654321", "987654321654321", "987654321123456"};
int ids[256] = {1, 2, 3, 4};

#ifndef function declaration
time_t str_to_datetime(std::string date_str);
std::vector<all_measurements_table> select_measurment_between_two_dates(time_t date_1, time_t date_2, std::vector<all_measurements_table> all_measurments);
json convert_to_json_array(std::vector<json> json_vect);
std::string gen_login_response_json(int err_type, std::string err_msg, bool add_solar_panel,
        bool remove_solar_panel, bool update_solar_panel);
std::string gen_measurement_response(int err_type, std::string err_msg,
        int measure, std::vector<json> json_vect);
std::string gen_settings_response(int err_type, std::string err_msg);
std::string gen_add_panel_response(int err_type, std::string err_msg);
std::string gen_remove_panel_response(int err_type, std::string err_msg);
static int callback_dumb_increment(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
void websocket_response(struct lws* web_socket, std::string reply);
static int callback_http( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len );
void* Http_Server_Process(void * arg);
std::string genLoginResponse(std::string id);
std::string genheartbeatMessageresponse(int id);
std::string genmeasurmentMessageresponse(int CurrVal, int VoltVal, int TempVal, int HumVal);
std::string genalarmMessageresponse(int numAlarm,int CurrVal, int VoltVal, int TempVal, int HumVal, int Talarm[]);
int check_imei_validation(std::string curr_imei);
std::string gen_alarm_type(int Talarm[], int len);
std::vector<bool> get_user_privilages(int user_id);
bool connect_to_database();
int create_server();
void print_received_message(char buffer[]);
void* process_client(void * arg);
void* connection_proccess(void * arg);
#endif

time_t str_to_datetime(std::string date_str)
{
    struct std::tm when = {0};
    int year = -1, month = -1, day = -1, hour = -1, minute = -1, second = -1;
    sscanf(date_str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    when.tm_year = year - 1900;
    when.tm_mon  = month - 1;
    when.tm_mday = day;
    when.tm_hour = hour;
    when.tm_min  = minute;
    when.tm_sec = second;
    strptime(date_str.c_str(), "YYYY-MM-DD HH:MM:SS", &when);
    
    time_t result = mktime(&when);
    return result;
    
}

std::vector<all_measurements_table> select_measurment_between_two_dates(time_t date_1, time_t date_2, std::vector<all_measurements_table> all_measurments)
{
    std::vector<all_measurements_table> result;
    for(int i = 0; i < all_measurments.size(); i++)
    {
        if(all_measurments[i].date_time <= date_2 && all_measurments[i].date_time >= date_1)
        {
            result.push_back(all_measurments[i]);
        }
    }
    return result;
}

#ifndef json_functions

json convert_to_json_array(std::vector<json> json_vect)
{
    json result;
    std::string result_str = "";
    result_str += "[";
    for(int i = 0; i < json_vect.size(); i++)
    {
        std::string sub_str = json_vect[i].dump();
        result_str += sub_str;
        if(i != json_vect.size() - 1)
        {
            result_str += ",";
        }
        else
        {
            result_str += "]";
        }
    }
    
    result = json::parse(result_str);
    return result;
}

std::string gen_login_response_json(int err_type, std::string err_msg, bool add_solar_panel,
        bool remove_solar_panel, bool update_solar_panel)
{
    json response = {
        {"err_type",std::to_string(err_type)},
        {"err_msg",err_msg},
        {"add_solar_panel", std::to_string(add_solar_panel)},
        {"remove_solar_panel", std::to_string(remove_solar_panel)},
        {"update_solar_panel", std::to_string(update_solar_panel)}
    };
    
    std::string resp_str = response.dump();
    return resp_str;
}

std::string gen_measurement_response(int err_type, std::string err_msg,
        int measure, std::vector<json> json_vect)
{
    json json_array = convert_to_json_array(json_vect);
    json response = {
        {"msg_type","4"},
        {"err_type",std::to_string(err_type)},
        {"err_msg",err_msg},
        {"measure",std::to_string(measure)},
        {"data",json_array}
    };
    
    std::string resp_str = response.dump();
    return resp_str;
}

std::string gen_pannels_response(int err_type, std::string err_msg,
        std::vector<json> json_vect)
{
    json json_array = convert_to_json_array(json_vect);
    json response = {
        {"msg_type","2"},
        {"err_type",std::to_string(err_type)},
        {"err_msg",err_msg},
        {"data",json_array}
    };
    
    std::string resp_str = response.dump();
    return resp_str;
}

std::string gen_settings_response(int err_type, std::string err_msg)
{
    json response = {
        {"err_type",std::to_string(err_type)},
        {"err_msg",err_msg}
    };
    
    std::string resp_str = response.dump();
    return resp_str;
}
std::string gen_add_panel_response(int err_type, std::string err_msg)
{
    json response = {
        {"err_type",std::to_string(err_type)},
        {"err_msg",err_msg}
    };
    
    std::string resp_str = response.dump();
    return resp_str;
}
std::string gen_remove_panel_response(int err_type, std::string err_msg)
{
    json response = {
        {"err_type",std::to_string(err_type)},
        {"err_msg",err_msg},
        {"msg_type","6"}
    };
    
    std::string resp_str = response.dump();
}
#endif

#ifndef web_socket_functions

static int callback_dumb_increment(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason) 
    {
        case LWS_CALLBACK_ESTABLISHED: // just log message that someone is connecting
            printf("connection established\n");
            break;
        case LWS_CALLBACK_RECEIVE: { 
            // the funny part
            // create a buffer to hold our response
            // it has to have some pre and post padding. You don't need to care
            // what comes there, libwebsockets will do everything for you. For more info see
            // http://git.warmcat.com/cgi-bin/cgit/libwebsockets/tree/lib/libwebsockets.h#n597
            unsigned char *buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + len +
                                                         LWS_SEND_BUFFER_POST_PADDING);
            // pointer to `void *in` holds the incomming request
            // we're just going to put it in reverse order and put it in `buf` with
            // correct offset. `len` holds length of the request.
            std::string params = "";
            params_from_webpages = params;
            for (int i=0; i < len; i++) {
                buf[LWS_SEND_BUFFER_PRE_PADDING + i ] = ((char *) in)[i];
                params += ((char *) in)[i];
            }
            params_from_webpages = params;
            web_socket = wsi;
            try
            {
                
                //memcpy( &buf, in, len );
                // log what we recieved and what we're going to send as a response.
                // that disco syntax `%.*s` is used to print just a part of our buffer
                // http://stackoverflow.com/questions/5189071/print-part-of-char-array
                std::cout<< params << std::endl;
                // release memory back into the wild
                free(buf);
                break;
            }
            catch(...)
            {
                std::cout << "error with websocket" << std::endl;
            }
            
        }
        default:
        break;
    }
    return 0;
}

void websocket_response(struct lws* web_socket, std::string reply)
{
    unsigned char *buf_response = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + reply.length() +
                                                                 LWS_SEND_BUFFER_POST_PADDING);
    for (int i=0; i < reply.length(); i++) 
    {
        buf_response[LWS_SEND_BUFFER_PRE_PADDING + i ] = reply[i];
    }
    lws_write(web_socket, &buf_response[LWS_SEND_BUFFER_PRE_PADDING], reply.length(), LWS_WRITE_TEXT);
}

static int callback_http( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len )
{
    switch (reason) 
    {
        case LWS_CALLBACK_ESTABLISHED: // just log message that someone is connecting
            printf("connection established\n");
            break;
        case LWS_CALLBACK_RECEIVE: { 
            // the funny part
            // create a buffer to hold our response
            // it has to have some pre and post padding. You don't need to care
            // what comes there, libwebsockets will do everything for you. For more info see
            // http://git.warmcat.com/cgi-bin/cgit/libwebsockets/tree/lib/libwebsockets.h#n597
            unsigned char *buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + len +
                                                         LWS_SEND_BUFFER_POST_PADDING);
            // pointer to `void *in` holds the incomming request
            // we're just going to put it in reverse order and put it in `buf` with
            // correct offset. `len` holds length of the request.
            std::string params = "";
            params_from_webpages = params;
            for (int i=0; i < len; i++) {
                buf[LWS_SEND_BUFFER_PRE_PADDING + i ] = ((char *) in)[i];
                params += ((char *) in)[i];
            }
            params_from_webpages = params;
            web_socket = wsi;
            try
            {
                
                //memcpy( &buf, in, len );
                // log what we recieved and what we're going to send as a response.
                // that disco syntax `%.*s` is used to print just a part of our buffer
                // http://stackoverflow.com/questions/5189071/print-part-of-char-array
                std::cout<< params << std::endl;
                json json_received = json::parse(params);
                std::string msg_type_str = json_received["msg_type"];
                int msg_type = std::stoi(msg_type_str);
                switch(msg_type)
                {
                    case 1:
                    {
                        json json_data = json_received["data"];
                        std::string username = json_data["username"];
                        std::string password_hash = json_data["password_hash"];

                        std::cout << msg_type << std::endl;
                        std::cout << username << std::endl;
                        std::cout << password_hash << std::endl;
                        users user_login_try(mysql);
                        user_login_try.select_user_by_name(username);
                        
                        if(user_login_try.id != -1)
                        {
                            std::vector<bool> user_privilages = get_user_privilages(user_login_try.id);
                            if(std::string(user_login_try.password) == password_hash)
                            {
                                std::string response = gen_login_response_json(200, "", user_privilages[0], user_privilages[1], user_privilages[2]);
                                std::cout << "response: " << response << std::endl;
                                websocket_response(web_socket, response);
                            }
                            else
                            {
                                std::string response = gen_login_response_json(300, "password is wrong", user_privilages[0], user_privilages[1], user_privilages[3]);
                                websocket_response(web_socket, response);
                            }
                        }
                        else
                        {
                            std::string response = gen_login_response_json(400, "not found!!", false, false, false);
                            websocket_response(web_socket, response);
                        }

                        
                    }break;
                    case 2:
                    {
                        solar_pannels solar_pannel_db(mysql);
                        solar_pannel_settings solar_pannel_setting_db(mysql);
                        std::vector<solar_pannels> list_of_pannels = solar_pannel_db.read_all_solar_pannels(mysql);
                        std::vector<json> json_records;
                        for(int i = 0; i < list_of_pannels.size(); i++)
                        {
                            solar_pannel_setting_db.select_solar_pannel_setting_by_solar_pannel_id(list_of_pannels[i].id);
                            json record = {
                                {"id", std::to_string(list_of_pannels[i].id)}, 
                                {"imei", std::string(list_of_pannels[i].solar_pannel_imei)}, 
                                {"min_current", std::to_string(solar_pannel_setting_db.min_current)}, 
                                {"max_current", std::to_string(solar_pannel_setting_db.max_current)}, 
                                {"min_voltage", std::to_string(solar_pannel_setting_db.min_voltage)},
                                {"max_voltage", std::to_string(solar_pannel_setting_db.max_voltage)},
                                {"min_temperature", std::to_string(solar_pannel_setting_db.min_humidity)},
                                {"max_temperature", std::to_string(solar_pannel_setting_db.max_humidity)},
                                {"min_humidity", std::to_string(solar_pannel_setting_db.min_temperature)},
                                {"max_humidity", std::to_string(solar_pannel_setting_db.max_temperature)},
                                {"state", std::to_string(list_of_pannels[i].state)},
                                {"date", list_of_pannels[i].Print_time(list_of_pannels[i].date_time)}
                            };
                            json_records.push_back(record);
                        }
                        json pannels_json_array = convert_to_json_array(json_records);
                        std::string response = gen_pannels_response(200, "", pannels_json_array);
                        websocket_response(web_socket, response);
                    }break;
                    case 3:
                    {
                        std::string solar_pannel_id_str = json_received["solar_pannel_id"];
                        std::string measure_str = json_received["measure"];
                        std::string min_str = json_received["min"];
                        std::string max_str = json_received["max"];
                        int solar_pannel_id = std::stoi(solar_pannel_id_str);
                        solar_pannel_settings settings(mysql);
                        settings.select_solar_pannel_setting_by_solar_pannel_id(solar_pannel_id);
                        int measure_type = std::stoi(measure_str);
                        int min_val = std::stoi(min_str);
                        int max_val = std::stoi(max_str);
                        switch(measure_type)
                        {
                            case 0:
                            {
                                settings.min_voltage = min_val; 
                                settings.max_voltage = max_val;
                                settings.date_time = time(0);
                                settings.insert_update_solar_pannel_setting();
                            }break;
                            case 1:
                            {
                                settings.min_current= min_val; 
                                settings.max_current = max_val;
                                settings.date_time = time(0);
                                settings.insert_update_solar_pannel_setting();
                            }break;
                            case 2:
                            {
                                settings.min_temperature = min_val; 
                                settings.max_temperature = max_val;
                                settings.date_time = time(0);
                                settings.insert_update_solar_pannel_setting();
                            }break;
                            case 3:
                            {
                                settings.min_humidity = min_val; 
                                settings.max_humidity = max_val;
                                settings.date_time = time(0);
                                settings.insert_update_solar_pannel_setting();
                            }break;
                            default:
                            {
                                
                            }break;
                        }
                        std::string response = gen_settings_response(200, "");
                        websocket_response(web_socket, response);
                    }break;
                    case 4:
                    {
                        std::string solar_pannel_id_str = json_received["solar_pannel_id"];
                        std::string measure_str = json_received["measure"];
                        std::string min_date_str = json_received["min_date"];
                        std::string max_date_str = json_received["max_date"];
                        int solar_pannel_id = std::stoi(solar_pannel_id_str);
                        int measure_type = std::stoi(measure_str);
                        time_t min_date_val = str_to_datetime(min_date_str);
                        time_t max_date_val = str_to_datetime(max_date_str);
                        std::cout << "min_date_val: " <<  min_date_val << std::endl;
                        std::cout << "max_date_val: " <<  max_date_val << std::endl;
                        all_measurements_table solar_pannel_measurment(mysql);
                        std::vector<all_measurements_table> all_measurment = solar_pannel_measurment.select_measurement_by_solar_pannel_id(solar_pannel_id);
                        std::vector<all_measurements_table> selected_measures = select_measurment_between_two_dates(min_date_val, max_date_val, all_measurment);
                        switch(measure_type)
                        {
                            case 0:
                            {
                                std::vector<json> json_vect;
                                for(int i = 0; i < selected_measures.size(); i++)
                                {
                                    json data = {
                                        {"record_index",std::to_string(selected_measures[i].record_index)},
                                        {"measure",std::to_string(selected_measures[i].voltage_measure)},
                                        {"alarm_state",std::to_string(selected_measures[i].alarm_state)},
                                        {"alarm_types",std::string(selected_measures[i].alarm_types)},
                                        {"time",std::to_string(selected_measures[i].date_time)}
                                    };
                                    json_vect.push_back(data);
                                }
                                if(selected_measures.size() == 0)
                                {
                                    std::string response = gen_settings_response(404, "no_record_found");
                                    std::cout << "web response:\n" << response << std::endl;
                                    websocket_response(web_socket, response);
                                }
                                else
                                {
                                    std::string response = gen_measurement_response(200, "",
                                    0, json_vect);
                                    websocket_response(web_socket, response);
                                }
                                
                            }break;
                            case 1:
                            {
                                std::vector<json> json_vect;
                                for(int i = 0; i < selected_measures.size(); i++)
                                {
                                    json data = {
                                        {"record_index",std::to_string(selected_measures[i].record_index)},
                                        {"measure",std::to_string(selected_measures[i].current_measure)},
                                        {"alarm_state",std::to_string(selected_measures[i].alarm_state)},
                                        {"alarm_types",std::string(selected_measures[i].alarm_types)},
                                        {"time",std::to_string(selected_measures[i].date_time)}
                                    };
                                    json_vect.push_back(data);
                                }
                                if(selected_measures.size() == 0)
                                {
                                    std::string response = gen_settings_response(404, "no_record_found");
                                    websocket_response(web_socket, response);
                                }
                                else
                                {
                                    std::string response = gen_measurement_response(200, "",
                                    1, json_vect);
                                    websocket_response(web_socket, response);
                                }
                            }break;
                            case 2:
                            {
                               std::vector<json> json_vect;
                                for(int i = 0; i < selected_measures.size(); i++)
                                {
                                    json data = {
                                        {"record_index",std::to_string(selected_measures[i].record_index)},
                                        {"measure",std::to_string(selected_measures[i].temperature_measure)},
                                        {"alarm_state",std::to_string(selected_measures[i].alarm_state)},
                                        {"alarm_types",std::string(selected_measures[i].alarm_types)},
                                        {"time",std::to_string(selected_measures[i].date_time)}
                                    };
                                    json_vect.push_back(data);
                                }
                                if(selected_measures.size() == 0)
                                {
                                    std::string response = gen_settings_response(404, "no_record_found");
                                    websocket_response(web_socket, response);
                                }
                                else
                                {
                                    std::string response = gen_measurement_response(200, "",
                                    2, json_vect);
                                    websocket_response(web_socket, response);
                                }
                                   
                            }break;
                            case 3:
                            {
                                std::vector<json> json_vect;
                                for(int i = 0; i < selected_measures.size(); i++)
                                {
                                    json data = {
                                        {"record_index",std::to_string(selected_measures[i].record_index)},
                                        {"measure",std::to_string(selected_measures[i].humidity_measure)},
                                        {"alarm_state",std::to_string(selected_measures[i].alarm_state)},
                                        {"alarm_types",std::string(selected_measures[i].alarm_types)},
                                        {"time",std::to_string(selected_measures[i].date_time)}
                                    };
                                    json_vect.push_back(data);
                                }
                                if(selected_measures.size() == 0)
                                {
                                    std::string response = gen_settings_response(404, "no_record_found");
                                    websocket_response(web_socket, response);
                                }
                                else
                                {
                                    std::string response = gen_measurement_response(200, "",
                                    3, json_vect);
                                    websocket_response(web_socket, response);
                                }
                            }break;
                            default:
                            {
                                
                            }break;
                        }
                    }break;
                    case 5:
                    {
                        std::string imei=json_received["device_imei"];
                        std::string min_current_str= json_received["min_current"];
                        std::string max_current_str= json_received["max_current"];
                        std::string min_voltage_str= json_received["min_voltage"];
                        std::string max_voltage_str= json_received["max_voltage"];
                        std::string min_temperature_str= json_received["min_temperature"];
                        std::string max_temperature_str= json_received["max_temperature"];
                        std::string min_humidity_str= json_received["min_humidity"];
                        std::string max_humidity_str= json_received["max_humidity"];
                        solar_pannels solar_panel(mysql);
                        bool check=solar_panel.select_solar_pannel_by_imei(imei);
                        if (solar_panel.id==-1)
                        {
                            
                           solar_panel.id=solar_panel.get_solar_pannels_insert_id();
                           strcpy(solar_panel.solar_pannel_imei,imei.c_str());
                           
                            solar_panel.current_measure = 0;
                            solar_panel.voltage_measure = 0;
                            solar_panel.temperature_measure = 0;
                            solar_panel.humidity_measure = 0;
                            solar_panel.state = 0;
                            solar_panel.date_time = time(0);
                            solar_panel.get_solar_pannels_insert_id();
                            solar_panel.insert_update_solar_pannel();
                            solar_pannel_settings setting(mysql);
                           
                            setting.id =setting.get_solar_pannel_setting_insert_id();
                            setting.solar_pannel_id =solar_panel.id ;
                            setting.min_current =std::stoi(min_current_str);
                            setting.max_current = stoi(max_current_str);
                            setting.min_voltage = stoi(min_voltage_str);
                            setting.max_voltage = stoi(max_voltage_str);
                            setting.min_temperature = stoi(min_temperature_str);
                            setting.max_temperature =stoi(max_temperature_str);
                            setting.min_humidity = stoi(min_humidity_str);
                            setting.max_humidity = stoi(max_humidity_str);
                            strcpy(setting.server_ip,"");
                            setting.server_port = -1;
                            setting.date_time = time(0);
                            setting.insert_update_solar_pannel_setting();
                            std::string response=gen_add_panel_response(200,"");
                            websocket_response(web_socket, response);
                            
                        }
                        else
                        {
                          std::string response=gen_add_panel_response(300,"solar panel is already exist");
                            websocket_response(web_socket, response);
                        }
                        
                    }break;
                    case 6:
                    {
                    std::string id=json_received["solar_panel_id"];
                    int solar_panel_id=std::stoi(id);
                    solar_pannel_settings setting(mysql);
                    setting.select_solar_pannel_setting_by_solar_pannel_id(solar_panel_id);
                    setting.delete_solar_pannel_setting();
                    solar_pannels solar_pannel(mysql);
                    solar_pannel.select_solar_pannel_by_id(solar_panel_id);
                    solar_pannel.delete_solar_pannel();
                    std::string response=gen_remove_panel_response(200,"");
                    websocket_response(web_socket, response);
                    
                    }break;
                    default:
                    {

                    }break;
                }
                // release memory back into the wild
                free(buf);
                break;
            }
            catch(...)
            {
                std::cout << "error with websocket" << std::endl;
            }
            
        }
        default:
        break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
    /* first protocol must always be HTTP handler */
    {
        "http-only",   // name
        callback_http, // callback
        0              // per_session_data_size
    },
    {
        "dumb-increment-protocol", // protocol name - very important!
        callback_dumb_increment,   // callback
        0                          // we don't use any per session data
    },
    {
        NULL, NULL, 0   /* End of list */
    }
};

void* Http_Server_Process(void * arg)
 {
    struct lws_context_creation_info info;
    memset( &info, 0, sizeof(info) );
    info.port = 9000;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    
    // create libwebsocket context representing this server
    struct lws_context *context = lws_create_context( &info);
    
    if (context == NULL) {
        fprintf(stderr, "libwebsocket init failed\n");
    }
    
    printf("starting server...\n");
    
    // infinite loop, to end this server send SIGTERM. (CTRL+C)
    while (1) {
        lws_service( context, /* timeout_ms = */ 50 );
        // libwebsocket_service will process all waiting events with their
        // callback functions and then wait 50 ms.
        // (this is a single threaded webserver and this will keep our server
        // from generating load while there are not requests to process)
    }
    
    lws_context_destroy(context);
 }

#endif

#ifndef response_messages_to_device_functions

std::string genLoginResponse(std::string id)
{
    uint8_t pData[1024] = { 0 };
    char dataInChar[1024] = "";
    int header = 255;
    int dataLength = 26;
    int direction = 0;
    int type = 1;
    int footer = 205;
    int indexPdata = 0;
    time_t now = time(0);
    std::string  s = "";
    s += hex_dealer->to_String_X2(header);
    s += hex_dealer->to_String_X2(dataLength);
    s += hex_dealer->to_String_X2(direction);
    s += hex_dealer->to_String_X2(type);
    s += hex_dealer->calcCurrentTime(now);
    for (int i = 0; i < id.length(); i++)
    {
        s += hex_dealer->to_String_X2(id[i]);
    }
    s += hex_dealer->to_String_X4(0);
    s += hex_dealer->to_String_X2(footer);
    int len = 0;
    hex_dealer->StringToByteArrayToSend(s, s.length(), dataInChar);
    for (int i = 0; i < s.length() / 2 - 5; i++)
    {
        pData[i] = (uint8_t)dataInChar[i + 2];
        len++;
    }
    uint16_t chSum = hex_dealer->GetCrc16(pData, len);
    s = "";
    s += hex_dealer->to_String_X2(header);
    s += hex_dealer->to_String_X2(dataLength);
    s += hex_dealer->to_String_X2(direction);
    s += hex_dealer->to_String_X2(type);
    s += hex_dealer->calcCurrentTime(now);
    for (int i = 0; i < id.length(); i++)
    {
        s += hex_dealer->to_String_X2(id[i]);
    }
    s += hex_dealer->to_String_X4(chSum);
    s += hex_dealer->to_String_X2(footer);
    return s;
}
std::string genheartbeatMessageresponse(int id)
{
	uint8_t pData[1024] = { 0 };
	char dataInChar[1024] = "";
	int header = 255;
	int dataLength = 27;
	int direction = 0;
	int type = 2;
	int footer = 205;
	int content = 1;
        solar_pannel_settings solar_pannel_settings_hb_db(mysql);
        solar_pannel_settings_hb_db.select_solar_pannel_setting_by_solar_pannel_id(id);
        time_t now = time(0);
	std::string  s = "";
	s += hex_dealer->to_String_X2(header);
	s +=hex_dealer->to_String_X2(dataLength);
	s +=hex_dealer->to_String_X2(direction);
	s +=hex_dealer->to_String_X2(type);
	s +=hex_dealer->calcCurrentTime(now);
	s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.min_current);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.max_current);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.min_voltage);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.max_voltage);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.min_temperature);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.max_temperature);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.min_humidity);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.max_humidity);
	s +=hex_dealer->to_String_X4(0);
	s +=hex_dealer->to_String_X2(footer);
	int len = 0;
	hex_dealer->StringToByteArrayToSend(s, s.length(), dataInChar);
	for (int i = 0; i < s.length() / 2 - 5; i++)
	{
		pData[i] = (uint8_t)dataInChar[i + 2];
		len++;
	}
	uint16_t chSum =hex_dealer->GetCrc16(pData, len);
	s = "";
	s +=hex_dealer->to_String_X2(header);
	s +=hex_dealer->to_String_X2(dataLength);
	s +=hex_dealer->to_String_X2(direction);
	s +=hex_dealer->to_String_X2(type);
	s +=hex_dealer->calcCurrentTime(now);
	s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.min_current);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.max_current);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.min_voltage);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.max_voltage);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.min_temperature);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.max_temperature);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.min_humidity);
        s +=hex_dealer->to_String_X4(solar_pannel_settings_hb_db.max_humidity);
	s +=hex_dealer->to_String_X4(chSum);
	s +=hex_dealer->to_String_X2(footer);
	return s;
}
std::string genmeasurmentMessageresponse(int CurrVal, int VoltVal, int TempVal, int HumVal)
{
  uint8_t pData[1024] = { 0 };
  char dataInChar[1024] = "";
  int header = 255;
  int dataLength = 19;
  int direction = 0;
  int type = 3;
  int footer = 205;
  time_t now = time(0);
  std::string  s = "";
  s +=hex_dealer->to_String_X2(header);
  s +=hex_dealer->to_String_X2(dataLength);
  s +=hex_dealer->to_String_X2(direction);
  s +=hex_dealer->to_String_X2(type);
  s +=hex_dealer->calcCurrentTime(now);
  s +=hex_dealer->to_String_X4(CurrVal);
  s +=hex_dealer->to_String_X4(VoltVal);
  s +=hex_dealer->to_String_X4(TempVal);
  s +=hex_dealer->to_String_X4(HumVal);
  s +=hex_dealer->to_String_X4(0);
  s +=hex_dealer->to_String_X2(footer);
  int len = 0;
  hex_dealer->StringToByteArrayToSend(s, s.length(), dataInChar);
  
  for (int i = 0; i < s.length() / 2 - 5; i++)
  {
    pData[i] = (uint8_t)dataInChar[i + 2];
    len++;
  }
  uint16_t chSum =hex_dealer->GetCrc16(pData, len);
  s = "";
  s +=hex_dealer->to_String_X2(header);
  s +=hex_dealer->to_String_X2(dataLength);
  s +=hex_dealer->to_String_X2(direction);
  s +=hex_dealer->to_String_X2(type);
  s +=hex_dealer->calcCurrentTime(now);
  s +=hex_dealer->to_String_X4(CurrVal);
  s +=hex_dealer->to_String_X4(VoltVal);
  s +=hex_dealer->to_String_X4(TempVal);
  s +=hex_dealer->to_String_X4(HumVal);
  s +=hex_dealer->to_String_X4(chSum);
  s +=hex_dealer->to_String_X2(footer);
  return s;
}
std::string genalarmMessageresponse(int numAlarm,int CurrVal, int VoltVal, int TempVal, int HumVal, int Talarm[])
{
    uint8_t pData[1024] = { 0 };
    char dataInChar[1024] = "";
    int header = 255;
    int dataLength = 20 + numAlarm;
    int direction = 0;
    int type = 4;
    int footer = 205;
    time_t now = time(0);
    std::string  s ="";      
    s += hex_dealer->to_String_X2(header);
    s += hex_dealer->to_String_X2(dataLength);
    s += hex_dealer->to_String_X2(direction);
    s += hex_dealer->to_String_X2(type);
    s += hex_dealer->calcCurrentTime(now);
    s += hex_dealer->to_String_X2(numAlarm);  
    for (int i = 0; i < numAlarm; i++)
    {
        s += hex_dealer->to_String_X2(Talarm[i]);
    }
    s += hex_dealer->to_String_X4(CurrVal);
    s += hex_dealer->to_String_X4(VoltVal);
    s += hex_dealer->to_String_X4(TempVal);
    s += hex_dealer->to_String_X4(HumVal);
    s += hex_dealer->to_String_X4(0);
    s += hex_dealer->to_String_X2(footer);
    int len = 0;
    hex_dealer->StringToByteArrayToSend(s, s.length(), dataInChar);
    for (int i = 0; i < s.length() / 2 - 5; i++)
    {
        pData[i] = (uint8_t)dataInChar[i + 2];
        len++;
    }
    uint16_t chSum = hex_dealer->GetCrc16(pData, len);
    s = "";
    s += hex_dealer->to_String_X2(header);
    s += hex_dealer->to_String_X2(dataLength);
    s += hex_dealer->to_String_X2(direction);
    s += hex_dealer->to_String_X2(type);
    s += hex_dealer->calcCurrentTime(now);
    s += hex_dealer->to_String_X2(numAlarm);
    for (int i = 0; i < numAlarm; i++)
    {
        s += hex_dealer->to_String_X2(Talarm[i]);
    }
    s += hex_dealer->to_String_X4(CurrVal);
    s += hex_dealer->to_String_X4(VoltVal);
    s += hex_dealer->to_String_X4(TempVal);
    s += hex_dealer->to_String_X4(HumVal);
    s += hex_dealer->to_String_X4(chSum);
    s += hex_dealer->to_String_X2(footer);

    return s;

}

#endif

#ifndef generate_control_messages_to_device_functions

std::string genCurrControlMessage(int minValcurr, int maxValcurr)
{
  uint8_t pData[1024] = { 0 };
  char* dataInChar;
  int header = 255;
  int dataLength =15 ;
  int direction = 0;  //server to client
  int type = 5;
  int footer = 205;

  std::string  s = "";
  s += hex_dealer->to_String_X2(header);
  s += hex_dealer->to_String_X2(dataLength);
  s += hex_dealer->to_String_X2(direction);
  s += hex_dealer->to_String_X2(type);
  s += hex_dealer->calcCurrentTime(time(0));
  s += hex_dealer->to_String_X4(minValcurr);
  s += hex_dealer->to_String_X4(maxValcurr);
  s += hex_dealer->to_String_X4(0);
  s += hex_dealer->to_String_X2(footer);
  int len = 0;
  dataInChar = hex_dealer->StringToByteArray(s, s.length());
  for (int i = 0; i < s.length() / 2 - 5; i++)
  {
    pData[i] = (uint8_t)dataInChar[i + 2];
    len++;
  }
  uint16_t chSum = hex_dealer->GetCrc16(pData, len);
  s = "";
  s +=hex_dealer->to_String_X2(header);
  s +=hex_dealer-> to_String_X2(dataLength);
  s +=hex_dealer->to_String_X2(direction);
  s +=hex_dealer-> to_String_X2(type);
  s +=hex_dealer-> calcCurrentTime(time(0));
  s +=hex_dealer-> to_String_X4(minValcurr);
  s +=hex_dealer-> to_String_X4(maxValcurr);
  s +=hex_dealer-> to_String_X4(chSum);
  s +=hex_dealer-> to_String_X2(footer);
  return s;
}
std::string genVoltControlMessage(int minValvolt, int maxValvolt)
{
  uint8_t pData[1024] = { 0 };
  char* dataInChar;
  int header = 255;
  int dataLength = 15;
  int direction = 0;  //server to client
  int type = 6;
  int footer = 205;

  std::string  s = "";
  s +=hex_dealer-> to_String_X2(header);
  s +=hex_dealer->to_String_X2(dataLength);
  s +=hex_dealer->to_String_X2(direction);
  s +=hex_dealer-> to_String_X2(type);
  s +=hex_dealer-> calcCurrentTime(time(0));
  s +=hex_dealer-> to_String_X4(minValvolt);
  s +=hex_dealer-> to_String_X4(maxValvolt);
  s +=hex_dealer-> to_String_X4(0);
  s +=hex_dealer-> to_String_X2(footer);
  int len = 0;
  dataInChar = hex_dealer->StringToByteArray(s, s.length());
  for (int i = 0; i < s.length() / 2 - 5; i++)
  {
    pData[i] = (uint8_t)dataInChar[i + 2];
    len++;
  }
  uint16_t chSum = hex_dealer->GetCrc16(pData, len);
  s = "";
  s +=hex_dealer-> to_String_X2(header);
  s +=hex_dealer-> to_String_X2(dataLength);
  s +=hex_dealer-> to_String_X2(direction);
  s +=hex_dealer-> to_String_X2(type);
  s +=hex_dealer-> calcCurrentTime(time(0));
  s +=hex_dealer-> to_String_X4(minValvolt);
  s +=hex_dealer-> to_String_X4(maxValvolt);
  s +=hex_dealer-> to_String_X4(chSum);
  s += hex_dealer->to_String_X2(footer);
  return s;
}
std::string gentempControlMessage(int minValtemp, int maxValtemp)
{
    uint8_t pData[1024] = { 0 };
  char* dataInChar;
  int header = 255;
  int dataLength = 15;
  int direction = 0;  //server to client
  int type = 7;
  int footer = 205;

  std::string  s = "";
  s +=hex_dealer-> to_String_X2(header);
  s +=hex_dealer->to_String_X2(dataLength);
  s +=hex_dealer->to_String_X2(direction);
  s +=hex_dealer-> to_String_X2(type);
  s +=hex_dealer-> calcCurrentTime(time(0));
  s +=hex_dealer-> to_String_X4(minValtemp);
  s +=hex_dealer-> to_String_X4(maxValtemp);
  s +=hex_dealer-> to_String_X4(0);
  s +=hex_dealer-> to_String_X2(footer);
  int len = 0;
  dataInChar = hex_dealer->StringToByteArray(s, s.length());
  for (int i = 0; i < s.length() / 2 - 5; i++)
  {
    pData[i] = (uint8_t)dataInChar[i + 2];
    len++;
  }
  uint16_t chSum = hex_dealer->GetCrc16(pData, len);
  s = "";
  s +=hex_dealer-> to_String_X2(header);
  s +=hex_dealer-> to_String_X2(dataLength);
  s +=hex_dealer-> to_String_X2(direction);
  s +=hex_dealer-> to_String_X2(type);
  s +=hex_dealer-> calcCurrentTime(time(0));
  s +=hex_dealer-> to_String_X4(minValtemp);
  s +=hex_dealer-> to_String_X4(maxValtemp);
  s +=hex_dealer-> to_String_X4(chSum);
  s += hex_dealer->to_String_X2(footer);
  return s;
}
std::string genHumidityControlMessage(int minValhumidity, int maxValhumidity)
{
  uint8_t pData[1024] = { 0 };
  char* dataInChar;
  int header = 255;
  int dataLength = 15;
  int direction = 0;  //server to client
  int type = 8;
  int footer = 205;

  std::string  s = "";
  s +=hex_dealer-> to_String_X2(header);
  s +=hex_dealer->to_String_X2(dataLength);
  s +=hex_dealer->to_String_X2(direction);
  s +=hex_dealer-> to_String_X2(type);
  s +=hex_dealer-> calcCurrentTime(time(0));
  s +=hex_dealer-> to_String_X4(minValhumidity);
  s +=hex_dealer-> to_String_X4(maxValhumidity);
  s +=hex_dealer-> to_String_X4(0);
  s +=hex_dealer-> to_String_X2(footer);
  int len = 0;
  dataInChar = hex_dealer->StringToByteArray(s, s.length());
  for (int i = 0; i < s.length() / 2 - 5; i++)
  {
    pData[i] = (uint8_t)dataInChar[i + 2];
    len++;
  }
  uint16_t chSum =hex_dealer-> GetCrc16(pData, len);
  s = "";
  s +=hex_dealer-> to_String_X2(header);
  s +=hex_dealer-> to_String_X2(dataLength);
  s +=hex_dealer-> to_String_X2(direction);
  s +=hex_dealer-> to_String_X2(type);
  s +=hex_dealer-> calcCurrentTime(time(0));
  s +=hex_dealer-> to_String_X4(minValhumidity);
  s +=hex_dealer-> to_String_X4(maxValhumidity);
  s +=hex_dealer-> to_String_X4(chSum);
  s += hex_dealer->to_String_X2(footer);
  return s;
}

#endif

int check_imei_validation(std::string curr_imei)
{
    int result = -1;
    if(solar_pannel.select_solar_pannel_by_imei(curr_imei) == true)
    {
        result = solar_pannel.id;
    }
    else
    {
        result = -1;
    }
    return result;
}

std::string gen_alarm_type(int Talarm[], int len)
{
    std::string result = "";
    for(int i = 0; i < len; i++)
    {
        if(i != len - 1)
        {
            result += std::to_string(Talarm[i]) + ",";
        }
        else
        {
            result += std::to_string(Talarm[i]);
        }
    }
    return result;
}

std::vector<bool> get_user_privilages(int user_id)
{
    std::vector<bool> result;
    privilages privs(mysql);
    std::vector<privilages> user_privilages = privs.get_all_user_privilages(user_id);
    std::cout << "num user_privilages = " << user_privilages.size() << std::endl;
    bool add_priv = false;
    bool remove_priv = false;
    bool update_priv = false;
    for(int i = 0; i < user_privilages.size(); i++)
    {
        if(user_privilages[i].priv_type == 1)
        {
            add_priv = true;
        }
        else if(user_privilages[i].priv_type == 2)
        {
           remove_priv = true; 
        }
        else if(user_privilages[i].priv_type == 3)
        {
            update_priv = true;
        }
    }
    result.push_back(add_priv);
    result.push_back(remove_priv);
    result.push_back(update_priv);
    return result;
}

bool connect_to_database()
{
    try
    {
        mysql = mysql_init(NULL);
        if(mysql == NULL)
        {
            fprintf(stderr, "ABB : %s\n", mysql_error(mysql));
            std::cout << "error with mysql initialization" << std::endl;
            return 0;
        }
    }
    catch(...)
    {
        std::cout << "error with mysql initialization (catch _err)" << std::endl;
        return false;
    }
    
    try
    {
        my_bool reconnect= 1;
        unsigned int timeout= 10;
        long long mysql_buffer_length= 0x400000000;
        std::cout << "MYSQL_OPT_RECONNECT set option = " << mysql_options(mysql, MYSQL_OPT_RECONNECT, (void *)&reconnect) << "\n";
        std::cout << "MYSQL_OPT_CONNECT_TIMEOUT set option = " << mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, (void *)&timeout) << "\n";
        std::cout << "MYSQL_OPT_READ_TIMEOUT set option = " << mysql_options(mysql, MYSQL_OPT_READ_TIMEOUT, (void *)&timeout) << "\n";
        std::cout << "MYSQL_OPT_WRITE_TIMEOUT set option = " << mysql_options(mysql, MYSQL_OPT_WRITE_TIMEOUT, (void *)&timeout) << "\n";
    }
    catch(...)
    {
        std::cout << "error in setting mysql connection timeout\n";
        return false;
    }
    
    try
    {
        enum mysql_protocol_type prot_type= MYSQL_PROTOCOL_TCP;
        mysql_options(mysql, MYSQL_OPT_PROTOCOL, (void *)&prot_type);
    }
    catch(...)
    {
        std::cout << "error in setting mysql connection protocol\n";
        return false;
    }
    
    try
    {
        if(mysql_real_connect(mysql, "127.0.0.1", "solar_pannel_admin", "123456789", "solar_system_db", 3306, NULL, 0) == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(mysql));
            printf("Database connection failed.\r\n");
            return false;
        }
        else
        {
            printf("Database connection successful.\r\n");
            
        }
    }
    catch(...)
    {
        std::cout << "error in connection with mysql server (catch)\n";
        return false;
    }
    
    return true;
}

int create_server()
{
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
        return -1;
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
        return -2;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
        return -3;
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
        return -4;
    }
    return 0;
}

void print_received_message(char buffer[])
{
    for(int i=0;i<(unsigned char) buffer[1] + 2; i++)
    {
        std::cout << hex_dealer->to_String_X2((unsigned char) buffer[i]);
    }
    std::cout << std::endl;
}

void* process_client(void * arg)
{
    
    int curr_id = *((int *) arg);
    std::cout << "start serving client " << curr_id << std::endl;
    char buffer[1024]="";
    all_measurements_table last_measure(mysql);
    int record_index = last_measure.get_measurment_record_id(curr_id);
    std::cout << "record_index " << record_index << std::endl;
    while(true)
    {
        memset(buffer,0,sizeof(buffer));
        int valread = recv(solar_pannels_collection_card[curr_id].socket, buffer, 1024, 0);
        if(valread > 0)
        {
            int message_type = (unsigned char) buffer[3];
            //print_received_message(buffer);
            solar_pannels_collection_card[curr_id].parser->parse_client_message((unsigned char*)buffer);
            bool correct = solar_pannels_collection_card[curr_id].parser->check_client_message((unsigned char*)buffer);
            if(correct == true)
            {
                switch (message_type)
                {
                    case 1:
                    {
                        char data_login[8192] = "";
                        solar_pannels_collection_card[curr_id].parser->print_login_msg(data_login);
                        solar_pannels_collection_card[curr_id].outfile.open(solar_pannels_collection_card[curr_id].file_name,std::ios_base::app);
                        solar_pannels_collection_card[curr_id].outfile << data_login <<std::endl;
                        solar_pannels_collection_card[curr_id].outfile.close();
                        std::string login_response = genLoginResponse(solar_pannels_collection_card[curr_id].parser->login_params.imei);
                        memset(buffer,0,sizeof(buffer));
                        hex_dealer->StringToByteArrayToSend(login_response, login_response.length(), buffer);
                        send(solar_pannels_collection_card[curr_id].socket , buffer , strlen(login_response.c_str())/2 , 0 );
                        print_received_message(buffer);
                    }break;
                    case 2:
                    {
                          char data_heartbeat[8192] = "";
                        solar_pannels_collection_card[curr_id].parser->print_heartbeat_msg(data_heartbeat);
                        solar_pannels_collection_card[curr_id].outfile.open(solar_pannels_collection_card[curr_id].file_name,std::ios_base::app);
                        solar_pannels_collection_card[curr_id].outfile << data_heartbeat <<std::endl;
                        solar_pannels_collection_card[curr_id].outfile.close();
                        std::string heartbeat_response = genheartbeatMessageresponse(curr_id);
                        memset(buffer,0,sizeof(buffer));
                        hex_dealer->StringToByteArrayToSend(heartbeat_response, heartbeat_response.length(), buffer);
                        send(solar_pannels_collection_card[curr_id].socket , buffer , strlen(heartbeat_response.c_str())/2 , 0 );

                    }break;
                    case 3:
                    {
                        char data_measurment[8192] = "";
                        solar_pannels_collection_card[curr_id].parser->print_measurment_msg (data_measurment);
                        solar_pannels_collection_card[curr_id].outfile.open(solar_pannels_collection_card[curr_id].file_name,std::ios_base::app);
                        solar_pannels_collection_card[curr_id].outfile << data_measurment <<std::endl;
                        solar_pannels_collection_card[curr_id].outfile.close();
                        solar_pannels solar_pannel_measure(mysql);
                        solar_pannel_measure.select_solar_pannel_by_id(curr_id);
                        solar_pannel_measure.current_measure = solar_pannels_collection_card[curr_id].parser->measurment_params.curr;
                        solar_pannel_measure.voltage_measure = solar_pannels_collection_card[curr_id].parser->measurment_params.voltage;
                        solar_pannel_measure.temperature_measure = solar_pannels_collection_card[curr_id].parser->measurment_params.temp;
                        solar_pannel_measure.humidity_measure = solar_pannels_collection_card[curr_id].parser->measurment_params.humidity;
                        solar_pannel_measure.insert_update_solar_pannel();
                        all_measurements_table location_measurment(mysql);
                        location_measurment.id = location_measurment.get_measurment_insert_id();
                        location_measurment.solar_pannel_id = curr_id;
                        location_measurment.current_measure = solar_pannels_collection_card[curr_id].parser->measurment_params.curr;
                        location_measurment.voltage_measure = solar_pannels_collection_card[curr_id].parser->measurment_params.voltage;
                        location_measurment.temperature_measure = solar_pannels_collection_card[curr_id].parser->measurment_params.temp;
                        location_measurment.humidity_measure = solar_pannels_collection_card[curr_id].parser->measurment_params.humidity;
                        location_measurment.alarm_state = 0;
                        location_measurment.record_index = record_index;
                        strcpy(location_measurment.alarm_types,"0,0,0,0,0,0,0,0");
                        location_measurment.date_time = time(0);
                        location_measurment.insert_update_measurement();
                        int last_measure_id = location_measurment.id;
                        location_measurment.select_measurement_by_id_solar_pannel_id(last_measure_id, curr_id);
                        std::string measurment_response = genmeasurmentMessageresponse(location_measurment.current_measure, location_measurment.voltage_measure, location_measurment.temperature_measure, location_measurment.humidity_measure);
                        memset(buffer,0,sizeof(buffer));
                        hex_dealer->StringToByteArrayToSend(measurment_response, measurment_response.length(), buffer);
                        send(solar_pannels_collection_card[curr_id].socket , buffer , strlen(measurment_response.c_str())/2 , 0 );
                    }break;
                    case 4:
                    {
                        char data_alarm[8192] = "";
                        solar_pannels_collection_card[curr_id].parser->print_alarm_msg(data_alarm);
                        solar_pannels solar_pannel_measure(mysql);
                        solar_pannel_measure.select_solar_pannel_by_id(curr_id);
                        solar_pannel_measure.current_measure = solar_pannels_collection_card[curr_id].parser->alarm_params.curr;
                        solar_pannel_measure.voltage_measure = solar_pannels_collection_card[curr_id].parser->alarm_params.voltage;
                        solar_pannel_measure.temperature_measure = solar_pannels_collection_card[curr_id].parser->alarm_params.temp;
                        solar_pannel_measure.humidity_measure = solar_pannels_collection_card[curr_id].parser->alarm_params.humidity;
                        solar_pannel_measure.insert_update_solar_pannel();
                        all_measurements_table alarm_measurment(mysql);
                        alarm_measurment.id = alarm_measurment.get_measurment_insert_id();
                        alarm_measurment.solar_pannel_id = curr_id;
                        alarm_measurment.current_measure = solar_pannels_collection_card[curr_id].parser->alarm_params.curr;
                        alarm_measurment.voltage_measure = solar_pannels_collection_card[curr_id].parser->alarm_params.voltage;
                        alarm_measurment.temperature_measure = solar_pannels_collection_card[curr_id].parser->alarm_params.temp;
                        alarm_measurment.humidity_measure = solar_pannels_collection_card[curr_id].parser->alarm_params.humidity;
                        alarm_measurment.alarm_state = solar_pannels_collection_card[curr_id].parser->alarm_params.numalarm;
                        std::string alarms = gen_alarm_type(solar_pannels_collection_card[curr_id].parser->alarm_params.Talarm, 8);
                        strcpy(alarm_measurment.alarm_types, alarms.c_str());
                        alarm_measurment.record_index = record_index;
                        alarm_measurment.date_time = time(0);
                        alarm_measurment.insert_update_measurement();
                        int last_measure_id = alarm_measurment.id;
                        alarm_measurment.select_measurement_by_id_solar_pannel_id(last_measure_id, curr_id);
                        std::string alarm_response = genalarmMessageresponse(alarm_measurment.alarm_state, alarm_measurment.current_measure, alarm_measurment.voltage_measure, alarm_measurment.temperature_measure, alarm_measurment.humidity_measure,solar_pannels_collection_card[curr_id].parser->alarm_params.Talarm);
                           solar_pannels_collection_card[curr_id].outfile.open(solar_pannels_collection_card[curr_id].file_name,std::ios_base::app);
                        solar_pannels_collection_card[curr_id].outfile << data_alarm <<std::endl;
                        solar_pannels_collection_card[curr_id].outfile.close();
                        //std::cout << "alarm_response done\n";
                        memset(buffer,0,sizeof(buffer));
                        hex_dealer->StringToByteArrayToSend(alarm_response, alarm_response.length(), buffer);
                        //std::cout << "StringToByteArrayToSend done\n";
                        send(solar_pannels_collection_card[curr_id].socket , buffer , strlen(alarm_response.c_str())/2 , 0 );
                        //std::cout << "send done\n";
                    }break;
                    default:{}break;
                }

            }
            else
            {
                printf("error with device\n");
                solar_pannels_collection_card[curr_id].socket = -1;
                solar_pannels thread_solar_pannel(mysql);
                thread_solar_pannel.select_solar_pannel_by_id(curr_id);
                thread_solar_pannel.id = curr_id;
                thread_solar_pannel.state = 0;
                thread_solar_pannel.date_time = time(0);
                solar_pannel.insert_update_solar_pannel();
                break;
            }
        }
        else
        {
            printf("Timeout On Socket\n");
            solar_pannels_collection_card[curr_id].socket = -1;
            solar_pannels thread_solar_pannel(mysql);
            thread_solar_pannel.select_solar_pannel_by_id(curr_id);
            thread_solar_pannel.id = curr_id;
            thread_solar_pannel.state = 0;
            thread_solar_pannel.date_time = time(0);
            thread_solar_pannel.insert_update_solar_pannel();
            break;
        }
    }
}

void* connection_proccess(void * arg)
{
    unsigned char buffer[1024]="";
    struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    while(1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        else
        {
            current_id++;
            if (setsockopt (new_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                    sizeof timeout) < 0)
            {
                perror("setsockopt");
                exit(EXIT_FAILURE);
            }
            memset(buffer,0,sizeof(buffer));
            int valread = recv(new_socket, buffer, 1024, 0);
            if(valread > 0)
            {
                int message_type = (unsigned char) buffer[3];
                //print_received_message(buffer);
                parser_general->parse_client_message((unsigned char*)buffer);
                bool correct = parser_general->check_client_message((unsigned char*)buffer);
                if(correct == true)
                {
                    if(message_type == 1) // procees just login message
                    {
                        int result = check_imei_validation(parser_general->login_params.imei);
                        if(result >= 0)   // imei is valid
                        {
                            solar_pannels_collection_card[result].socket = new_socket;
                            sprintf(solar_pannels_collection_card[result].file_name, "device_%d", result);
                            solar_pannels_collection_card[result].parser->parse_client_message((unsigned char*)buffer);
                            char data_login[8192] = "";
                            solar_pannels_collection_card[result].parser->print_login_msg(data_login);
                            solar_pannels_collection_card[result].outfile.open(solar_pannels_collection_card[result].file_name,std::ios_base::app);
                            solar_pannels_collection_card[result].outfile << data_login <<std::endl;
                            solar_pannels_collection_card[result].outfile.close();
                            std::string login_response = genLoginResponse(solar_pannels_collection_card[result].parser->login_params.imei);
                            memset(buffer,0,sizeof(buffer));
                            solar_pannel.id = result;
                            solar_pannel.state = 1;
                            solar_pannel.date_time = time(0);
                            solar_pannel.insert_update_solar_pannel();
                            hex_dealer->StringToByteArrayToSend(login_response, login_response.length(), (char *) buffer);
                            send(solar_pannels_collection_card[result].socket , buffer , strlen(login_response.c_str())/2 , 0 );
                            pthread_create(&my_thread[result], NULL, process_client, &result);
                        }
                        else              // not valid imei
                        {
                            new_socket = -1;
                            std::cout<<"(not valid ieme)\n";
                        }
                    }
                    else                  // received message is not login 
                    {
                        new_socket = -1;
                        std::cout<<"fuck mother client(received message is not login )\n";
                    }
                }
                else                      // RECEIVED MESSAGE IS FALSE
                {
                    new_socket = -1;
                     std::cout<<"fuck mother client(RECEIVED MESSAGE IS FALSE)\n";
                }
            }
            else                         // Timeout On Socket
            {
                printf("Timeout On Socket\n");
                new_socket = -1;
            }
            
            
        }
    }
    
}
    
int main(int argc, char** argv) {
    current_id = 1;
    
    if(connect_to_database() == true)
    {
        std::cout << "database connection successfull\n";
    }
    else
    {
        std::cout << "database connection failed\n";
        exit(0);
    }
    
    user.mysql = mysql;
    privilage.mysql = mysql;
    solar_pannel.mysql = mysql;
    solar_pannel_setting.mysql = mysql;
    measurments.mysql = mysql;
    
    std::vector<solar_pannels> device_solar = solar_pannel.read_all_solar_pannels(mysql);
    for(int i = 0; i < device_solar.size(); i++)
    {
        device_solar[i].state = 0;
        device_solar[i].insert_update_solar_pannel();
    }
    
    if(create_server() == 0)
    {
        pthread_create(&connection_thread, NULL, connection_proccess, NULL);
    }
    
    pthread_create(&Http_Server_Thread, NULL, Http_Server_Process, NULL);
    
    while(true)
    {
        sleep(3000);
    }
    return 0;
}

