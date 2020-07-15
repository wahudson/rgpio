// 2020-06-16  William A. Hudson

#ifndef rgIic_P
#define rgIic_P

#include "rgRegister.h"

//--------------------------------------------------------------------------
// rGPIO IIC (I2C) Master class.
//--------------------------------------------------------------------------

class rgIic_Cntl : public rgRegister {
  public:

    uint32_t	get_IicEnable_1()         { return  get_field( 15, 0x1    ); }
    void	put_IicEnable_1(     uint32_t v ) { put_field( 15, 0x1, v ); }

    uint32_t	get_IrqRxHalf_1()         { return  get_field( 10, 0x1    ); }
    void	put_IrqRxHalf_1(     uint32_t v ) { put_field( 10, 0x1, v ); }

    uint32_t	get_IrqTxHalf_1()         { return  get_field(  9, 0x1    ); }
    void	put_IrqTxHalf_1(     uint32_t v ) { put_field(  9, 0x1, v ); }

    uint32_t	get_IrqDone_1()           { return  get_field(  8, 0x1    ); }
    void	put_IrqDone_1(       uint32_t v ) { put_field(  8, 0x1, v ); }

    uint32_t	get_StartTrans_1()        { return  get_field(  7, 0x1    ); }
    void	put_StartTrans_1(    uint32_t v ) { put_field(  7, 0x1, v ); }

    uint32_t	get_ClearFifo_2()         { return  get_field(  4, 0x3    ); }
    void	put_ClearFifo_2(     uint32_t v ) { put_field(  4, 0x3, v ); }

    uint32_t	get_ReadPacket_1()        { return  get_field(  0, 0x1    ); }
    void	put_ReadPacket_1(    uint32_t v ) { put_field(  0, 0x1, v ); }

};

class rgIic_Stat   : public rgRegister {
  public:

    uint32_t	get_ClkTimeout_1()        { return  get_field(  9, 0x1    ); }
    void	put_ClkTimeout_1(    uint32_t v ) { put_field(  9, 0x1, v ); }

    uint32_t	get_AckErr_1()            { return  get_field(  8, 0x1    ); }
    void	put_AckErr_1(        uint32_t v ) { put_field(  8, 0x1, v ); }

    uint32_t	get_RxFull_1()            { return  get_field(  7, 0x1    ); }
    void	put_RxFull_1(        uint32_t v ) { put_field(  7, 0x1, v ); }

    uint32_t	get_TxEmpty_1()           { return  get_field(  6, 0x1    ); }
    void	put_TxEmpty_1(       uint32_t v ) { put_field(  6, 0x1, v ); }

    uint32_t	get_RxHasData_1()         { return  get_field(  5, 0x1    ); }
    void	put_RxHasData_1(     uint32_t v ) { put_field(  5, 0x1, v ); }

    uint32_t	get_TxHasSpace_1()        { return  get_field(  4, 0x1    ); }
    void	put_TxHasSpace_1(    uint32_t v ) { put_field(  4, 0x1, v ); }

    uint32_t	get_RxHalf_1()            { return  get_field(  3, 0x1    ); }
    void	put_RxHalf_1(        uint32_t v ) { put_field(  3, 0x1, v ); }

    uint32_t	get_TxHalf_1()            { return  get_field(  2, 0x1    ); }
    void	put_TxHalf_1(        uint32_t v ) { put_field(  2, 0x1, v ); }

    uint32_t	get_TransDone_1()         { return  get_field(  1, 0x1    ); }
    void	put_TransDone_1(     uint32_t v ) { put_field(  1, 0x1, v ); }

    uint32_t	get_TransActive_1()       { return  get_field(  0, 0x1    ); }
    void	put_TransActive_1(   uint32_t v ) { put_field(  0, 0x1, v ); }

};

class rgIic_DatLen : public rgRegister {
  public:
    uint32_t	get_DataLen_16()          { return  get_field(  0, 0xffff ); }
    void	put_DataLen_16(      uint32_t v ) { put_field(  0, 0xffff, v); }
};

class rgIic_Addr   : public rgRegister {
  public:
    uint32_t	get_SlaveAddr_7()         { return  get_field(  0, 0x7f   ); }
    void	put_SlaveAddr_7(     uint32_t v ) { put_field(  0, 0x7f, v); }
};

class rgIic_Fifo   : public rgRegister {
    // No field accessor, typically use full word read/write.
};

class rgIic_ClkDiv : public rgRegister {
  public:
    uint32_t	get_ClkDiv_16()           { return  get_field(  0, 0xffff ); }
    void	put_ClkDiv_16(       uint32_t v ) { put_field(  0, 0xffff, v); }
};

class rgIic_Delay  : public rgRegister {
  public:

    uint32_t	get_Fall2Out_16()         { return  get_field( 16, 0xffff ); }
    void	put_Fall2Out_16(     uint32_t v ) { put_field( 16, 0xffff, v); }

    uint32_t	get_Rise2In_16()          { return  get_field(  0, 0xffff ); }
    void	put_Rise2In_16(      uint32_t v ) { put_field(  0, 0xffff, v); }
};

class rgIic_ClkStr : public rgRegister {
  public:
    uint32_t	get_TimeOut_16()          { return  get_field(  0, 0xffff ); }
    void	put_TimeOut_16(      uint32_t v ) { put_field(  0, 0xffff, v); }
};


class rgIic {
  private:
    volatile uint32_t	*GpioBase;	// IO base address
    uint32_t		IicNum;		// IIC unit number {0,1,2}
    uint32_t		FeatureAddr;	// BCM doc value, in constructor

  public:
				// Registers
    rgIic_Cntl		Cntl;		// C     Control
    rgIic_Stat		Stat;		// S     Status
    rgIic_DatLen	DatLen;		// DLEN  Data Length
    rgIic_Addr		Addr;		// A     Slave Address
    rgIic_Fifo		Fifo;		// FIFO  Data FIFO
    rgIic_ClkDiv	ClkDiv;		// DIV   Clock Divider
    rgIic_Delay		Delay;		// DEL   Data Delay
    rgIic_ClkStr	ClkStr;		// CLKT  Clock Stretch Timeout

  private:
				// Register word offset in Page
    static const uint32_t	Cntl_offset      = 0x00 /4;
    static const uint32_t	Stat_offset      = 0x04 /4;
    static const uint32_t	DatLen_offset    = 0x08 /4;
    static const uint32_t	Addr_offset      = 0x0c /4;
    static const uint32_t	Fifo_offset      = 0x10 /4;
    static const uint32_t	ClkDiv_offset    = 0x14 /4;
    static const uint32_t	Delay_offset     = 0x18 /4;
    static const uint32_t	ClkStr_offset    = 0x1c /4;
    //##!  maybe put in constructor instead

  public:
    rgIic(			// constructor
	uint32_t	iicnum,		// IIC unit number {0,1,2}
	rgAddrMap	*xx
    );

    inline
    volatile uint32_t*	get_base_addr()  { return  GpioBase; }
    uint32_t		get_iic_num()    { return  IicNum; }

		// Object state operations
    void		init_put_reset();

    void		grab_regs();
    void		push_regs();

		// Test/Debug accessors

    void		show_debug( std::ostream&  sout );

    inline uint32_t	get_bcm_address() { return FeatureAddr; }
};

#endif

