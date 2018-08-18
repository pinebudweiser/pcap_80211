#include "display-mapping.h"
#include<iostream>
#include<string.h>

DisplayMapping::DisplayMapping()
{

}

bool DisplayMapping::InsertItem(uint64_t integer_mac, DISPLAY_ITEM* data)
{

    if(display_map_.find(integer_mac) == display_map_.end()) // if key find fail
    {
        DISPLAY_ITEM* new_item = (DISPLAY_ITEM*)malloc(sizeof(DISPLAY_ITEM));

        memcpy(new_item, data, sizeof(DISPLAY_ITEM));
        std::pair<uint64_t, DISPLAY_ITEM*> p(integer_mac, new_item);
        display_map_.insert(p);
    }
    else
    {
        display_map_.find(integer_mac)->second->hit_beacon += 1;
        display_map_.find(integer_mac)->second->channel = data->channel;
        display_map_.find(integer_mac)->second->power = data->power;
    }
    return true;
}
void DisplayMapping::ShowItem()
{
    std::map<uint64_t, DISPLAY_ITEM*>::iterator iter;

    printf("BSSID\t\t\tESSID\t\tCHAN\tBEACON\tPOWER\n");
    for(iter = display_map_.begin(); iter != display_map_.end(); ++iter)
    {
        printf("%02X:%02X:%02X:%02X:%02X:%02X\t", (*iter).second->bssid[0], (*iter).second->bssid[1], (*iter).second->bssid[2], (*iter).second->bssid[3], (*iter).second->bssid[4], (*iter).second->bssid[5]);
        printf("%s\t%d\t%d\t%d\n",(*iter).second->essid, (*iter).second->channel, (*iter).second->hit_beacon, (*iter).second->power);
    }
    printf("\033[1J");
}

void DisplayMapping::FreeItem()
{
    std::map<uint64_t, DISPLAY_ITEM*>::iterator iter;

    for(iter = display_map_.begin(); iter != display_map_.end(); ++iter)
    {
        free(iter->second);
    }
}
