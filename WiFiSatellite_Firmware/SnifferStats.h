#ifndef SnifferStats_h
#define SnifferStats_h

typedef struct {
    uint64_t all_pkts;
    uint64_t misc_pkts;
    uint64_t dropped_pkts;
    uint64_t bytes;
	uint64_t   rssi;
} SnifferStats;

#endif /* ifndef SnifferStats_h */