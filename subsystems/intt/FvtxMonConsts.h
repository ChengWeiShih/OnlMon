#ifndef __FVTXMONCONSTS_H__
#define __FVTXMONCONSTS_H__

const int NARMS = 2;
const int NSTATIONS = 4;
const int NROCS = 24; // 12 in each arm
const int NPACKETSARM[NARMS] = { 24, 24 };
const int FIRSTPACKET[NARMS] = { 25001, 25101 }; 
const int NTOTSTRIPS[NARMS] = { 540672, 540672 }; 
//const int NFEMS = 4; // for each packet
//const int NCHIPS = 52;  // for each fem
//const int NCHANNELS = 128; // for each chip
//const int NCHANNELSONPACKET = 22528;    // for each packet 128*((10+26)+(10+26)+(26+26)+(26+26))=22528

//const int NCAGES = 2;
//const int NWEDGES = 24;
//const int NCOLUMNS = 2;
//const int NSTRIPS[NSTATIONS] = {640, 1664, 1664, 1664}; 

#endif // __FVTXMONCONSTS_H__
