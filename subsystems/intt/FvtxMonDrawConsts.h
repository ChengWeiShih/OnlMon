#ifndef __FVTXMONDRAWCONSTS_H__
#define __FVTXMONDRAWCONSTS_H__

const int LW = 1; // line width
const int BCOL = 4; // border color
const int WCOL = 2; // warning color
const float TXTSIZE = 0.045;
const float TXTSCALE = 1.0; // how far above the max should the text be?
const float BSCALE = 1.0; // how far above the max should the borders go?
const float MSIZE = 0.5; // marker size

// constants for how to position error messages
const float ERRORSPACE = 0.1;
const float ERROROFF = 0.1;

// canvas appearance
const int NPADSPERARM = 4;
const float DISTBORD = 0.010; // distance to border

// we have a (0-1)x(0-1) space to place out our pads on
// Leave bottom third of each canvas empty for messages
const float PADSIZEYBOTTOM = 0.333; // Leave bottom third of canvas empty
const float PADSIZEX1 = 1.0; // For 1 pad across
const float PADSIZEY1 = 0.666; // For 1 pad vertically 
const float PADSIZEX2 = 0.5; // For 2 pads across
const float PADSIZEY2 = 0.333; // For 2 pads vertically 
const float PADSIZEY3 = 0.222; // For 3 pads vertically 

// we need at least these many events
const int FVTXMINEVENTS = 1000;

//=====================
//! fvtxDraw("fvtxYieldsByPacket")
//=====================

//=====================
//! Max hits per events per packet 
// Comment out and update for other species or energies
//const int MAXHITSPERPACKET = 100; // Run-12 Cu+Au 200 GeV
//const int MAXHITSPERPACKET = 7; // Run-13 p+p 510 GeV, JMD 11 Mar 2013
//const int MAXHITSPERPACKET = 20; // Run-14 Au+Au  15 GeV, JMD 10 Mar 2014
//const int MAXHITSPERPACKET = 220; // Run-14 Au+Au 200 GeV
//const int MAXHITSPERPACKET = 20; // Run-14 He+Au 200 GeV
//const int MAXHITSPERPACKET = 6; // Run-15 p+p 200 GeV
//const int MAXHITSPERPACKET[2] = {12,10}; // Run-15 p+Au 200 GeV
//const int MAXHITSPERPACKET[2] = {8,8}; // Run-15 p+Al 200 GeV
const int MAXHITSPERPACKET[2] = {250,250}; // Run-16 Au+Au 200 GeV, yuhw
//const int MAXHITSPERPACKET[2] = {40,25}; // Run-16 d+Au 200 GeV, yuhw
//const int MAXHITSPERPACKET[2] = {20,10}; // Run-16 d+Au 62 GeV
//const int MAXHITSPERPACKET[2] = {50,50};//{10,6}; // Run-16 d+Au 20 GeV
//=====================

//=====================
//! Hot/Cold limits for hits per events per packet
//---------------------
//! for Run-15 p+p 200 GeV
//const float CUT_HOT_PACKET = 3.1;
//const float CUT_COLD_PACKET = 1.7;
//---------------------
//! for Run-15 p+Au 200 GeV
//const float CUT_HOT_PACKET[2] = {7.5,5.5};
//const float CUT_COLD_PACKET[2] = {3.5,2.5};
//---------------------
//! for Run-15 p+Al 200 GeV
//const float CUT_HOT_PACKET[2] = {5.5,4.5};
//const float CUT_COLD_PACKET[2] = {2.5,2.5};
//---------------------
//! for Run-16 Au+Au 200 GeV yuhw 03 Feb 2016
const float CUT_HOT_PACKET[2] = {160,160};
const float CUT_COLD_PACKET[2] = {30,30};//last one week //{50,50}; //first AuAu run
//---------------------
//! for Run-16 d+Au 200 GeV
//const float CUT_HOT_PACKET[2] = {30,14};
//const float CUT_COLD_PACKET[2] = {6,4};
//---------------------
//! for Run-16 d+Au 62 GeV
//const float CUT_HOT_PACKET[2] = {15, 8};
//const float CUT_COLD_PACKET[2] = {3, 2};
//---------------------
//! for Run-16 d+Au 20 GeV
//const float CUT_HOT_PACKET[2] = {8,5};
//const float CUT_COLD_PACKET[2] = {1, 0.5};
//=====================

