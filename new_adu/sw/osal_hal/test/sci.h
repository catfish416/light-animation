//---------------------------------------------------------------------------
// SCI Individual Register Bit Definitions

//----------------------------------------------------------
// SCICCR communication control register bit definitions:
//
                                              
struct  SCICCR_BITS {      // bit    description
   uint16_t SCICHAR:3;         // 2:0    Character length control        
   uint16_t ADDRIDLE_MODE:1;   // 3      ADDR/IDLE Mode control
   uint16_t LOOPBKENA:1;       // 4      Loop Back enable
   uint16_t PARITYENA:1;       // 5      Parity enable   
   uint16_t PARITY:1;          // 6      Even or Odd Parity
   uint16_t STOPBITS:1;        // 7      Number of Stop Bits
   uint16_t rsvd1:8;           // 15:8   reserved
}; 

union SCICCR_REG {
   uint16_t                all;
   struct SCICCR_BITS  bit;
};

//-------------------------------------------
// SCICTL1 control register 1 bit definitions:
//
                       
struct  SCICTL1_BITS {     // bit    description
   uint16_t RXENA:1;           // 0      SCI receiver enable
   uint16_t TXENA:1;           // 1      SCI transmitter enable
   uint16_t SLEEP:1;           // 2      SCI sleep  
   uint16_t TXWAKE:1;          // 3      Transmitter wakeup method
   uint16_t rsvd:1;            // 4      reserved
   uint16_t SWRESET:1;         // 5      Software reset   
   uint16_t RXERRINTENA:1;     // 6      Recieve interrupt enable
   uint16_t rsvd1:9;           // 15:7   reserved

}; 

union SCICTL1_REG {
   uint16_t                 all;
   struct SCICTL1_BITS  bit;
};

//---------------------------------------------
// SCICTL2 control register 2 bit definitions:
// 

struct  SCICTL2_BITS {     // bit    description
   uint16_t TXINTENA:1;        // 0      Transmit interrupt enable    
   uint16_t RXBKINTENA:1;      // 1      Receiver-buffer break enable
   uint16_t rsvd:4;            // 5:2    reserved
   uint16_t TXEMPTY:1;         // 6      Transmitter empty flag
   uint16_t TXRDY:1;           // 7      Transmitter ready flag  
   uint16_t rsvd1:8;           // 15:8   reserved

}; 

union SCICTL2_REG {
   uint16_t                 all;
   struct SCICTL2_BITS  bit;
};

//---------------------------------------------------
// SCIRXST Receiver status register bit definitions:
//

struct  SCIRXST_BITS {     // bit    description
   uint16_t rsvd:1;            // 0      reserved
   uint16_t RXWAKE:1;          // 1      Receiver wakeup detect flag
   uint16_t PE:1;              // 2      Parity error flag
   uint16_t OE:1;              // 3      Overrun error flag
   uint16_t FE:1;              // 4      Framing error flag
   uint16_t BRKDT:1;           // 5      Break-detect flag   
   uint16_t RXRDY:1;           // 6      Receiver ready flag
   uint16_t RXERR:1;           // 7      Receiver error flag

}; 

union SCIRXST_REG {
   uint16_t                 all;
   struct SCIRXST_BITS  bit;
};

//----------------------------------------------------
// SCIRXBUF Receiver Data Buffer with FIFO bit definitions:
// 

struct  SCIRXBUF_BITS {    // bits   description
   uint16_t RXDT:8;            // 7:0    Receive word
   uint16_t rsvd:6;            // 13:8   reserved
   uint16_t SCIFFPE:1;         // 14     SCI PE error in FIFO mode
   uint16_t SCIFFFE:1;         // 15     SCI FE error in FIFO mode
};

union SCIRXBUF_REG {
   uint16_t                  all;
   struct SCIRXBUF_BITS  bit;
};

struct SCITXBUF_BITS {
    uint16_t TXDT:8;            // 7:0    Send word
    uint16_t rsvd:8;            // 15:8   reserved
};

union SCITXBUF_REG {
   uint16_t                  all;
   struct SCITXBUF_BITS  bit;
};

//--------------------------------------------------
// SCIPRI Priority control register bit definitions:
// 
//
                                                   
struct  SCIPRI_BITS {      // bit    description
   uint16_t rsvd:3;            // 2:0    reserved
   uint16_t FREE:1;            // 3      Free emulation suspend mode
   uint16_t SOFT:1;            // 4      Soft emulation suspend mode
   uint16_t rsvd1:3;           // 7:5    reserved
}; 

union SCIPRI_REG {
   uint16_t                all;
   struct SCIPRI_BITS  bit;
};



//---------------------------------------------------------------------------
// SCI Register File:
//
struct  SCI_REGS {
   union SCICCR_REG     SCICCR;     // Communications control register
   union SCICTL1_REG    SCICTL1;    // Control register 1
   uint16_t               SCIHBAUD;   // Baud rate (high) register
   uint16_t               SCILBAUD;   // Baud rate (low) register
   union SCICTL2_REG    SCICTL2;    // Control register 2
   union SCIRXST_REG    SCIRXST;    // Recieve status register
   uint16_t               SCIRXEMU;   // Recieve emulation buffer register
   union SCIRXBUF_REG   SCIRXBUF;   // Recieve data buffer  
   uint16_t  rsvd1;                   // reserved
   union SCITXBUF_REG   SCITXBUF;   // Transmit data buffer 
   uint16_t rsvd2; 
   uint16_t rsvd3;                    // reserved
   uint16_t rsvd4;                    // reserved
   uint16_t rsvd5;                    // reserved
   uint16_t rsvd6;                    // reserved
   union SCIPRI_REG      SCIPRI;    // FIFO Priority control   
};

