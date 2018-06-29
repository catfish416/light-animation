#ifdef __cplusplus
extern "C" {
#endif

#include "hal_uart.h"
#include <inttypes.h>
#include <stdbool.h>
#include "sci.h"
#include "led_uart.h"

// just for debug
#define DEBUG_BRIGHT_VALUE  0x05
#define DEBUG_DARK_VALUE  0x00

#define SLEEP_SEC     1

#define RD_LEN  11
#define	LMMBUFDEPTH		40	// Max MCU-to-LMM is 32-byte write = 37 bytes

#ifdef __cplusplus
#pragma DATA_SECTION("ScibRegsFile")
#else
#pragma DATA_SECTION(ScibRegs,"ScibRegsFile");
#endif

volatile struct SCI_REGS ScibRegs;

#define	LMM_RX_DIS()		(ScibRegs.SCICTL1.bit.RXENA = 0)
#define	LMM_RX_EN()			(ScibRegs.SCICTL1.bit.RXENA = 1)

uint8_t sepTR           = true;						// Flag that indicates whether we are in single-ended or CAN modes
uint8_t acksEnabledFlag = false;			// Set this if we enable ACKs from the LMM
uint8_t lmmTOFlag       = false;					// This flag will be set true when LMM does NOT respond
extern int serial_fd;


// Physical to communications address look-up (including parity)
const uint8_t PHY_TO_COMM[32] =
{//  1     2     3     4     5     6
  //0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    0x20, 0x61, 0xE2, 0xA3, 0x64, 0x25, 0xA6, 0xE7, 0xA8, 0xE9, 0x6A, 0x2B, 0xEC, 0xAD, 0x2E, 0x6F,

  //0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
  0xF0, 0xB1, 0x32, 0x73, 0xB4, 0xF5, 0x76, 0x37, 0x78, 0x39, 0xBA, 0xFB, 0x3C, 0x7D, 0xFE, 0xBF
};

// Physical to width reg address look-up
const uint8_t PHY_TO_LED_INDEX_L[13] =
{
    0x0, DWIDTH01L, DWIDTH02L, DWIDTH03L, DWIDTH04L, DWIDTH05L, DWIDTH06L,

         DWIDTH07L, DWIDTH08L, DWIDTH09L, DWIDTH10L, DWIDTH11L, DWIDTH12L,
};

const uint8_t PHY_TO_LED_INDEX_H[13] =
{
    0x0, DWIDTH01H, DWIDTH02H, DWIDTH03H, DWIDTH04H, DWIDTH05H, DWIDTH06H,

         DWIDTH07H, DWIDTH08H, DWIDTH09H, DWIDTH10H, DWIDTH11H, DWIDTH12H,
};

uint16_t Crc_16_Ibm(uint8_t *buf, uint8_t len)
{
	uint16_t crc = 0;
	uint16_t i;

	while (len--)
	{
		crc ^= *buf++;
		for (i = 0; i < 8; i++)
		{
			crc = (crc >> 1) ^ ((crc & 1) ? 0xA001 : 0);
		}
	}

	return crc;
}

// Do this polled for evaluation as more deterministic
void LmmTx(uint8_t *buf, uint8_t len)
{
	uint16_t i;

	// Check if we need to ignore CAN echo
	if(sepTR == false)
	{
		LMM_RX_DIS();
	}

	for(i = 0; i < len; i++)
	{
		// Wait until the Tx buffer is clear
		while(ScibRegs.SCICTL2.bit.TXRDY == 0);
		// Place byte in the Tx buffer
		ScibRegs.SCITXBUF.bit.TXDT = *(buf + i);
	}

	// All data is loaded, wait for it to leave
	if(sepTR == false)
	{
		// Wait until shift completes
		while(ScibRegs.SCICTL2.bit.TXEMPTY == 0);
		// The C2000 seems to set this bit prematurely
		// Re-enable the receiver
		LMM_RX_EN();
		// And wait on the looped back byte to complete
		while(ScibRegs.SCIRXST.bit.RXRDY == 0);
		// Then read it out (i because it's available)
		i = ScibRegs.SCIRXBUF.bit.RXDT;
	}
}

int LmmWrReg(FrameInit command, uint8_t lmm, uint8_t regAddr, uint8_t* data_ptr)
{
	uint16_t crc = 0;
	uint16_t numDataBytes;
	uint8_t writeBuf[LMMBUFDEPTH];
	uint8_t readAck = 0x00;

	// Assemble the packet in the pre-buffer and CRC it
	writeBuf[0] = command;
	writeBuf[1] = PHY_TO_COMM[lmm];
	writeBuf[2] = regAddr;

	switch(command)
	{
	case WRITE1:
		numDataBytes = 1;
		break;

	case WRITE2:
		numDataBytes = 2;
		break;

	case WRITE3:
		numDataBytes = 3;
		break;

	case WRITE4:
		numDataBytes = 4;
		break;

	case WRITE12:
		numDataBytes = 12;
		break;

	case WRITE16:
		numDataBytes = 16;
		break;

	case WRITE32:
		numDataBytes = 32;
		break;

	default:
		numDataBytes = 0;
		//TODO: Do we need to exit this function here?
		break;
	}
	// Just use "crc" variable because it's available
	for(crc = 0; crc < numDataBytes; crc++)
	{
		writeBuf[(3 + crc)] = *data_ptr++;
	}

	// Process the CRC over these bytes
	crc = Crc_16_Ibm(writeBuf, (3 + numDataBytes));

    // And load it into the final 2 bytes of the packet
	writeBuf[(3 + numDataBytes)] = (crc & 0x00FF);         // LSByte of CRC
	writeBuf[(4 + numDataBytes)] = ((crc >> 8) & 0x00FF);  // MSByte of CRC

    // Now we can send it to the TPS92662 bus
//    printf("Now we can send data to the TPS92662 bus \n");
//    LmmTx(writeBuf, (5 + numDataBytes));

    return uart_send(serial_fd, writeBuf, (5 + numDataBytes));
#if 0
    // Do we expect an ACK?
    if(acksEnabledFlag)
    {
    	// Clear the timeout flag
    	lmmTOFlag = FALSE;
		// Start the timeout
		StartCpuTimer1();
		// Wait for the byte to come back
		while((ScibRegs.SCIRXST.bit.RXRDY == 0) && (lmmTOFlag == FALSE));
		if(lmmTOFlag)
		{
			//TODO: Auto comms reset?
		}
		else
		{
			// We can now stop the timer
			StopCpuTimer1();
			// Get the byte received
			readAck = ScibRegs.SCIRXBUF.bit.SAR;
			if(readAck != 0x7F)
			{
				//TODO: Auto comms reset?
			}
		}
    }
#endif
} 

#define H8_MASK     0xFF
#define L2_MASK     0x03


void discrete_data_high_low(uint16_t x, uint8_t *high, uint8_t *low)
{
    *high = x >> 8;
    *high &= L2_MASK;

    *low = x & H8_MASK;
/*
    printf("data = %d\n", x);
    printf("high 2 bits = %d\n", *high);
    printf("low 8 bits = %d\n", *low);
*/
}

int send_led_uart_data(FrameInit command, uint8_t lmm, uint8_t led_index, uint16_t* data_ptr)
{
    // for debug
//    spc(4); printf("cmd: 0x%x\n", command);
//    spc(4); printf("lmm: %d\n", lmm);
//    spc(4); printf("led_index: %d\n", led_index);
//    spc(4); printf("data: %d\n", *data_ptr);
//    printf("\n");

    uint16_t tmp_data = *data_ptr;
    uint8_t high, low;
    discrete_data_high_low(tmp_data, &high, &low);

    if (LmmWrReg(command, lmm, PHY_TO_LED_INDEX_L[led_index], &low) < 0)
        return -1;
    if (LmmWrReg(command, lmm, PHY_TO_LED_INDEX_H[led_index], &high) < 0)
        return -1;

    return 0;
}
#if 0
int main(int argc, char **argv) 
{
    serial_fd = init_uart();
    if (serial_fd < 0)
    {
        printf("init serial failed!\n");
        return -1;
    }
  
    char buf_w[] = "hello world";
    char buf_r[RD_LEN] ; 
    memset(buf_r, 0, sizeof(char)*RD_LEN);

//    uint8_t dataByte[] = { 0xff, 0x03 };
//    LmmWrReg(WRITE2, 4, 0x46, dataByte);  // set lmm 0 to parallel mode
    uint8_t dataByte = 0;
    for(;;)
    {
        dataByte = DEBUG_BRIGHT_VALUE;
        LmmWrReg(WRITE1, 1, DWIDTH01L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH02L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH03L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH04L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH05L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH06L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH07L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH08L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH09L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH10L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH11L, &dataByte);
        sleep(SLEEP_SEC);
        LmmWrReg(WRITE1, 1, DWIDTH12L, &dataByte);
        sleep(SLEEP_SEC);
        dataByte = DEBUG_DARK_VALUE;
        LmmWrReg(WRITE1, 1, DWIDTH01L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH02L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH03L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH04L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH05L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH06L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH07L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH08L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH09L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH10L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH11L, &dataByte);
        LmmWrReg(WRITE1, 1, DWIDTH12L, &dataByte);
        sleep(3);
        tcflush(serial_fd, TCOFLUSH);
        printf("bbb\n");
    }
    

//    uart_send(serial_fd, buf_w, strlen(buf_w));
    //printf("\n");
/*
    while(1)
    {
        uart_send(serial_fd, buf_w, strlen(buf_w));
        printf("\n");
        sleep(1);
        //if(uart_recv(serial_fd, buf_r, 11) > 0)
        uart_recv(serial_fd, buf_r, RD_LEN);
        printf("receive: %s\n", buf_r);
        memset(buf_r, 0, sizeof(char)*RD_LEN);
    }
*/
    close(serial_fd); 
    return 0; 
}
#endif

#ifdef __cplusplus
}
#endif /* end of __cplusplus */
