#include "RSP_IP_DISCOVERY.h"
#include <QDebug>

#ifdef QT_VERSION
    #include <QProcess>
#endif






#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")


void pingIP(const std::string& ip);
std::vector<int> getIPFields(const std::string& ip);

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
        case SM_INIT_GET_INTERFACES:
            cout << "call get_interfaces_win " << endl;
            get_interfaces_win();
            state = SM_WIFI_CONNEX_CONNECTED;
            break;
        case SM_WIFI_CONNEX_CONNECTED:
            //scan_wifi("CONNEX");
            cout << "call scan_and_ping_wifi" << endl;
            scan_and_ping_wifi("CONNEX");
            state = SM_READ_ARP;
            break;
        case SM_READ_ARP:
            cout << "call read_ARP()" << endl;
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
        vector<BYTE> buffer(size);

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
                struct in_addr ipAddr;
                ipAddr.S_un.S_addr = row.dwAddr;
                //string s_ipAddr = inet_ntoa(ipAddr);
                char ipv4Str[INET_ADDRSTRLEN];
                string s_ipAddr = inet_ntop(AF_INET, &ipAddr, ipv4Str,INET_ADDRSTRLEN);

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

    IP_ADAPTER_INFO* pAdapterInfos = nullptr;

    found_WiFi_IP.clear();

    // Get the required buffer size by passing nullptr
    if (GetAdaptersInfo(nullptr, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
        pAdapterInfos = (PIP_ADAPTER_INFO)malloc(outBufLen);
        if (pAdapterInfos == nullptr) {
            std::cerr << "Failed to allocate memory for adapter information." << std::endl;
            return;
        }
    } else {
        std::cerr << "Failed to calc size for adapter information." << std::endl;
        return;
    }

    // retry up to 5 times, to get the adapter infos needed
    for (int i = 0; i < 5; ++i)
    {
        cout << "call GetAdaptersInfo for i = " << i << std::endl;

        dwRetVal = GetAdaptersInfo(pAdapterInfos, &outBufLen);

        if (dwRetVal == NO_ERROR)
        {
            cout << "GetAdaptersInfo: no error" << std::endl;
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
                // print only if WIFI
                if (pAdapterInfo->Type == IF_TYPE_IEEE80211) // IF_TYPE_ETHERNET_CSMACD) || (pAdapterInfo->Type == IF_TYPE_ETHERNET_CSMACD)) 
                {
                    string IP_addr = pAdapterInfo->IpAddressList.IpAddress.String;
                    if (IP_addr != "0.0.0.0") {

                        cout << "Description " << pAdapterInfo->Description;
                        cout << " Index " << hex << pAdapterInfo->Index;
                        cout << " Type " << hex<<pAdapterInfo->Type;// IF_TYPE_IEEE80211 ot IF_TYPE_ETHERNET_CSMACD
                        cout << " IP addr " << IP_addr;// pAdapterInfo->IpAddressList.IpAddress.String;
                        cout << " MAC Address " << hex << static_cast<int>(pAdapterInfo->Address[0]) << \
                            hex << static_cast<int>(pAdapterInfo->Address[1]) << \
                            hex << static_cast<int>(pAdapterInfo->Address[2]) << \
                            hex << static_cast<int>(pAdapterInfo->Address[3]) << \
                            hex << static_cast<int>(pAdapterInfo->Address[4]) << \
                            hex << static_cast<int>(pAdapterInfo->Address[5]) << endl;

                        found_WiFi_IP= IP_addr;
                    }
                }

                pIpAddress = pIpAddress->Next;
            }

            pAdapterInfo = pAdapterInfo->Next;
        }
    } else {
        cout << "GetAdaptersInfo: nothing found o error, dwRetVal = " << dwRetVal << endl;
    }

    // Clean up
    if (pAdapterInfos) {
        free(pAdapterInfos);
    }
}

