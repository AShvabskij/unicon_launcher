#pragma once

#include <thread>
#include <ctime>
#include <chrono>

#include <stdint.h>
#include <iostream>
#include <list>
#include <vector>
#include <string>


#include <stdio.h>
#include <stdlib.h>


#define SM_INIT			0
#define SM_READ_ARP		1
#define SM_SCAN_ETH0	2
#define SM_SCAN_WIFI0	3






class RSP_IP_DISCOVERY
{
public:
	RSP_IP_DISCOVERY();
	~RSP_IP_DISCOVERY();
	
	void set_timeout_s(uint16_t sec); //how long to do dicovering
    std::string get_status_string();

    std::vector <std::string> get_IP_list();
	uint32_t		run_IP_discovery();

private:
	uint16_t state = SM_INIT;
	uint16_t state_prev= SM_INIT;
	uint16_t E;
    std::vector <std::string> ip_list;
	void IP_discovery_proc();
	void read_ARP();
	//void read_ARP_linux();
	void read_ARP_win();
	void get_interfaces_win();
	uint16_t timeout_s = 5; //by defult 5 seconds for dicovery


    std::vector <uint32_t> RPI_MAC_TABLE_DEFAULT =
	{
		0x28CDC1,\
		0xB827EB,\
		0xD83ADD,\
		0xDCA632,\
		0xE45F01
	};
};

