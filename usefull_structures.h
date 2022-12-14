#include <cstdlib>
#include <netdb.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include <chrono>
#include <string.h>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits>
#include <dlfcn.h>
#include <errno.h>

struct time_struct
{
   int year   = -1;
   int month  = -1;
   int day    = -1;
   int hour   = -1;
   int minute = -1;
   int second = -1;
};

struct Talarm_struct
{
    int curr_low=-1;
    int voltage_low=-1;
    int humidity_low=-1;
    int temp_low=-1;
    int curr_hight=-1;
    int voltage_hight=-1;
    int humidity_hight=-1;
    int temp_hight=-1;
    
};

struct login_data
{
    int header = -1;
    int data_length = -1;
    int direction = -1;
    int type = -1;
    time_struct current_time = {-1, -1, -1, -1, -1, -1};
    std::string imei = "";
    int check_sum = -1;
    int footer = -1;
};

struct heartbeat_data
{
    int header = -1;
    int data_length = -1;
    int direction = -1;
    int type = -1;
    time_struct current_time = {-1, -1, -1, -1, -1, -1};
    int content=-1;
    int check_sum = -1;
    int footer = -1;
};
struct measurment_data
{
    int header = -1;
    int data_length = -1;
    int direction = -1;
    int type = -1;
    time_struct current_time = {-1, -1, -1, -1, -1, -1};
    int curr=-1;
    int voltage=-1;
    int temp=-1;
    int humidity=-1;
    int check_sum = -1;
    int footer = -1;
};
struct alarm_data
{
    int header = -1;
    int data_length = -1;
    int direction = -1;
    int type = -1;
    time_struct current_time = {-1, -1, -1, -1, -1, -1};
    int numalarm =-1;
    int Talarm[1024] = { 0 };
    int curr=-1;
    int voltage=-1;
    int temp=-1;
    int humidity=-1;
    int check_sum = -1;
    int footer = -1;
};
struct control_curr_data
{

    int header = -1;
    int data_length = -1;
    int direction = -1;
    int type = -1;
    time_struct current_time = {-1, -1, -1, -1, -1, -1};
    int minvalcurr=-1;
    int maxvalcurr=-1;
    int check_sum = -1;
    int footer = -1;
};
struct  control_voltage_data 
{

    int header = -1;
    int data_length = -1;
    int direction = -1;
    int type = -1;
    time_struct current_time = {-1, -1, -1, -1, -1, -1};
    int minvalvoltage=-1;
    int maxvalvoltage=-1;
    int check_sum = -1;
    int footer = -1;
};
struct  control_temp_data 
{

    int header = -1;
    int data_length = -1;
    int direction = -1;
    int type = -1;
    time_struct current_time = {-1, -1, -1, -1, -1, -1};
    int minvaltemp=-1;
    int maxvaltemp=-1;
    int check_sum = -1;
    int footer = -1;
};
struct  control_humidity_data 
{

    int header = -1;
    int data_length = -1;
    int direction = -1;
    int type = -1;
    time_struct current_time = {-1, -1, -1, -1, -1, -1};
    int minvalhumidity=-1;
    int maxvalhumidity=-1;
    int check_sum = -1;
    int footer = -1;
};