#include "usefull_structures.h"
#include "HexDealer.h"

#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

class MessageParser {
public:
    login_data login_params;
    heartbeat_data heartbeat_params;
    measurment_data measurment_params;
    alarm_data alarm_params;
    control_curr_data control_curr_params;
    control_voltage_data control_voltage_params;
    control_temp_data control_temp_params;
    control_humidity_data control_humidity_params;
    
            
    HexDealer* hex_dealer = new HexDealer();
    MessageParser();
    virtual ~MessageParser();
    void parse_client_message(unsigned char message[]);
    void parse_server_message(unsigned char message[]);
    bool check_client_message(unsigned char message[]);
    bool check_server_message(unsigned char message[]);
    void print_login_msg(char data_login[]);
    void print_heartbeat_msg (char data_heartbeat[]);
    void print_measurment_msg (char data_measurment[]);
    void print_alarm_msg(char data_alarm[]);
    
private:

};

#endif /* MESSAGEPARSER_H */

