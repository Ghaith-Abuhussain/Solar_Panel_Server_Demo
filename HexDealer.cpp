/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HexDealer.cpp
 * Author: mohammad
 * 
 * Created on April 9, 2022, 11:59 PM
 */

#include "HexDealer.h"

HexDealer::HexDealer() {
}

HexDealer::~HexDealer() {
}

std::string HexDealer::calcCurrentTime(time_t now)
{
	tm* my_time = new tm();
	my_time = localtime(&now);
	std::string date = "now = ";
	date += std::to_string(my_time->tm_year + 1900);
	date += "-";
	date += std::to_string(my_time->tm_mon + 1);
	date += "-";
	date += std::to_string(my_time->tm_mday);
	date += " ";
	date += std::to_string(my_time->tm_hour);
	date += ":";
	date += std::to_string(my_time->tm_min);
	date += ":";
	date += std::to_string(my_time->tm_sec);
	date += " ";
	if (my_time->tm_hour > 12)
		date += "PM";
	else
		date += "AM";

	//if (debug)
	//	std::cout << date << std::endl;


	std::stringstream hex_date;
	hex_date <<std::setfill('0') << std::setw(sizeof(short)) << std::hex << (my_time->tm_year - 100)
		<< std::setfill('0') << std::setw(sizeof(short)) << std::hex << (my_time->tm_mon + 1)
		<< std::setfill('0') << std::setw(sizeof(short)) << std::hex << my_time->tm_mday
		<< std::setfill('0') << std::setw(sizeof(short)) << std::hex << (my_time->tm_hour)
		<< std::setfill('0') << std::setw(sizeof(short)) << std::hex << my_time->tm_min
		<< std::setfill('0') << std::setw(sizeof(short)) << std::hex << my_time->tm_sec;

	std::string v_temp_string = hex_date.str();
	std::string v_ = "123456789ABC";;
	for (int i = 0; i < v_temp_string.length(); i++)
	{
		v_[i] = toupper(v_temp_string[i]);
	}
	return v_;
}

uint16_t HexDealer::GetCrc16(const uint8_t pData[], int nLength)
{
	int i = 0;
	uint16_t fcs = 0xffff; // initialization
	while (nLength > 0) {
		fcs = (fcs >> 8) ^ crctab16[(fcs ^ pData[i]) & 0xff];
		nLength--;
		i++;
	}
	return ~fcs; // negated
}

char* HexDealer::StringToByteArray(std::string hex, int length)
{
	std::string temp = "";
	char val[10000] = { 0 };
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

char* HexDealer::StringToByteArrayToSend(std::string hex, int length, char dataBuff[])
{
	std::string temp = "";
	char val[10000] = { 0 };
	int x[10000] = { 0 };
	std::stringstream ss;
	for (int i = 0; hex[i] != '\0'; i += 2)
	{
		temp += hex[i];
		temp += hex[i + 1];
		ss << std::hex << temp;
		ss >> x[i / 2];
		val[i / 2] = x[i / 2];
		dataBuff[i / 2] = x[i / 2];
		temp = "";
		ss.clear();
	}
	return val;
}

std::string HexDealer::to_String_X2(unsigned int v)
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

std::string HexDealer::to_String_X4(unsigned int v)
{
	v &= 0xffff;
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(4)
		<< std::hex << v;
	std::string v_temp_string = stream.str();
	std::string v_ = "    ";
	for (int i = 0; v_temp_string[i] != '\0'; i++)
	{
		v_[i] = toupper(v_temp_string[i]);
	}
	return v_;
}

std::string HexDealer::to_String_X8(unsigned int v)
{
	v &= 0xffffffff;
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(8)
		<< std::hex << v;
	std::string v_temp_string = stream.str();
	std::string v_ = "        ";
	for (int i = 0; v_temp_string[i] != '\0'; i++)
	{
		v_[i] = toupper(v_temp_string[i]);
	}
	return v_;
}

std::string HexDealer::to_String_X16(unsigned long long v)
{
	v &= 0xffffffffffffffff;
	std::string v1 = to_String_X8(v % 0xffffffff);
	std::string v2 = to_String_X8(v / 0xffffffff);
	return v2 + v1;
}

