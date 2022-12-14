/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MessageParser.cpp
 * Author: mohammad
 * 
 * Created on April 9, 2022, 11:31 PM
 */

#include "MessageParser.h"


MessageParser::MessageParser() {
}

MessageParser::~MessageParser() {
}

void MessageParser::parse_client_message(unsigned char message[])
{
    int message_type = (unsigned char) message[3];
    
    switch(message_type)
    {
        case 1:                 // login message
        {
            login_params.header = -1;
            login_params.data_length = -1;
            login_params.direction = -1;
            login_params.type = -1;
            login_params.current_time.year   = -1;
            login_params.current_time.month  = -1;
            login_params.current_time.day    = -1;
            login_params.current_time.hour   = -1;
            login_params.current_time.minute = -1;
            login_params.current_time.second = -1;
            login_params.imei = "";
            login_params.check_sum = -1;
            login_params.footer = -1;
            
            login_params.header = (unsigned char) message[0];
            login_params.data_length = (unsigned char) message[1];
            login_params.direction = (unsigned char) message[2];
            login_params.type = (unsigned char) message[3];
            login_params.current_time.year = (unsigned char) message[4];
            login_params.current_time.month = (unsigned char) message[5];
            login_params.current_time.day = (unsigned char) message[6];
            login_params.current_time.hour = (unsigned char) message[7];
            login_params.current_time.minute = (unsigned char) message[8];
            login_params.current_time.second = (unsigned char) message[9];
            for(int i = 0; i < 15; i++)
            {
                login_params.imei += message[10 + i];
            }
            login_params.check_sum = ((unsigned char) message[25])*256
                    + (unsigned char) message[26];
            login_params.footer = (unsigned char) message[27];
        }break;
        case 2:                 //heartbeat message
        {
            heartbeat_params.header = -1;
            heartbeat_params.data_length = -1;
            heartbeat_params.direction = -1;
            heartbeat_params.type = -1;
            heartbeat_params.current_time.year   = -1;
            heartbeat_params.current_time.month  = -1;
            heartbeat_params.current_time.day    = -1;
            heartbeat_params.current_time.hour   = -1;
            heartbeat_params.current_time.minute = -1;
            heartbeat_params.current_time.second = -1;
            heartbeat_params.content = -1;
            heartbeat_params.check_sum = -1;
            heartbeat_params.footer = -1;
            
            
            heartbeat_params.header = (unsigned char) message[0];
            heartbeat_params.data_length = (unsigned char) message[1];
            heartbeat_params.direction = (unsigned char) message[2];
            heartbeat_params.type = (unsigned char) message[3];
            heartbeat_params.current_time.year = (unsigned char) message[4];
            heartbeat_params.current_time.month = (unsigned char) message[5];
            heartbeat_params.current_time.day = (unsigned char) message[6];
            heartbeat_params.current_time.hour = (unsigned char) message[7];
            heartbeat_params.current_time.minute = (unsigned char) message[8];
            heartbeat_params.current_time.second = (unsigned char) message[9];
            heartbeat_params.content=(unsigned char) message[10];
            heartbeat_params.check_sum = ((unsigned char) message[11])*256
                    + (unsigned char) message[12];
            heartbeat_params.footer = (unsigned char) message[13];    
        
        }break;
        case 3:                     //measurement message
        {
            measurment_params.header = -1;
            measurment_params.data_length = -1;
            measurment_params.direction =-1;
            measurment_params.type = -1;
            measurment_params.current_time.year = -1;
            measurment_params.current_time.month = -1;
            measurment_params.current_time.day = -1;
            measurment_params.current_time.hour = -1;
            measurment_params.current_time.minute = -1;
            measurment_params.current_time.second = -1;
            measurment_params.curr=-1;
            measurment_params.voltage=-1;
            measurment_params.temp=-1;
            measurment_params.humidity=-1;
            measurment_params.check_sum =-1;
            measurment_params.footer = -1;
            
            measurment_params.header = (unsigned char) message[0];
            measurment_params.data_length = (unsigned char) message[1];
            measurment_params.direction = (unsigned char) message[2];
            measurment_params.type = (unsigned char) message[3];
            measurment_params.current_time.year = (unsigned char) message[4];
            measurment_params.current_time.month = (unsigned char) message[5];
            measurment_params.current_time.day = (unsigned char) message[6];
            measurment_params.current_time.hour = (unsigned char) message[7];
            measurment_params.current_time.minute = (unsigned char) message[8];
            measurment_params.current_time.second = (unsigned char) message[9];
            measurment_params.curr=((unsigned char) message[10])*256 + (unsigned char) message[11];
            measurment_params.voltage=((unsigned char) message[12])*256+(unsigned char) message[13];
            measurment_params.temp=((unsigned char) message[14])*256+(unsigned char) message[15];
            measurment_params.humidity=((unsigned char) message[16])*256+(unsigned char) message[17];
            measurment_params.check_sum = ((unsigned char) message[18])*256
                    + (unsigned char) message[19];
            measurment_params.footer = (unsigned char) message[20];
            
        }break;
        case 4:
        {
            alarm_params.header = -1;
            alarm_params.data_length =-1;
            alarm_params.direction = -1;
            alarm_params.type = -1;
            alarm_params.current_time.year =-1;
            alarm_params.current_time.month = -1;
            alarm_params.current_time.day = -1;
            alarm_params.current_time.hour = -1;
            alarm_params.current_time.minute = -1;
            alarm_params.current_time.second = -1;
            alarm_params.numalarm = (unsigned char) message[10];
            for(int i = 0; i < alarm_params.numalarm; i++)
            {
                alarm_params.Talarm[i] = -1;
            }
            alarm_params.curr=-1;
            alarm_params.voltage=-1;
            alarm_params.temp=-1;
            alarm_params.humidity=-1;
            alarm_params.check_sum = -1;
            alarm_params.footer = -1;
            
            alarm_params.header = (unsigned char) message[0];
            alarm_params.data_length = (unsigned char) message[1];
            alarm_params.direction = (unsigned char) message[2];
            alarm_params.type = (unsigned char) message[3];
            alarm_params.current_time.year = (unsigned char) message[4];
            alarm_params.current_time.month = (unsigned char) message[5];
            alarm_params.current_time.day = (unsigned char) message[6];
            alarm_params.current_time.hour = (unsigned char) message[7];
            alarm_params.current_time.minute = (unsigned char) message[8];
            alarm_params.current_time.second = (unsigned char) message[9];
            alarm_params.numalarm = (unsigned char) message[10];
            for(int i = 0; i < alarm_params.numalarm; i++)
            {
                alarm_params.Talarm[i] = (unsigned char) message[11 + i];
            }
            alarm_params.curr=((unsigned char) message[11 + alarm_params.numalarm])*256 + (unsigned char) message[12 + alarm_params.numalarm];
            alarm_params.voltage=((unsigned char) message[13 + alarm_params.numalarm])*256+(unsigned char) message[14 + alarm_params.numalarm];
            alarm_params.temp=((unsigned char) message[15 + alarm_params.numalarm])*256+(unsigned char) message[16 + alarm_params.numalarm];
            alarm_params.humidity=((unsigned char) message[17 + alarm_params.numalarm])*256+(unsigned char) message[18 + alarm_params.numalarm];
            alarm_params.check_sum = ((unsigned char) message[19 + alarm_params.numalarm])*256 +(unsigned char) message[20 + alarm_params.numalarm];
            alarm_params.footer = (unsigned char) message[21 + alarm_params.numalarm];
        }break;
        default:
        {
            
        }break;
    }
}
bool MessageParser::check_client_message(unsigned char message[])
{
    bool result = false;
    int message_type = (unsigned char) message[3];
    uint8_t pData[1024] = { 0 };
    switch(message_type)
    {
        case 1:                 // login message
        {
            int len = 0;
            for(int i=0;i<login_params.data_length - 3; i++)
            {
                pData[i] = (uint8_t)message[i + 2];
                len ++;
            }
            
            uint16_t recievedCRC = hex_dealer->GetCrc16(pData, len);
            
            //std::cout << "recievedCRC = " << recievedCRC << std::endl;
            //std::cout << "calculatedCRC = " << login_params.check_sum << std::endl;
            if(recievedCRC == login_params.check_sum)
            {
                return true;
            }
            else
            {
                return false;
            }
            
        }break;
        case 2:
        {
             int len = 0;
            for(int i=0;i<heartbeat_params.data_length - 3; i++)
            {
                pData[i] = (uint8_t)message[i + 2];
                len ++;
            }
            
            uint16_t recievedCRC = hex_dealer->GetCrc16(pData, len);
            
            //std::cout << "recievedCRC = " << recievedCRC << std::endl;
            //std::cout << "calculatedCRC = " << heartbeat_params.check_sum << std::endl;
            if(recievedCRC == heartbeat_params.check_sum)
            {
                return true;
            }
            else
            {
                return false;
            }
            
            
        }break;
        case 3:
        {
            int len = 0;
            for(int i=0;i<measurment_params.data_length - 3; i++)
            {
                pData[i] = (uint8_t)message[i + 2];
                len ++;
            }
            
            uint16_t recievedCRC = hex_dealer->GetCrc16(pData, len);
            
            //std::cout << "recievedCRC = " << recievedCRC << std::endl;
            //std::cout << "calculatedCRC = " << measurment_params.check_sum << std::endl;
            if(recievedCRC == measurment_params.check_sum)
            {
                return true;
            }
            else
            {
                return false;
            }
            
        }break;
        case 4:
        {
            int len = 0;
            for(int i=0;i<alarm_params.data_length - 3; i++)
            {
                pData[i] = (uint8_t)message[i + 2];
                len ++;
            }
            
            uint16_t recievedCRC = hex_dealer->GetCrc16(pData, len);
            
            ///std::cout << "recievedCRC = " << recievedCRC << std::endl;
            //std::cout << "calculatedCRC = " << alarm_params.check_sum << std::endl;
            if(recievedCRC == alarm_params.check_sum)
            {
                return true;
            }
            else
            {
                return false;
            }
            
        }break;
        default:
        {
            
        }break;
    }
    
    return result;
}
void MessageParser::parse_server_message(unsigned char message[])
{
 int message_type = (unsigned char) message[3];
    
    switch(message_type)
    {
        case 5:
        {
            
            control_curr_params.header = -1;
            control_curr_params.data_length = -1;
            control_curr_params.direction = -1;
            control_curr_params.type = -1;
            control_curr_params.current_time.year   = -1;
            control_curr_params.current_time.month  = -1;
            control_curr_params.current_time.day    = -1;
            control_curr_params.current_time.hour   = -1;
            control_curr_params.current_time.minute = -1;
            control_curr_params.current_time.second = -1;
            control_curr_params.minvalcurr = -1;
            control_curr_params.maxvalcurr = -1;
            control_curr_params.check_sum = -1;
            control_curr_params.footer = -1;
            
            
            control_curr_params.header = (unsigned char) message[0];
            control_curr_params.data_length = (unsigned char) message[1];
            control_curr_params.direction = (unsigned char) message[2];
            control_curr_params.type = (unsigned char) message[3];
            control_curr_params.current_time.year = (unsigned char) message[4];
            control_curr_params.current_time.month = (unsigned char) message[5];
            control_curr_params.current_time.day = (unsigned char) message[6];
            control_curr_params.current_time.hour = (unsigned char) message[7];
            control_curr_params.current_time.minute = (unsigned char) message[8];
            control_curr_params.current_time.second = (unsigned char) message[9];
            control_curr_params.minvalcurr=(unsigned char) message[10]*256+(unsigned char) message[11];
            control_curr_params.maxvalcurr=(unsigned char) message[12]*256+(unsigned char) message[13];
            control_curr_params.check_sum = ((unsigned char) message[14])*256
                    + (unsigned char) message[15];
            control_curr_params.footer = (unsigned char) message[16];    
        
            
        }   break;
        case 6:
        {
            control_voltage_params.header = -1;
            control_voltage_params.data_length = -1;
            control_voltage_params.direction = -1;
            control_voltage_params.type = -1;
            control_voltage_params.current_time.year   = -1;
            control_voltage_params.current_time.month  = -1;
            control_voltage_params.current_time.day    = -1;
            control_voltage_params.current_time.hour   = -1;
            control_voltage_params.current_time.minute = -1;
            control_voltage_params.current_time.second = -1;
            control_voltage_params.minvalvoltage= -1;
            control_voltage_params.maxvalvoltage = -1;
            control_voltage_params.check_sum = -1;
            control_voltage_params.footer = -1;
            
            
            control_voltage_params.header = (unsigned char) message[0];
            control_voltage_params.data_length = (unsigned char) message[1];
            control_voltage_params.direction = (unsigned char) message[2];
            control_voltage_params.type = (unsigned char) message[3];
            control_voltage_params.current_time.year = (unsigned char) message[4];
            control_voltage_params.current_time.month = (unsigned char) message[5];
            control_voltage_params.current_time.day = (unsigned char) message[6];
            control_voltage_params.current_time.hour = (unsigned char) message[7];
            control_voltage_params.current_time.minute = (unsigned char) message[8];
            control_voltage_params.current_time.second = (unsigned char) message[9];
            control_voltage_params.minvalvoltage=(unsigned char) message[10]*256+(unsigned char) message[11];
            control_voltage_params.maxvalvoltage=(unsigned char) message[12]*256+(unsigned char) message[13];
            control_voltage_params.check_sum = ((unsigned char) message[14])*256
                    + (unsigned char) message[15];
            control_voltage_params.footer = (unsigned char) message[16];    
        
        }   break;
        case 7:
        {
            control_temp_params.header = -1;
            control_temp_params.data_length = -1;
            control_temp_params.direction = -1;
            control_temp_params.type = -1;
            control_temp_params.current_time.year   = -1;
            control_temp_params.current_time.month  = -1;
            control_temp_params.current_time.day    = -1;
            control_temp_params.current_time.hour   = -1;
            control_temp_params.current_time.minute = -1;
            control_temp_params.current_time.second = -1;
            control_temp_params.minvaltemp= -1;
            control_temp_params.maxvaltemp = -1;
            control_temp_params.check_sum = -1;
            control_temp_params.footer = -1;
            
            
            control_temp_params.header = (unsigned char) message[0];
            control_temp_params.data_length = (unsigned char) message[1];
            control_temp_params.direction = (unsigned char) message[2];
            control_temp_params.type = (unsigned char) message[3];
            control_temp_params.current_time.year = (unsigned char) message[4];
            control_temp_params.current_time.month = (unsigned char) message[5];
            control_temp_params.current_time.day = (unsigned char) message[6];
            control_temp_params.current_time.hour = (unsigned char) message[7];
            control_temp_params.current_time.minute = (unsigned char) message[8];
            control_temp_params.current_time.second = (unsigned char) message[9];
            control_temp_params.minvaltemp=(unsigned char) message[10]*256+(unsigned char) message[11];
            control_temp_params.maxvaltemp=(unsigned char) message[12]*256+(unsigned char) message[13];
            control_temp_params.check_sum = ((unsigned char) message[14])*256
                    + (unsigned char) message[15];
            control_temp_params.footer = (unsigned char) message[16];   
        }   break;
        case 8:
        {
            control_humidity_params.header = -1;
            control_humidity_params.data_length = -1;
            control_humidity_params.direction = -1;
            control_humidity_params.type = -1;
            control_humidity_params.current_time.year   = -1;
            control_humidity_params.current_time.month  = -1;
            control_humidity_params.current_time.day    = -1;
            control_humidity_params.current_time.hour   = -1;
            control_humidity_params.current_time.minute = -1;
            control_humidity_params.current_time.second = -1;
            control_humidity_params.minvalhumidity= -1;
            control_humidity_params.maxvalhumidity = -1;
            control_humidity_params.check_sum = -1;
            control_humidity_params.footer = -1;
            
            
            control_humidity_params.header = (unsigned char) message[0];
            control_humidity_params.data_length = (unsigned char) message[1];
            control_humidity_params.direction = (unsigned char) message[2];
            control_humidity_params.type = (unsigned char) message[3];
            control_humidity_params.current_time.year = (unsigned char) message[4];
            control_humidity_params.current_time.month = (unsigned char) message[5];
            control_humidity_params.current_time.day = (unsigned char) message[6];
            control_humidity_params.current_time.hour = (unsigned char) message[7];
            control_humidity_params.current_time.minute = (unsigned char) message[8];
            control_humidity_params.current_time.second = (unsigned char) message[9];
            control_humidity_params.minvalhumidity=(unsigned char) message[10]*256+(unsigned char) message[11];
            control_humidity_params.maxvalhumidity=(unsigned char) message[12]*256+(unsigned char) message[13];
            control_temp_params.check_sum = ((unsigned char) message[14])*256
                    + (unsigned char) message[15];
            control_temp_params.footer = (unsigned char) message[16]; 
        }   break;
        default :
        {
            
        }   break;
    }
}
bool MessageParser::check_server_message(unsigned char message[])
{
    bool result = false;
    int message_type = (unsigned char) message[3];
    uint8_t pData[1024] = { 0 };
    switch(message_type)
    {
        case 5:
        {
             int len = 0;
            for(int i=0;i<control_curr_params.data_length - 3; i++)
            {
                pData[i] = (uint8_t)message[i + 2];
                len ++;
            }
            
            uint16_t recievedCRC = hex_dealer->GetCrc16(pData, len);
            
            //std::cout << "recievedCRC = " << recievedCRC << std::endl;
            //std::cout << "calculatedCRC = " << control_curr_params.check_sum << std::endl;
            if(recievedCRC == control_curr_params.check_sum)
            {
                return true;
            }
            else
            {
                return false;
            }
        }break;
        case 6:
        {
            int len = 0;
            for(int i=0;i<control_voltage_params.data_length - 3; i++)
            {
                pData[i] = (uint8_t)message[i + 2];
                len ++;
            }
            
            uint16_t recievedCRC = hex_dealer->GetCrc16(pData, len);
            
            //std::cout << "recievedCRC = " << recievedCRC << std::endl;
            //std::cout << "calculatedCRC = " << control_voltage_params.check_sum << std::endl;
            if(recievedCRC == control_voltage_params.check_sum)
            {
                return true;
            }
            else
            {
                return false;
            }
            
            
        }break;
        case 7:
        {
            int len = 0;
            for(int i=0;i<control_temp_params.data_length - 3; i++)
            {
                pData[i] = (uint8_t)message[i + 2];
                len ++;
            }
            
            uint16_t recievedCRC = hex_dealer->GetCrc16(pData, len);
            
            //std::cout << "recievedCRC = " << recievedCRC << std::endl;
            //std::cout << "calculatedCRC = " << control_temp_params.check_sum << std::endl;
            if(recievedCRC == control_temp_params.check_sum)
            {
                return true;
            }
            else
            {
                return false;
            }
        }break;
        case 8:
        {
            int len = 0;
            for(int i=0;i<control_humidity_params.data_length - 3; i++)
            {
                pData[i] = (uint8_t)message[i + 2];
                len ++;
            }
            
            uint16_t recievedCRC = hex_dealer->GetCrc16(pData, len);
            
            ///std::cout << "recievedCRC = " << recievedCRC << std::endl;
            //std::cout << "calculatedCRC = " << control_humidity_params.check_sum << std::endl;
            if(recievedCRC == control_humidity_params.check_sum)
            {
                return true;
            }
            else
            {
                return false;
            }
        }break;
        default :
        {
        }break;
        
    }
     return result;
}