//=====================
//! fvtxDraw("fvtxYields")
//! Yields per event per wedge
//---------------------
//Run-15 p+Au 200 GeV yuhw
//const float MAXHITSPERWEDGE[2] = {1.4,1.0};
//const float CUT_HOT_WEDGE_ST0[2] 	= {0.70,0.50};
//const float CUT_HOT_WEDGE_ST1[2] 	= {1.20,1.00};
//const float CUT_COLD_WEDGE_ST0[2] = {0.05,0.05};
//const float CUT_COLD_WEDGE_ST1[2] = {0.10,0.01};
//---------------------
//Run-15 p+Al 200 GeV yuhw
//const float MAXHITSPERWEDGE[2] = {1.1,1.1};
//const float CUT_HOT_WEDGE_ST0[2] 	= {0.60,0.60};
//const float CUT_HOT_WEDGE_ST1[2] 	= {1.00,1.00};
//const float CUT_COLD_WEDGE_ST0[2] = {0.05,0.05};
//const float CUT_COLD_WEDGE_ST1[2] = {0.10,0.10};
//---------------------
//Run-16 Au+Au 200 GeV yuhw 03 Feb 2016
const float MAXHITSPERWEDGE[2] = {40,40};
const float CUT_COLD_WEDGE_ST0[2] = {1,1};
const float CUT_HOT_WEDGE_ST0[2] 	= {20,20};
const float CUT_COLD_WEDGE_ST1[2] = {3,3};
const float CUT_HOT_WEDGE_ST1[2] 	= {30,30};
//---------------------
//Run-16 d+Au 200 GeV
//const float MAXHITSPERWEDGE[2] = {4,4};
//const float CUT_COLD_WEDGE_ST0[2] = {0.1,0.1};
//const float CUT_HOT_WEDGE_ST0[2] 	= {2,2};
//const float CUT_COLD_WEDGE_ST1[2] = {0.2,0.2};
//const float CUT_HOT_WEDGE_ST1[2] 	= {3.4,2.6};
//---------------------
//Run-16 d+Au 62 GeV
//const float MAXHITSPERWEDGE[2] = {3.5,3.5};
//const float CUT_COLD_WEDGE_ST0[2] = {0.01,0.01};
//const float CUT_HOT_WEDGE_ST0[2] 	= {1.5,1.5};
//const float CUT_COLD_WEDGE_ST1[2] = {0.01,0.01};
//const float CUT_HOT_WEDGE_ST1[2] 	= {3.0,3.0};
//---------------------
//Run-16 d+Au 20 GeV
//const float MAXHITSPERWEDGE[2] = {10,10};//{2.,1.5};
//const float CUT_COLD_WEDGE_ST0[2] = {0.01,0.01};
//const float CUT_HOT_WEDGE_ST0[2] 	= {1.5, 1.};
//const float CUT_COLD_WEDGE_ST1[2] = {0.01,0.01};
//const float CUT_HOT_WEDGE_ST1[2] 	= {1.5, 1.};
//=====================


//=====================
//! chip vs. channel maximum
//! fvtxDraw("fvtxChipVsChannelW")
//! fvtxDraw("fvtxChipVsChannelE")
//---------------------
//const float MAX_CHIP_CHANNEL 	= 0.01;		//Run-12 Cu+Au 200
//---------------------
//const float MAX_CHIP_CHANNEL 	= 0.001;	//Run-13 p+p 510 
//---------------------
//const float MAX_CHIP_CHANNEL 	= 0.01;		//Run-14 Au+Au 200 
//---------------------
//const float MAX_CHIP_CHANNEL 	= 0.002;	//Run-14 He+Au 200
//---------------------
//const float MAX_CHIP_CHANNEL 		= 0.0003;	//Run-15 p+p 200 
//---------------------
//const float MAX_CHIP_CHANNEL 		= 0.0008;	//Run-15 p+Au 200 
//---------------------
//const float MAX_CHIP_CHANNEL 		= 0.00045;	//Run-15 p+Al 200 
//---------------------
const float MAX_CHIP_CHANNEL 	= 0.01;		//Run-16 Au+Au 200 yuhw
//---------------------
//const float MAX_CHIP_CHANNEL 	= 0.001;		//Run-16 d+Au 200 yuhw
//---------------------
//const float MAX_CHIP_CHANNEL 	= 0.0008;		//Run-16 d+Au 62GeV
//---------------------
//const float MAX_CHIP_CHANNEL 	= 0.0008;		//Run-16 d+Au 20GeV
//=====================

//=====================
//! Max for yield per packet vs. time
//! fvtxDraw("fvtxYieldsByPacketVsTime")
//const float MAX_YIELD_PER_PACKET_VS_TIME				=	15.; //Run-15 p+p 200
//const float MAX_YIELD_PER_PACKET_VS_TIME				= 25.; //Run-15 p+Au 200
//const float MAX_YIELD_PER_PACKET_VS_TIME				= 25.; //Run-15 p+Al 200
const float MAX_YIELD_PER_PACKET_VS_TIME				=  150;//linear scale //5000;//log scale //Run-16 Au+Au 200 yuhw
//const float MAX_YIELD_PER_PACKET_VS_TIME				= 1000.; //Run-16 d+Au 200GeV
//const float MAX_YIELD_PER_PACKET_VS_TIME				= 20.; //Run-16 d+Au 62GeV
//const float MAX_YIELD_PER_PACKET_VS_TIME				= 5.; //Run-16 d+Au 20GeV
//=====================

//=====================
//! Max for yield per packet vs. time short
//! fvtxDraw("fvtxYieldsByPacketVsTimeShort")
//const float MAX_YIELD_PER_PACKET_VS_TIME_SHORT	=	6.0; //Run-15 p+p 200
//const float MAX_YIELD_PER_PACKET_VS_TIME_SHORT	=	10.; //Run-15 p+Au 200
//const float MAX_YIELD_PER_PACKET_VS_TIME_SHORT	=	10.; //Run-15 p+Al 200
const float MAX_YIELD_PER_PACKET_VS_TIME_SHORT	=	200.; //Run-16 Au+Au 200 yuhw
//const float MAX_YIELD_PER_PACKET_VS_TIME_SHORT	=	30.; //Run-16 d+Au 200 yuhw
//const float MAX_YIELD_PER_PACKET_VS_TIME_SHORT	=	30.; //Run-16 d+Au 62GeV
//const float MAX_YIELD_PER_PACKET_VS_TIME_SHORT	=	15.; //Run-16 d+Au 20GeV
//=====================

#endif // __FVTXMONDRAWCONSTS_H__
