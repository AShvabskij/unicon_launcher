#include <windows.h>

#include "RSP_IP_DISCOVERY.h"

#include <iphlpapi.h>

using namespace std;

//#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

RSP_IP_DISCOVERY::RSP_IP_DISCOVERY()
{

}
RSP_IP_DISCOVERY::~RSP_IP_DISCOVERY()
{

}

vector <string> RSP_IP_DISCOVERY::get_IP_list()
{
    
    return ip_list;
}



uint32_t RSP_IP_DISCOVERY::run_IP_discovery()
{

	//start threads
    
	thread thread_app(&RSP_IP_DISCOVERY::IP_discovery_proc,this);
	thread_app.join();

    return 0;
}

void RSP_IP_DISCOVERY::IP_discovery_proc()
{

    bool done = false;
    while (!done) {
        if (state != state_prev) E = 1; else E = 0;
        state_prev = state;
        
        switch (state) {
        case SM_INIT:
            state = SM_READ_ARP;
            break;

        case SM_READ_ARP:
            read_ARP();
            done = true;
            break;


        case SM_SCAN_ETH0:
            
            break;
        case SM_SCAN_WIFI0: //
                    
            break;

        default: break;
        }

        this_thread::sleep_for(chrono::milliseconds(1000));//

    }

    cout << "IP_discovery_proc done!" << endl;
}

void RSP_IP_DISCOVERY::set_timeout_s(uint16_t sec) //how long to do dicovering
{
    timeout_s = sec;

}


string RSP_IP_DISCOVERY::get_status_string() 
{
    string s ="";
    return s;
}


void RSP_IP_DISCOVERY::read_ARP()
{
    get_interfaces_win();
    read_ARP_win();
//
//#if defined(_WIN32)
//
//#else
//    read_ARP_linux();
//#endif

}



void RSP_IP_DISCOVERY::read_ARP_win() {




    
        // Allocate memory for the ARP table
        ULONG size = 0;
        GetIpNetTable(nullptr, &size, false);
        vector<std::byte> buffer(size);

        // Retrieve the ARP table
        PMIB_IPNETTABLE arpTable = reinterpret_cast<PMIB_IPNETTABLE>(buffer.data());
        if (GetIpNetTable(arpTable, &size, false) == NO_ERROR) {
            cout << "ARP Table:" << endl;
            cout << "=========================================" << endl;
            cout << "IP Address       MAC Address       Type" << endl;
            cout << "=========================================" << endl;

            for (DWORD i = 0; i < arpTable->dwNumEntries; ++i) {
                MIB_IPNETROW& row = arpTable->table[i];
                
                // Print IP address
                in_addr ipAddr;
                ipAddr.S_un.S_addr = row.dwAddr;
                string s_ipAddr = inet_ntoa(ipAddr);
                cout << s_ipAddr << "  dwIndex "<< row.dwIndex<<"  ";

                // Print MAC address
                if (row.dwPhysAddrLen == 6) {
                    for (DWORD j = 0; j < row.dwPhysAddrLen; ++j) {
                        if (j > 0) cout << "-";
                        printf("%02X", row.bPhysAddr[j]);        
                    }

                    uint32_t mac_addr_top3_byte = ( (row.bPhysAddr[0] << 16) + \
                                                    (row.bPhysAddr[1] << 8) + \
                                                    (row.bPhysAddr[2] << 0) );
                                        //(row.bPhysAddr[3] << 16) + \
                                        //(row.bPhysAddr[4] << 8) + \
                                        //(row.bPhysAddr[5] << 0));

                    //check if RPI  mac 
                    for (int ii = 0; ii < RPI_MAC_TABLE_DEFAULT.size(); ii++)
                        if (mac_addr_top3_byte == RPI_MAC_TABLE_DEFAULT[ii])
                            ip_list.push_back(s_ipAddr);

                }
                else {
                    cout << "N/A";
                }
                cout << "       ";

                // Print type
                switch (row.dwType) {
                case MIB_IPNET_TYPE_OTHER:
                    cout << "Other";
                    break;
                case MIB_IPNET_TYPE_INVALID:
                    cout << "Invalid";
                    break;
                case MIB_IPNET_TYPE_DYNAMIC:
                    cout << "Dynamic";
                    break;
                case MIB_IPNET_TYPE_STATIC:
                    cout << "Static";
                    break;
                default:
                    cout << "Unknown";
                    break;
                }
                cout << endl;
            }
        }
        else {
            cerr << "Failed to retrieve ARP table." << endl;
        }


        if (ip_list.empty()) {
            cout << "NO RSP FOUND" << endl;
        }
        else
        {
            cout << "RSP FOUND AT IPs..." << endl;
            for (string& s : ip_list)
                cout << s << endl;
        }
        //bool res;                         TODO verify that connection exist and this is not old data from ARP table
        //string ping_cmd = "ping -n 2" + ;
        //res = system("ping -n 2 10.9.0.4");
        //if (res)  cout << "PING OK" << endl; 
        //else cout << "PING FAILED" << endl;
}

