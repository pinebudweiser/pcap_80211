#include <iostream>
#include <stdint.h>
#include <map>
#include <pcap/pcap.h>
#include <string.h>
#include "802-wireless.h"
#include "display-mapping.h"

#define MAX_PACKET_SIZE 0xFFFF
#define PROMISCUOUS     1
#define TIME_OUT        0xFF

using namespace std;

uint64_t MacToIntegerKey(uint8_t* byte_mac);
void CopyBeaconDetail(BEACON_DETAIL* beacon_temp,
                      uint8_t* bssid_addr, char* essid_addr,
                      int8_t power, uint16_t frequency, bool crypt);
void CopyProbeDetail(PROBE_DETAIL* probe_temp,
                     uint8_t* bssid_addr, uint8_t* station_addr, char* probe_addr,
                     int8_t power);

int main(int argc, char** argv)
{
    /* Setting pcap */
    pcap_t* pkt_descriptor;
    pcap_pkthdr* pkt_header;
    const unsigned char* pkt_buffer;
    char* interface_name = argv[1];
    char err_buf[256];
    /* 802.11 frame */
    RADIOTAP_HDR* radio_header;
    IETRIPLE_HDR* ieee_header;
    IETRIPLE_BODY* ieee_body;
    TAG_SSID* tag_ssid;
    BEACON_DETAIL temp_beacon_item;
    PROBE_DETAIL temp_probe_item;
    /* Display class */
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
        tag_ssid = (TAG_SSID*)ieee_body;

        if(ieee_header->frame_type.type == MANAGEMENT_FRAME)
        {
            switch(ieee_header->frame_type.sub_type)
            {
                case PROBE_REQUEST:
                    CopyProbeDetail(&temp_probe_item, ieee_header->address3, ieee_header->address2, (char*)&(tag_ssid->tag_length)+1,
                                radio_header->antenna_signal1);
                    display.InsertProbeItem(MacToIntegerKey(ieee_header->address2), &temp_probe_item);
                    break;
                case BEACON:
                    CopyBeaconDetail(&temp_beacon_item, ieee_header->address3, (char*)&(ieee_body->tag_length)+1,
                                      radio_header->antenna_signal1, radio_header->channel_frequency, ieee_body->cpb_info.privacy);
                    display.InsertBeaconItem(MacToIntegerKey(ieee_header->address3), &temp_beacon_item);
                    break;
                default:
                    break;
            }
            display.ShowItem();
        }
    }
    return 0;
}

void CopyBeaconDetail(BEACON_DETAIL* beacon_temp, uint8_t* bssid_addr, char* essid_addr,
                      int8_t power, uint16_t frequency, bool crypt)
{
    memset(beacon_temp->essid, 0, sizeof(beacon_temp->essid)); // clear essid
    memcpy(beacon_temp->bssid, bssid_addr, 6); // copy bssid, because bssid will be converting integer
    beacon_temp->power = power;
    beacon_temp->channel = (uint8_t)((frequency - 2412)/5) + 1;    // Todo: Now setting middle frequency, Add Low and High
    beacon_temp->hit_beacon = 0; // beacon will be counting
    beacon_temp->crypt = crypt; // 0 - Open, 1 - Crypt
    strncpy(beacon_temp->essid, (char*)(essid_addr), (uint8_t)(*(essid_addr-1))); // essid_addr => length
}
void CopyProbeDetail(PROBE_DETAIL* probe_temp, uint8_t* bssid_addr, uint8_t* station_addr, char* probe_addr,
                     int8_t power)
{
    memset(probe_temp->probe, 0, sizeof(probe_temp->probe)); // clear probe
    memcpy(probe_temp->bssid, bssid_addr, 6);
    memcpy(probe_temp->station_id, station_addr, 6);
    probe_temp->power = power;
    probe_temp->frame = 0; // frame will be counting
    strncpy(probe_temp->probe, (char*)(probe_addr), (uint8_t)(*(probe_addr-1))); // probe_addr => length
}

uint64_t MacToIntegerKey(uint8_t* byte_mac)
{
    uint64_t result = 0;

    for (int i=0;i<=5;i++){
        result = result + (byte_mac[i] << 8*(5-i));
    }
    return result;
}
