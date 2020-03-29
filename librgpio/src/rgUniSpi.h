// 2018-01-24  William A. Hudson

#ifndef rgUniSpi_P
#define rgUniSpi_P

#include "rgRegister.h"

//--------------------------------------------------------------------------
// rGPIO Universal SPI Master class.  Spi1, Spi2
//--------------------------------------------------------------------------

class rgUniSpi_AuxIrq : public rgRegister {	// Read-Only
  public:

    inline
    uint32_t	get_Spi2Irq_1()           { return  get_field(  2, 0x1    ); }
    void	put_Spi2Irq_1(       uint32_t v ) { put_field(  2, 0x1, v ); }

    uint32_t	get_Spi1Irq_1()           { return  get_field(  1, 0x1    ); }
    void	put_Spi1Irq_1(       uint32_t v ) { put_field(  1, 0x1, v ); }

    uint32_t	get_MiniUartIrq_1()       { return  get_field(  0, 0x1    ); }
    void	put_MiniUartIrq_1(   uint32_t v ) { put_field(  0, 0x1, v ); }
};

class rgUniSpi_AuxEn : public rgRegister {
  public:

    inline
    uint32_t	get_Spi2Enable_1()        { return  get_field(  2, 0x1    ); }
    void	put_Spi2Enable_1(    uint32_t v ) { put_field(  2, 0x1, v ); }

    uint32_t	get_Spi1Enable_1()        { return  get_field(  1, 0x1    ); }
    void	put_Spi1Enable_1(    uint32_t v ) { put_field(  1, 0x1, v ); }

    uint32_t	get_MiniUartEnable_1()     { return  get_field(  0, 0x1    ); }
    void	put_MiniuartEnable_1( uint32_t v ) { put_field(  0, 0x1, v ); }
};

class rgUniSpi_Cntl0 : public rgRegister {
  public:

    inline
    uint32_t	get_Speed_12()            { return  get_field( 20, 0xfff   ); }
    void	put_Speed_12(        uint32_t v ) { put_field( 20, 0xfff, v); }

    uint32_t	get_ChipSelects_3()       { return  get_field( 17, 0x7    ); }
    void	put_ChipSelects_3(   uint32_t v ) { put_field( 17, 0x7, v ); }

    uint32_t	get_PostInMode_1()        { return  get_field( 16, 0x1    ); }
    void	put_PostInMode_1(    uint32_t v ) { put_field( 16, 0x1, v ); }

    uint32_t	get_VariableCs_1()        { return  get_field( 15, 0x1    ); }
    void	put_VariableCs_1(    uint32_t v ) { put_field( 15, 0x1, v ); }

    uint32_t	get_VariableWidth_1()     { return  get_field( 14, 0x1    ); }
    void	put_VariableWidth_1( uint32_t v ) { put_field( 14, 0x1, v ); }

    uint32_t	get_DoutHoldTime_2()      { return  get_field( 12, 0x3    ); }
    void	put_DoutHoldTime_2(  uint32_t v ) { put_field( 12, 0x3, v ); }

    uint32_t	get_EnableSerial_1()      { return  get_field( 11, 0x1    ); }
    void	put_EnableSerial_1(  uint32_t v ) { put_field( 11, 0x1, v ); }

    uint32_t	get_InRising_1()          { return  get_field( 10, 0x1    ); }
    void	put_InRising_1(      uint32_t v ) { put_field( 10, 0x1, v ); }

    uint32_t	get_ClearFifos_1()        { return  get_field(  9, 0x1    ); }
    void	put_ClearFifos_1(    uint32_t v ) { put_field(  9, 0x1, v ); }

    uint32_t	get_OutRising_1()         { return  get_field(  8, 0x1    ); }
    void	put_OutRising_1(     uint32_t v ) { put_field(  8, 0x1, v ); }

    uint32_t	get_InvertClk_1()         { return  get_field(  7, 0x1    ); }
    void	put_InvertClk_1(     uint32_t v ) { put_field(  7, 0x1, v ); }

    uint32_t	get_OutMsbFirst_1()       { return  get_field(  6, 0x1    ); }
    void	put_OutMsbFirst_1(   uint32_t v ) { put_field(  6, 0x1, v ); }

    uint32_t	get_ShiftLength_6()       { return  get_field(  0, 0x3f   ); }
    void	put_ShiftLength_6(   uint32_t v ) { put_field(  0, 0x3f, v); }
};

class rgUniSpi_Cntl1 : public rgRegister {
  public:

    inline
    uint32_t	get_CsHighTime_3()        { return  get_field(  8, 0x7    ); }
    void	put_CsHighTime_3(    uint32_t v ) { put_field(  8, 0x7, v ); }

    uint32_t	get_TxEmptyIrq_1()        { return  get_field(  7, 0x1    ); }
    void	put_TxEmptyIrq_1(    uint32_t v ) { put_field(  7, 0x1, v ); }

