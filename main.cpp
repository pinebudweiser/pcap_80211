#include <iostream>
#include <stdint.h>
#include <map>
#include <vector>
#include <pcap/pcap.h>
#include <string.h>
#include "802-wireless.h"
#include "display-mapping.h"

#define MAX_PACKET_SIZE 0xFFFF
#define PROMISCUOUS 1
#define TIME_OUT 0xFF

using namespace std;

uint64_t MacToIntegerKey(uint8_t* byte_mac);
void CopyDisplayStruct(DISPLAY_ITEM* temp, uint8_t* bssid_addr, char* essid_addr,
                       int8_t power, uint16_t frequency);

int main(int argc, char** argv)
{
    pcap_t* pkt_descriptor;
    pcap_pkthdr* pkt_header;
    const unsigned char* pkt_buffer;
    char* interface_name = argv[1];
    char err_buf[256];
    RADIOTAP_HDR* radio_header;
    IETRIPLE_HDR* ieee_header;
    IETRIPLE_BODY* ieee_body;
    DISPLAY_ITEM temp_display_item;
    DisplayMapping display;

    if(argc != 2)
    {
        cout << "[Notice] Usage - program <interface_name>" << endl;
        return false;
    }

    pkt_descriptor = pcap_open_live(interface_name, MAX_PACKET_SIZE, PROMISCUOUS, TIME_OUT, err_buf);
    if(!pkt_descriptor)
    {
        cout << "[Err] pcap_open_live() failed.. Reason : " << err_buf << endl;
        return false;
    }

    while(true)
    {
        if(!pcap_next_ex(pkt_descriptor, &pkt_header, &pkt_buffer))
        {
            // packet capture fail
            continue;
        }
        radio_header = (RADIOTAP_HDR*)(pkt_buffer);
        ieee_header = (IETRIPLE_HDR*)(pkt_buffer + radio_header->length);
        ieee_body = (IETRIPLE_BODY*)(pkt_buffer + radio_header->length + sizeof(IETRIPLE_HDR));

        if(ieee_header->frame_type.type == MANAGEMENT_FRAME)
        {
            switch(ieee_header->frame_type.sub_type)
            {
                case BEACON:
                    CopyDisplayStruct(&temp_display_item, ieee_header->bss_id, (char*)&(ieee_body->tag_length)+1,
                                      radio_header->antenna_signal1,
                                      radio_header->channel_frequency);
                    display.InsertItem(MacToIntegerKey(ieee_header->bss_id), &temp_display_item);
                    display.ShowItem();
                    break;
                default:
                    continue;
            }
        }
    }
    return 0;
}

void CopyDisplayStruct(DISPLAY_ITEM* temp, uint8_t* bssid_addr, char* essid_addr,
                       int8_t power, uint16_t frequency)
{
    memcpy(temp->bssid, bssid_addr, 6);
    temp->power = power;
    temp->channel = (uint8_t)((frequency - 2412)/5) + 1;    // Todo: Now Middle frequency, add Low, High using enum
    temp->hit_beacon = 0;
    strncpy(temp->essid, (char*)(essid_addr), (uint8_t)(*(essid_addr-1))); // essid_addr = length
}

uint64_t MacToIntegerKey(uint8_t* byte_mac)
{
    uint64_t result = 0;

    for (int i=0;i<=5;i++){
        result = result + (byte_mac[i] << 8*(5-i));
    }
    return result;
}