#ifndef print message
void MessageParser:: print_login_msg(char data_login[])
{
    memset(data_login,0,sizeof(data_login));
    char current_line[1024] = "";
    sprintf(current_line, "Login Message:\n");
    strcat(data_login, current_line);
    sprintf(current_line, "Server Time: %s\n", (hex_dealer->calcCurrentTime(time(0))).c_str());
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "header=", login_params.header);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "data length=", login_params.data_length);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "direction=", login_params.direction);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "type=", login_params.type);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "current_time.year=", login_params.current_time.year);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "current_time.month=", login_params.current_time.month);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "current_time.day=", login_params.current_time.day);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "current_time.hour=", login_params.current_time.hour);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "current_time.minute=", login_params.current_time.minute);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "current_time.second=", login_params.current_time.second);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %s\n", "imei=", login_params.imei.c_str());
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "check_sum=", login_params.check_sum);
    strcat(data_login, current_line);
    sprintf(current_line, "%s %d\n", "footer=", login_params.footer);
    strcat(data_login, current_line);
    sprintf(current_line, "##############################################\n");
    strcat(data_login, current_line);
}

void MessageParser::print_measurment_msg (char data_measurment[])
{
   memset(data_measurment,0,sizeof(data_measurment));
    char current_line[1024] = "";
    sprintf(current_line, "measurment Message:\n");
    strcat(data_measurment, current_line);
    sprintf(current_line, "Server Time: %s\n", (hex_dealer->calcCurrentTime(time(0))).c_str());
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "header=", measurment_params.header);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "data length=", measurment_params.data_length);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "direction=", measurment_params.direction);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "type=", measurment_params.type);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "current_time.year=", measurment_params.current_time.year);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "current_time.month=", measurment_params.current_time.month);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "current_time.day=", measurment_params.current_time.day);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "current_time.hour=", measurment_params.current_time.hour);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "current_time.minute=", measurment_params.current_time.minute);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "current_time.second=", measurment_params.current_time.second);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "current=", measurment_params.curr);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "voltage=", measurment_params.voltage);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "temp=", measurment_params.temp);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "humidity=", measurment_params.humidity);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "check_sum=", measurment_params.check_sum);
    strcat(data_measurment, current_line);
    sprintf(current_line, "%s %d\n", "footer=", measurment_params.footer);
    strcat(data_measurment, current_line);
    sprintf(current_line, "##############################################\n");
}
void MessageParser::print_heartbeat_msg (char data_heartbeat[])
{
    memset(data_heartbeat,0,sizeof(data_heartbeat));
    char current_line[1024] = "";
    sprintf(current_line, "heartbeat:\n");
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "Server Time: %s\n", (hex_dealer->calcCurrentTime(time(0))).c_str());
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "header=", heartbeat_params.header);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "data length=", heartbeat_params.data_length);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "direction=", heartbeat_params.direction);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "type=", heartbeat_params.type);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "current_time.year=", heartbeat_params.current_time.year);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "current_time.month=", heartbeat_params.current_time.month);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "current_time.day=", heartbeat_params.current_time.day);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "current_time.hour=", heartbeat_params.current_time.hour);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "current_time.minute=", heartbeat_params.current_time.minute);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "current_time.second=", heartbeat_params.current_time.second);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "content=", heartbeat_params.content);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "check_sum=", heartbeat_params.check_sum);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "%s %d\n", "footer=", heartbeat_params.footer);
    strcat(data_heartbeat, current_line);
    sprintf(current_line, "##############################################\n");
    strcat(data_heartbeat, current_line);
}
void MessageParser::print_alarm_msg(char data_alarm[])
{
    std::string alarm_type_string[8] = {"current low", "current high", "voltage low", "voltage high", "temp low", "temp high" ,"hum low", "hum high" };
    
     memset(data_alarm,0,sizeof(data_alarm));
    char current_line[1024] = "";
    sprintf(current_line, "ALarm Message:\n");
    strcat(data_alarm, current_line);
    sprintf(current_line, "Server Time: %s\n", (hex_dealer->calcCurrentTime(time(0))).c_str());
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "header=", alarm_params.header);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "data length=", alarm_params.data_length);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "direction=", alarm_params.direction);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "type=", alarm_params.type);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "current_time.year=", alarm_params.current_time.year);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "current_time.month=", alarm_params.current_time.month);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "current_time.day=", alarm_params.current_time.day);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "current_time.hour=", alarm_params.current_time.hour);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "current_time.minute=", alarm_params.current_time.minute);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "current_time.second=", alarm_params.current_time.second);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "numalarm=", alarm_params.numalarm);
    strcat(data_alarm, current_line);
    
    for(int i = 0; i < alarm_params.numalarm; i++)
    {
        sprintf(current_line, "%s %d %s %s\n","alarm type[",i ,alarm_type_string[alarm_params.Talarm[i] - 1].c_str());
        strcat(data_alarm, current_line);
    }
    sprintf(current_line, "%s %d\n", "current=", alarm_params.curr);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "voltage=", alarm_params.voltage);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "temp=", alarm_params.temp);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "humidity=", alarm_params.humidity);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "check_sum=", alarm_params.check_sum);
    strcat(data_alarm, current_line);
    sprintf(current_line, "%s %d\n", "footer=", alarm_params.footer);
    strcat(data_alarm, current_line);
    sprintf(current_line, "##############################################\n");
    strcat(data_alarm, current_line);
  
}
#endif