#include <stdint.h>

#define MANAGEMENT_FRAME    0
#define CONTROL_FRAME       1
#define DATA_FRAME          2

enum SubType {
    ASSOC_REQUEST = 0,
    ASSOC_RESPONSE,
    REASSOC_REQUEST,
    REASSOC_RESPONSE,
    PROBE_REQUEST,
    PROBE_RESPONSE,
    BEACON=8,
};

typedef struct RadiotapHeader{
    uint8_t revision;
    uint8_t pad;
    uint16_t length;
    uint32_t present_flag_low;
    uint32_t present_flag_high;
    uint8_t flag;
    uint8_t data_rate; // 0x2 => 1Mb
    uint16_t channel_frequency; // (LOWER)2401, (CENTER)2412, (UPPER)2423 = Channel 1, +5(Next Channel)
    uint16_t channel_flags;
    int8_t antenna_signal1;
    uint8_t unuse;
    uint16_t rx_flag;
    int8_t antenna_signal2;
    int8_t antenna_signal3;
}RADIOTAP_HDR;

typedef struct IETripleHeader{
    struct FrameType{
        uint8_t version : 2;
        uint8_t type : 2;
        uint8_t sub_type : 4;
    }frame_type; // {0,0,8} = beacon, {0,2,0} = data
    uint8_t flags;
    uint16_t duration;
    uint8_t dest_mac[6];
    uint8_t src_mac[6];
    uint8_t bss_id[6];
    struct FraSeqNumber{
        uint16_t fragment_number_ : 4;
        uint16_t sequence_number_ : 12;
    }fra_seq_number;
}IETRIPLE_HDR;

typedef struct IETripleBody{
    uint64_t time_stamp;
    uint16_t beacon_interval;
    uint16_t cpb_info;
    uint8_t tag_number;
    uint8_t tag_length;
}IETRIPLE_BODY;
