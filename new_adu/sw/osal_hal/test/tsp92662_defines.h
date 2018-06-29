#ifndef _TSP92662_DEFINES_H
#define _TSP92662_DEFINES_H
// LMM2 Frame Initialization Byte
typedef enum
{
	WRITE1  = 0x87,
	WRITE2  = 0x99,
	WRITE3  = 0x1E,
	WRITE4  = 0xAA,
	WRITE12 = 0x2D,
	WRITE16 = 0x33,
	WRITE32 = 0xB4,

	READ1  = 0x4B,
	READ2  = 0xCC,
	READ3  = 0xD2,
	READ4  = 0x55,
	READ12 = 0xE1,
	READ16 = 0x66,
	READ32 = 0x78,
}FrameInit;

// LMM2 Register Addresses
typedef enum
{
    // LED registers
    MPHASE01L = 0x00,
    MWIDTH01L = 0x10,
    LPHASE01H = 0x20,
    LWIDTH01H = 0x30,

    DWIDTH01L = 0x42,
    DWIDTH02L = 0x46,
    DWIDTH03L = 0x4A,
    DWIDTH04L = 0x4E,
    DWIDTH05L = 0x52,
    DWIDTH06L = 0x56,
    DWIDTH07L = 0x5A,
    DWIDTH08L = 0x5E,
    DWIDTH09L = 0x62,
    DWIDTH10L = 0x66,
    DWIDTH11L = 0x6A,
    DWIDTH12L = 0x6E,

    DWIDTH01H = 0x43,
    DWIDTH02H = 0x47,
    DWIDTH03H = 0x4B,
    DWIDTH04H = 0x4F,
    DWIDTH05H = 0x53,
    DWIDTH06H = 0x57,
    DWIDTH07H = 0x5B,
    DWIDTH08H = 0x5F,
    DWIDTH09H = 0x63,
    DWIDTH10H = 0x67,
    DWIDTH11H = 0x6B,
    DWIDTH12H = 0x6F,

	// Control registers
	SLEWRATE = 0x70,
	OVLMT = 0x71,
	PARLED = 0x72,
	DEFWIDTH02_01 = 0x73,
	DEFWIDTH04_03 = 0x74,
	DEFWIDTH06_05 = 0x75,
	DEFWIDTH08_07 = 0x76,
	DEFWIDTH10_09 = 0x77,
	DEFWIDTH12_11 = 0x78,
	SYSCFG = 0x80,
	CMWTAP = 0x81,
	PWMTICK = 0x82,
	ADCID = 0x83,
	SOFTSYNC = 0x84,

	// I2C registers
	I2CTICK = 0x90,
	I2CSLA = 0x91,
	I2CRLEN = 0x92,
	I2CADR = 0x93,
	I2CCTRL = 0x94,
	I2CDAT = 0x95,
	I2CSTAT = 0x96,
	I2CBMON = 0x97,

	// ADC registers
	ADC1 = 0xA0,
	ADC2 = 0xA1,

	// Diagnostic registers
	FAULTL = 0xB0,
	FAULTH = 0xB1,
	CERRCNT = 0xB2,
	ICID = 0xFF
}RegAddr;

#endif
