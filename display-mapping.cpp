#include "display-mapping.h"
#include<iostream>
#include<string.h>

DisplayMapping::DisplayMapping()
{

}

bool DisplayMapping::InsertBeaconItem(uint64_t integer_mac, BEACON_DETAIL* data)
{
    if(beacon_detail_.find(integer_mac) == beacon_detail_.end()) // if key find fail
    {
        BEACON_DETAIL* new_item = (BEACON_DETAIL*)malloc(sizeof(BEACON_DETAIL));

        memcpy(new_item, data, sizeof(BEACON_DETAIL));
        std::pair<uint64_t, BEACON_DETAIL*> p(integer_mac, new_item);
        beacon_detail_.insert(p);

        return true;
    }
    else
    {
        std::map<uint64_t, BEACON_DETAIL*>::iterator iter;

        iter = beacon_detail_.find(integer_mac); // get_key
        /* Update data */
        iter->second->hit_beacon += 1;
        iter->second->channel = data->channel;
        iter->second->power = data->power;
        iter->second->crypt = data->crypt;

        return false;
    }
}
bool DisplayMapping::InsertProbeItem(uint64_t integer_mac, PROBE_DETAIL* data)
{
    if(probe_detail_.find(integer_mac) == probe_detail_.end()) // if key find fail
    {
        PROBE_DETAIL* new_item = (PROBE_DETAIL*)malloc(sizeof(PROBE_DETAIL));

        memcpy(new_item, data, sizeof(PROBE_DETAIL));
        std::pair<uint64_t, PROBE_DETAIL*> p(integer_mac, new_item);
        probe_detail_.insert(p);

        return true;
    }
    else
    {
        std::map<uint64_t, PROBE_DETAIL*>::iterator iter;

        iter = probe_detail_.find(integer_mac); // get_key
        /* Update data */
        iter->second->power = data->power;
        iter->second->frame += 1;

        return false;
    }
}
void DisplayMapping::ShowItem()
{
    std::map<uint64_t, BEACON_DETAIL*>::iterator beacon_iter;
    std::map<uint64_t, PROBE_DETAIL*>::iterator probe_iter;

    printf("BSSID\t\t\tESSID\t\tCHAN\tBEACON\tPOWER\tCRYPT\n");
    for(beacon_iter = beacon_detail_.begin(); beacon_iter != beacon_detail_.end(); ++beacon_iter)
    {
        printf("%02X:%02X:%02X:%02X:%02X:%02X\t", beacon_iter->second->bssid[0], beacon_iter->second->bssid[1], beacon_iter->second->bssid[2], beacon_iter->second->bssid[3], beacon_iter->second->bssid[4], beacon_iter->second->bssid[5]);
        printf("%s\t%d\t%d\t%d\t%d\n", beacon_iter->second->essid, beacon_iter->second->channel, beacon_iter->second->hit_beacon, beacon_iter->second->power, beacon_iter->second->crypt);
    }
    printf("\n");
    printf("BSSID\t\t\tSTATION\t\t\tPWR\tFRAME\tPROBE\n");
    for(probe_iter = probe_detail_.begin(); probe_iter != probe_detail_.end(); ++probe_iter)
    {
        printf("%02X:%02X:%02X:%02X:%02X:%02X\t", probe_iter->second->bssid[0], probe_iter->second->bssid[1], probe_iter->second->bssid[2], probe_iter->second->bssid[3], probe_iter->second->bssid[4], probe_iter->second->bssid[5]);
        printf("%02X:%02X:%02X:%02X:%02X:%02X\t", probe_iter->second->station_id[0], probe_iter->second->station_id[1], probe_iter->second->station_id[2], probe_iter->second->station_id[3], probe_iter->second->station_id[4], probe_iter->second->station_id[5]);
        printf("%d\t%d\t%s\n", probe_iter->second->power, probe_iter->second->frame, probe_iter->second->probe);
    }
    printf("\n\n\n\n\n\n\n");
    printf("\033[2J");
}

void DisplayMapping::FreeItem()
{
    std::map<uint64_t, BEACON_DETAIL*>::iterator beacon_iter;
    std::map<uint64_t, PROBE_DETAIL*>::iterator probe_iter;

    for(beacon_iter = beacon_detail_.begin(); beacon_iter != beacon_detail_.end(); ++beacon_iter)
    {
        free(beacon_iter->second);
    }
    for(probe_iter = probe_detail_.begin(); probe_iter != probe_detail_.end(); ++probe_iter)
    {
        free(probe_iter->second);
    }
}
