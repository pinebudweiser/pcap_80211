#ifndef DISPLAY_MAPPING_H
#define DISPLAY_MAPPING_H

#include<stdint.h>
#include<map>

typedef struct DisplayItem{
    int8_t power;
    uint32_t hit_beacon;
    uint8_t channel;
    char essid[32];
    uint8_t bssid[6];
}DISPLAY_ITEM;

class DisplayMapping
{
private:
    std::map<uint64_t, DISPLAY_ITEM*> display_map_; // mac to uint64, key matching display item
public:
    DisplayMapping();
    bool InsertItem(uint64_t integer_mac, DISPLAY_ITEM* data);
    void ShowItem();
    void FreeItem();
};

#endif // DISPLAY_MAPPING_H
