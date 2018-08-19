#ifndef DISPLAY_MAPPING_H
#define DISPLAY_MAPPING_H

#include<stdint.h>
#include<map>

typedef struct BeaconDetail{
    uint8_t bssid[6];
    char essid[32];
    int8_t power;
    uint32_t hit_beacon;
    uint8_t channel;
    bool crypt;
}BEACON_DETAIL;

typedef struct ProbeDetail{
    uint8_t bssid[6];
    uint8_t station_id[6];
    int8_t power;
    uint32_t frame;
    char probe[32];
}PROBE_DETAIL;

class DisplayMapping
{
private:
    std::map<uint64_t, BEACON_DETAIL*> beacon_detail_;
    std::map<uint64_t, PROBE_DETAIL*> probe_detail_;
    std::pair<uint16_t, uint16_t> screen_size;
public:
    DisplayMapping();
    bool InsertBeaconItem(uint64_t integer_mac, BEACON_DETAIL* data);
    bool InsertProbeItem(uint64_t integer_mac, PROBE_DETAIL* data);
    void ShowItem();
    void FreeItem();
    void GetScreenSize(std::pair<uint16_t, uint16_t>* xy_pair);
};

#endif // DISPLAY_MAPPING_H
