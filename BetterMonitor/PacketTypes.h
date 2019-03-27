#ifndef PacketTypes_h
#define PacketTypes_h

enum class PacketType {
    Unknown,
    Mgmt,
    Ctrl,
    Data
};

// page 640 on the 802.11-2016 standard
enum class SubType {
    Unknown,

    // mgmt:
    AssocReq, AssocRes, ReassocReq, ReassocRes, ProbeReq, ProbeRes, TimingAdv, Beacon, ATIM, Disassoc, Auth, Deauth, Action, ActionNoAck,

    // ctrl:
    BeamRepPoll, VHTNDPAnnoucement, Wrapper, BlockAckReq, BlockAck, PSPoll, RTS, CTS, Ack, CFEnd, CFEndCFAck, Poll, SPR, Grant, DMGCTS, DMGDTS, GrantAck, SSW, SSWFeedback, SSWAck,

    // data:
    Data, DataCFAck, DataCFPoll, DataCFAckCFPoll, Null, CFAck, CFPoll, CFAckCFPoll, QoSData, QoSDataCFAck, QoSDataCFPoll, QoSDataCFAckCFPoll, QoSNull, QoSCFPoll, QoSCFAckCFPoll
};

enum class Protocol {
    Unknown,
    bg,
    n,
    ac
};

enum class Bandwidth {
	nonHT,
    HT20,
    HT40minus,
    HT40plus
};

#endif // ifdef PacketTypes_h