#pragma once

#include <thread>
#include <ctime>
#include <chrono>


#include <sstream>

#include <stdint.h>
#include <iostream>
#include <list>
#include <vector>
#include <string>


#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>


#define SM_INIT_GET_INTERFACES			0
#define SM_WIFI_CONNEX_CONNECTED 1
#define SM_READ_ARP		2
#define SM_SCAN_ETH0	3
#define SM_SCAN_WIFI0	4




using namespace std;

class RSP_IP_DISCOVERY
{
public:
	RSP_IP_DISCOVERY();
	~RSP_IP_DISCOVERY();
	
	void set_timeout_s(uint16_t sec); //how long to do dicovering
	string get_status_string();		

	vector <string> get_IP_list();
	uint32_t		run_IP_discovery();

private:
	uint16_t state = SM_INIT_GET_INTERFACES;
	uint16_t state_prev= SM_INIT_GET_INTERFACES;
	uint16_t E;
	vector <string> ip_list;
	string found_WiFi_IP;

	void IP_discovery_proc();
	void read_ARP();
	//void read_ARP_linux();
	void read_ARP_win();
    void get_interfaces_win();
	void scan_and_ping_wifi(string wifi_name);
	uint16_t timeout_s = 5; //by defult 5 seconds for dicovery

	
	vector <uint32_t> RPI_MAC_TABLE_DEFAULT = 
	{
		0x28CDC1,\
		0xB827EB,\
		0xD83ADD,\
		0xDCA632,\
		0xE45F01
	};
};