void RSP_IP_DISCOVERY::get_interfaces_win()
{
    ULONG outBufLen = 0;
    DWORD dwRetVal = 0;
    IP_ADAPTER_INFO* pAdapterInfos = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));

    // retry up to 5 times, to get the adapter infos needed
    for (int i = 0; i < 5 && (dwRetVal == ERROR_BUFFER_OVERFLOW || dwRetVal == NO_ERROR); ++i)
    {
        dwRetVal = GetAdaptersInfo(pAdapterInfos, &outBufLen);
        if (dwRetVal == NO_ERROR)
        {
            break;
        }
        else if (dwRetVal == ERROR_BUFFER_OVERFLOW)
        {
            free(pAdapterInfos);
            pAdapterInfos = (IP_ADAPTER_INFO*)malloc(outBufLen);
        }
        else
        {
            pAdapterInfos = 0;
            break;
        }
    }
    if (dwRetVal == NO_ERROR)
    {
        IP_ADAPTER_INFO* pAdapterInfo = pAdapterInfos;
        while (pAdapterInfo)
        {
            IP_ADDR_STRING* pIpAddress = &(pAdapterInfo->IpAddressList);
            while (pIpAddress != 0)
            {
                // 
                // <<<<
                // here pAdapterInfo->Address should contain the MAC address
                // >>>>
                // 
                cout << "Description " << pAdapterInfo->Description ;
                cout << " Index " << hex<< pAdapterInfo->Index;

                cout<< " MAC Address " <<   hex << static_cast<int>(pAdapterInfo->Address[0]) <<\
                                            hex << static_cast<int>(pAdapterInfo->Address[1]) <<\
                                            hex << static_cast<int>(pAdapterInfo->Address[2]) <<\
                                            hex << static_cast<int>(pAdapterInfo->Address[3]) <<\
                                            hex << static_cast<int>(pAdapterInfo->Address[4]) <<\
                                            hex << static_cast<int>(pAdapterInfo->Address[5]) <<endl;
                pIpAddress = pIpAddress->Next;
            }
            pAdapterInfo = pAdapterInfo->Next;
        }
    }
    free(pAdapterInfos);
   
}


/*
void RSP_IP_DISCOVERY::read_ARP_linux () {

    char line[500]; // Read with fgets().
    char ip_address[500]; // Obviously more space than necessary, just illustrating here.
    int hw_type;
    int flags;
    char mac_address[500];
    char mask[500];
    char device[500];

    FILE* fp = fopen("/proc/net/arp", "r");
    fgets(line, sizeof(line), fp);              // Skip the first line (column headers).
    while (fgets(line, sizeof(line), fp))
    {
        // Read the data.
        sscanf(line, "%s 0x%x 0x%x %s %s %s\n",
            ip_address,
            &hw_type,
            &flags,
            mac_address,
            mask,
            device);

        // Do stuff with it.
    }

    fclose(fp);
}
*/
