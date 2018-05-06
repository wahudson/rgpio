// 2018-01-24  William A. Hudson

#ifndef rgUniSpi_P
#define rgUniSpi_P

//--------------------------------------------------------------------------
// rGPIO Universal SPI Master class.  Spi1, Spi2
//--------------------------------------------------------------------------

class rgUniSpi {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

    uint32_t		SpiNum;		// SPI number {1,2}

    uint32_t		Cntl0Reg;	// Control register value
    uint32_t		Cntl1Reg;	// Control register value
    uint32_t		StatReg;	// Status  register value

  private:
    const uint32_t	FeatureAddr  = 0x7e215000;	// BCM doc

				// Aux Register offsets
    const uint32_t	AuxIrq_offset    = 0x00 /4;
    const uint32_t	AuxEnable_offset = 0x04 /4;

				// Register word offset in Page for Spi1
    const uint32_t	Cntl0_offset     = 0x80 /4;
    const uint32_t	Cntl1_offset     = 0x84 /4;
    const uint32_t	Stat_offset      = 0x88 /4;
    const uint32_t	Peek_offset      = 0x8c /4;

    const uint32_t	Fifo_offset      = 0xA0 /4;
    const uint32_t	FifoH_offset     = 0xB0 /4;
//#!! use enum?

  public:
    rgUniSpi(			// constructor
	uint32_t	spinum,		// SPI number {1,2}
	rgAddrMap	*xx
    );

    inline volatile uint32_t*	get_base_addr()  { return  GpioBase; }
    inline uint32_t		get_spi_num()    { return  SpiNum; }

		// Access Enable and IRQ bits

    uint32_t		read_Spi_IRQ_1();
    uint32_t		read_Spi_Enable_1();
    void		write_Spi_Enable_1( uint32_t  bit1 );


		// Address of registers
    volatile uint32_t*	addr_AuxIrq();
    volatile uint32_t*	addr_AuxEnable();

    volatile uint32_t*	addr_Cntl0();
    volatile uint32_t*	addr_Cntl1();
    volatile uint32_t*	addr_Stat();
    volatile uint32_t*	addr_Peek();
    volatile uint32_t*	addr_Fifo();
    volatile uint32_t*	addr_FifoH();

		// Direct register access

    inline uint32_t	read_AuxIrq()     { return *(addr_AuxIrq());    }
    inline uint32_t	read_AuxEnable()  { return *(addr_AuxEnable()); }

    inline uint32_t	read_Cntl0()      { return *(addr_Cntl0());     }
    inline uint32_t	read_Cntl1()      { return *(addr_Cntl1());     }
    inline uint32_t	read_Stat()       { return *(addr_Stat());      }
    inline uint32_t	read_Peek()       { return *(addr_Peek());      }
    inline uint32_t	read_Fifo()       { return *(addr_Fifo());      }
    inline uint32_t	read_FifoH()      { return *(addr_FifoH());     }

    inline void		write_AuxIrq(    uint32_t reg ) {
		       *(addr_AuxIrq())    = reg;
    }
    inline void		write_AuxEnable( uint32_t reg ) {
		       *(addr_AuxEnable()) = reg;
    }

    inline void		write_Cntl0(     uint32_t reg ) {
		       *(addr_Cntl0())     = reg;
    }
    inline void		write_Cntl1(     uint32_t reg ) {
		       *(addr_Cntl1())     = reg;
    }
    inline void		write_Stat(      uint32_t reg ) {
		       *(addr_Stat())      = reg;
    }
    inline void		write_Fifo(      uint32_t reg ) {
		       *(addr_Fifo())      = reg;
    }
    inline void		write_FifoH(     uint32_t reg ) {
		       *(addr_FifoH())     = reg;
    }

		// Direct control:  (modify register fields)
    void		clear_fifos();

		// Object state operations
    void		grab_regs();
    void		push_regs();

		// Generic Field accessors:  (private)
    uint32_t		get_field(
	const uint32_t		reg,
	const uint32_t		pos,
	const uint32_t		mask
    );

    void		put_field(
	uint32_t*		rp,
	const uint32_t		pos,
	const uint32_t		mask,
	const uint32_t		val
    );

		// Object Cntl0 Field Accessors

    uint32_t		get_Speed_12();
    void		put_Speed_12(        uint32_t  v );

    uint32_t		get_ChipSelects_3();
    void		put_ChipSelects_3(   uint32_t  v );

    uint32_t		get_PostInMode_1();
    void		put_PostInMode_1(    uint32_t  v );

    uint32_t		get_VariableCS_1();
    void		put_VariableCS_1(    uint32_t  v );

    uint32_t		get_VariableWidth_1();
    void		put_VariableWidth_1( uint32_t  v );

    uint32_t		get_DoutHoldTime_2();
    void		put_DoutHoldTime_2(  uint32_t  v );

    uint32_t		get_EnableSerial_1();
    void		put_EnableSerial_1(  uint32_t  v );

    uint32_t		get_InRising_1();
    void		put_InRising_1(      uint32_t  v );

    uint32_t		get_ClearFifos_1();
    void		put_ClearFifos_1(    uint32_t  v );

    uint32_t		get_OutRising_1();
    void		put_OutRising_1(     uint32_t  v );

    uint32_t		get_InvertClk_1();
    void		put_InvertClk_1(     uint32_t  v );

    uint32_t		get_OutMsbFirst_1();
    void		put_OutMsbFirst_1(   uint32_t  v );

    uint32_t		get_ShiftLength_6();
    void		put_ShiftLength_6(   uint32_t  v );

		// Object Cntl1 Field Accessors

    uint32_t		get_CsHighTime_3();
    void		put_CsHighTime_3(    uint32_t  v );

    uint32_t		get_TxEmptyIRQ_1();
    void		put_TxEmptyIRQ_1(    uint32_t  v );

    uint32_t		get_DoneIRQ_1();
    void		put_DoneIRQ_1(       uint32_t  v );

    uint32_t		get_InMsbFirst_1();
    void		put_InMsbFirst_1(    uint32_t  v );

    uint32_t		get_KeepInput_1();
    void		put_KeepInput_1(     uint32_t  v );

		// Object Status Field Accessors (Read-only?)

    uint32_t		get_TxLevel_3();
    uint32_t		get_RxLevel_3();
    uint32_t		get_TxFull_1();
    uint32_t		get_TxEmpty_1();
    uint32_t		get_RxFull_1();
    uint32_t		get_RxEmpty_1();
    uint32_t		get_Busy_1();
    uint32_t		get_BitCount_6();

		// Test/Debug accessors

    void		show_debug( std::ostream&  sout );

    inline uint32_t	get_Cntl0() { return Cntl0Reg; }
    inline uint32_t	get_Cntl1() { return Cntl1Reg; }
    inline uint32_t	get_Stat()  { return StatReg;  }

    inline void		put_Cntl0( uint32_t reg ) { Cntl0Reg = reg; }
    inline void		put_Cntl1( uint32_t reg ) { Cntl1Reg = reg; }
    inline void		put_Stat(  uint32_t reg ) { StatReg  = reg; }

    inline uint32_t	get_bcm_address() { return FeatureAddr; }

};


#endif