    uint32_t	get_DoneIrq_1()           { return  get_field(  6, 0x1    ); }
    void	put_DoneIrq_1(       uint32_t v ) { put_field(  6, 0x1, v ); }

    uint32_t	get_InMsbFirst_1()        { return  get_field(  1, 0x1    ); }
    void	put_InMsbFirst_1(    uint32_t v ) { put_field(  1, 0x1, v ); }

    uint32_t	get_KeepInput_1()         { return  get_field(  0, 0x1    ); }
    void	put_KeepInput_1(     uint32_t v ) { put_field(  0, 0x1, v ); }
};

class rgUniSpi_Stat  : public rgRegister {	// Read-Only
  public:

    inline
    uint32_t	get_TxLevel_3()           { return  get_field( 28, 0x7    ); }
    void	put_TxLevel_3(       uint32_t v ) { put_field( 28, 0x1, v ); }

    uint32_t	get_RxLevel_3()           { return  get_field( 20, 0x7    ); }
    void	put_RxLevel_3(       uint32_t v ) { put_field( 20, 0x1, v ); }

    uint32_t	get_TxFull_1()            { return  get_field( 10, 0x1    ); }
    void	put_TxFull_1(        uint32_t v ) { put_field( 10, 0x1, v ); }

    uint32_t	get_TxEmpty_1()           { return  get_field(  9, 0x1    ); }
    void	put_TxEmpty_1(       uint32_t v ) { put_field(  9, 0x1, v ); }

    uint32_t	get_RxFull_1()            { return  get_field(  8, 0x1    ); }
    void	put_RxFull_1(        uint32_t v ) { put_field(  8, 0x1, v ); }

    uint32_t	get_RxEmpty_1()           { return  get_field(  7, 0x1    ); }
    void	put_RxEmpty_1(       uint32_t v ) { put_field(  7, 0x1, v ); }

    uint32_t	get_Busy_1()              { return  get_field(  6, 0x1    ); }
    void	put_Busy_1(          uint32_t v ) { put_field(  6, 0x1, v ); }

    uint32_t	get_BitCount_6()          { return  get_field(  0, 0x3f   ); }
    void	put_BitCount_6(      uint32_t v ) { put_field(  0, 0x1, v ); }
};

class rgUniSpi_Peek  : public rgRegister {	// Read-Only
};

class rgUniSpi_Fifo  : public rgRegister {
};

class rgUniSpi_FifoH : public rgRegister {
};


class rgUniSpi {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

    uint32_t		SpiNum;		// SPI number {1,2}

  public:
				// Registers
    rgUniSpi_AuxIrq	AuxIrq;		// AUXIRQ  Auxiliary Interrupt Req (RO)
    rgUniSpi_AuxEn	AuxEn;		// AUXENB  Auxiliary Enable

    rgUniSpi_Cntl0	Cntl0;		// CNTL0   Control 0
    rgUniSpi_Cntl1	Cntl1;		// CNTL1   Control 1
    rgUniSpi_Stat	Stat;		// STAT    Status  (RO)
    rgUniSpi_Peek	Peek;		// PEEK    Rx Fifo top entry  (RO)
    rgUniSpi_Fifo	Fifo;		// IO      Fifo end transaction
    rgUniSpi_FifoH	FifoH;		// TXHOLD  Fifo extend transaction

  private:
//#!! static
    const uint32_t	FeatureAddr  = 0x7e215000;	// BCM doc

				// Aux Register offsets
    const uint32_t	AuxIrq_offset    = 0x00 /4;
    const uint32_t	AuxEn_offset     = 0x04 /4;

				// Register word offset in Page for Spi1
    const uint32_t	Cntl0_offset     = 0x80 /4;
    const uint32_t	Cntl1_offset     = 0x84 /4;
    const uint32_t	Stat_offset      = 0x88 /4;
    const uint32_t	Peek_offset      = 0x8c /4;

    const uint32_t	Fifo_offset      = 0xA0 /4;
    const uint32_t	FifoH_offset     = 0xB0 /4;

  public:
    rgUniSpi(			// constructor
	uint32_t	spinum,		// SPI number {1,2}
	rgAddrMap	*xx
    );

    inline volatile uint32_t*	get_base_addr()  { return  GpioBase; }
    inline uint32_t		get_spi_num()    { return  SpiNum; }

		// Access Enable and IRQ bits

    uint32_t		read_SpiIrq_1();
    uint32_t		read_SpiEnable_1();
    void		write_SpiEnable_1( uint32_t val );

		// Object state operations
    void		init_put_reset();

    void		grab_regs();
    void		push_regs();

		// Test/Debug accessors

    void		show_debug( std::ostream&  sout );

    inline uint32_t	get_bcm_address() { return FeatureAddr; }
};

#endif