void RSP_IP_DISCOVERY::scan_and_ping_wifi(string wifi_name)
{
    /*
        call netsh wlan show int
        There is 1 interface on the system :

    Name: Wi - Fi
    Description : Intel(R) Wi - Fi 6 AX200 160MHz
    GUID : 9e003efe - 6e83 - 4ca4 - b2eb - fddceece3e70
    Physical address : 08 : d2 : 3e : 28 : 69 : e6
    State : connected
    SSID : CONNEX
    BSSID : c6:17 : 5a : 98 : d9 : df
    Network type : Infrastructure
    Radio type : 802.11n
    Authentication : WPA2 - Personal
    Cipher : CCMP
    Connection mode : Profile
    Channel : 1
    Receive rate(Mbps) : 144.4
    Transmit rate(Mbps) : 144.4
    Signal : 96 %
    Profile : CONNEX

    Hosted network status : Not available */

    STARTUPINFO info = { sizeof(info) };
    //LPWSTR cmd = ("netsh wlan show int");
    //wchar_t cmd[] = L"netsh wlan show int";
    
    if (found_WiFi_IP != "0.0.0.0") {
        
        cout << "call getIPFields for WiFi_IP" << found_WiFi_IP << endl;
        vector<int> ip_addr = getIPFields(found_WiFi_IP);
        if (ip_addr.empty() || ip_addr.size() != 4) {
            return;
        }
        
        std::vector<std::thread> threads;
        std::string baseIP = to_string(ip_addr[0])+"."+\
                             to_string(ip_addr[1])+"."+ \
                             to_string(ip_addr[2])+"." ; //""192.168.1.";

        std::cout << "Ping addresses from 192.168.X.1 to 192.168.X.255 in parallel" << "\n";

        for (int i = 1; i < 255; i++) {
            std::string ip = baseIP + std::to_string(i);
            threads.emplace_back(std::thread(pingIP, ip));  // Start a new thread for each ping
//          std::cout << "ping ip " << ip << "\n";
        }

        // Join all threads to ensure they finish before exiting
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }
     
}


string s;
// Function to ping a single IP address
void pingIP(const std::string& ip) 
{

#if defined(_WIN32)

    #ifdef QT_VERSION
            QProcess proc;
            QStringList args = { "-n", "1", ip.c_str() };
            std::cout << "ping ip = " <<  ip.c_str() << "\n";
            proc.start("ping", args);

            if (!proc.waitForFinished(5000)) {
                std::cout << "Ping process failed!";
                return;
            }
    #else
        std::string command = "ping -n 1 " + ip + " | FIND \"TTL\" ";  // Windows command; for Linux use `ping -c 1`
        int result = system(command.c_str());      // Execute the ping command
    #endif

#endif

    // if (!proc.waitForFinished()) {
    //     qDebug() << "Ping process failed!";
    //     return;
    // }

    // if (result == 0) {
    //     //std::cout << ip << " is reachable.\n";
    // }
    // else {
    //     //std::cout << ip << " is not reachable.\n";
    // }
}


// Function to split the IP string and return its octets as integers
std::vector<int> getIPFields(const std::string& ip) {
    std::vector<int> octets;
    std::stringstream ss(ip);
    std::string segment;
    
    // Split the IP address string by '.'
    while (std::getline(ss, segment, '.')) {
        try {
            int octet = std::stoi(segment);  // Convert string segment to integer
            if (octet < 0 || octet > 255) {
                throw std::out_of_range("Octet value out of range (0-255)");
            }
            octets.push_back(octet);
        } catch (const std::exception& e) {
            std::cerr << "Invalid IP address segment: " << e.what() << "\n";
            return {};
        }
    }

    // Ensure there are exactly 4 octets for a valid IPv4 address
    if (octets.size() != 4) {
        std::cerr << "Invalid IP address format\n";
        return {};
    }

    // std::cout << "result octets = ";
    // for (int oct: octets) {
    //     std::cout << oct << ".";
    // }
    // std::cout << endl;

    return octets;
}


//void  ExecAndCaptureOutuput(string cmd, OutputCapture, ErrorCapture)
//{
//
//    //char char_buff_from_pipe[128]; //   buff for pipe receive end
//    //int _res;
//    //int result = 0;
//    //GCB_CAN_MSG gcb_can_msg;
//
//    //std::fgets(char_buff_from_pipe, 128, f_pipe);
//    //std::cout << char_buff_from_pipe; //print to terminal received data
//
//    //f_pipe = popen(cmd.c_str(), "r");
//    //if (f_pipe)
//    //{
//    //    pclose(f_pipe);
//    //    f_pipe = NULL;
//    //}
//  
//
//
//}

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
