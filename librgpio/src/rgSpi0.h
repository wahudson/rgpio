// 2019-10-21  William A. Hudson

#ifndef rgSpi0_P
#define rgSpi0_P

//--------------------------------------------------------------------------
// rGPIO SPI0 Master class.  Spi0
//--------------------------------------------------------------------------

class rgSpi0 {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

				// Register data
    uint32_t		CntlStat_reg;	// CS    Control and Status
    //			Fifo_reg;	// FIFO  TX and RX FIFOs
    uint32_t		ClkDiv_reg;	// CLK   Clock Divider
    uint32_t		DmaLen_reg;	// DLEN  DMA Data Length
    uint32_t		Lossi_reg;	// LTOH  LoSSI mode Control
    uint32_t		DmaReq_reg;	// DC    DMA DREQ Controls
    // Note Fifo_reg is NOT part of object state.

  private:
    const uint32_t	FeatureAddr  = 0x7e204000;	// BCM doc value

				// Register word offset in Page
    const uint32_t	CntlStat_offset  = 0x00 /4;
    const uint32_t	Fifo_offset      = 0x04 /4;
    const uint32_t	ClkDiv_offset    = 0x08 /4;
    const uint32_t	DmaLen_offset    = 0x0c /4;
    const uint32_t	Lossi_offset     = 0x10 /4;
    const uint32_t	DmaReq_offset    = 0x14 /4;

  public:
    rgSpi0(			// constructor
	rgAddrMap	*xx
    );

    inline volatile uint32_t*	get_base_addr()  { return  GpioBase; }

		// Address of registers
    inline
    volatile uint32_t*	addr_CntlStat()	{ return (GpioBase + CntlStat_offset); }

    inline
    volatile uint32_t*	addr_Fifo()	{ return (GpioBase + Fifo_offset);   }

    inline
    volatile uint32_t*	addr_ClkDiv()	{ return (GpioBase + ClkDiv_offset); }

    inline
    volatile uint32_t*	addr_DmaLen()	{ return (GpioBase + DmaLen_offset); }

    inline
    volatile uint32_t*	addr_Lossi()	{ return (GpioBase + Lossi_offset);  }

    inline
    volatile uint32_t*	addr_DmaReq()	{ return (GpioBase + DmaReq_offset); }

		// Direct register access

    inline uint32_t	read_CntlStat()   { return *(addr_CntlStat());  }
    inline uint32_t	read_Fifo()       { return *(addr_Fifo());      }
    inline uint32_t	read_ClkDiv()     { return *(addr_ClkDiv());    }
    inline uint32_t	read_DmaLen()     { return *(addr_DmaLen());    }
    inline uint32_t	read_Lossi()      { return *(addr_Lossi());     }
    inline uint32_t	read_DmaReq()     { return *(addr_DmaReq());    }

    inline void		write_CntlStat(  uint32_t rval ) {
		       *(addr_CntlStat())  = rval;
    }
    inline void		write_Fifo(      uint32_t rval ) {
		       *(addr_Fifo())      = rval;
    }
    inline void		write_ClkDiv(    uint32_t rval ) {
		       *(addr_ClkDiv())    = rval;
    }
    inline void		write_DmaLen(    uint32_t rval ) {
		       *(addr_DmaLen())    = rval;
    }
    inline void		write_Lossi(     uint32_t rval ) {
		       *(addr_Lossi())     = rval;
    }
    inline void		write_DmaReq(    uint32_t rval ) {
		       *(addr_DmaReq())    = rval;
    }

		// Direct control:  (modify register fields)
//  void		clear_fifos();	#!! not implemented

		// Object state operations
    void		init_put_reset();

    void		grab_regs();
    void		push_regs();

    void		grab_CntlStat();

		// Generic Field accessors:  (private)
    uint32_t		get_field(
	const uint32_t		rval,
	const uint32_t		pos,
	const uint32_t		mask
    );

    void		put_field(
	uint32_t*		rp,
	const uint32_t		pos,
	const uint32_t		mask,
	const uint32_t		val
    );

		// Object CntlStat Field Accessors

    uint32_t		get_LossiWord_1();
    void		put_LossiWord_1(     uint32_t  v );

    uint32_t		get_LossiDmaEn_1();
    void		put_LossiDmaEn_1(    uint32_t  v );

    uint32_t		get_CsPolarity_3();
    void		put_CsPolarity_3(    uint32_t  v );

    uint32_t		get_RxFullStop_1();
//  void		put_RxFullStop_1(    uint32_t  v );

    uint32_t		get_RxHalf_1();
//  void		put_RxHalf_1(        uint32_t  v );

    uint32_t		get_TxHasSpace_1();
//  void		put_TxHasSpace_1(    uint32_t  v );

    uint32_t		get_RxHasData_1();
//  void		put_RxHasData_1(     uint32_t  v );

    uint32_t		get_TxEmpty_1();
//  void		put_TxEmpty_1(       uint32_t  v );

    uint32_t		get_LossiEnable_1();
    void		put_LossiEnable_1(   uint32_t  v );

    uint32_t		get_ReadEnable_1();
    void		put_ReadEnable_1(    uint32_t  v );

    uint32_t		get_DmaEndCs_1();
    void		put_DmaEndCs_1(      uint32_t  v );

    uint32_t		get_IrqRxHalf_1();
    void		put_IrqRxHalf_1(     uint32_t  v );

    uint32_t		get_IrqTxEmpty_1();
    void		put_IrqTxEmpty_1(    uint32_t  v );

    uint32_t		get_DmaEnable_1();
    void		put_DmaEnable_1(     uint32_t  v );

    uint32_t		get_RunActive_1();
    void		put_RunActive_1(     uint32_t  v );

    uint32_t		get_CsPolarity_1();
    void		put_CsPolarity_1(    uint32_t  v );

    uint32_t		get_ClearRxTxFifo_2();
    void		put_ClearRxTxFifo_2( uint32_t  v );

    uint32_t		get_ClockPolarity_1();
    void		put_ClockPolarity_1( uint32_t  v );

    uint32_t		get_ClockPhase_1();
    void		put_ClockPhase_1(    uint32_t  v );

    uint32_t		get_ChipSelectN_2();
    void		put_ChipSelectN_2(   uint32_t  v );

		// Object ClkDiv Field Accessors

    uint32_t		get_ClockDiv_16();
    void		put_ClockDiv_16(     uint32_t  v );

		// Object DmaLen Field Accessors

    uint32_t		get_DmaDataLen_16();
    void		put_DmaDataLen_16(   uint32_t  v );

		// Object Lossi Field Accessors

    uint32_t		get_LossiHoldDly_4();
    void		put_LossiHoldDly_4(  uint32_t  v );

		// Object DmaReq Field Accessors

    uint32_t		get_DmaRxPanicLev_8();
    void		put_DmaRxPanicLev_8( uint32_t  v );

    uint32_t		get_DmaRxReqLev_8();
    void		put_DmaRxReqLev_8(   uint32_t  v );

    uint32_t		get_DmaTxPanicLev_8();
    void		put_DmaTxPanicLev_8( uint32_t  v );

    uint32_t		get_DmaTxReqLev_8();
    void		put_DmaTxReqLev_8(   uint32_t  v );

		// Test/Debug accessors

    void		show_debug( std::ostream&  sout );

    inline uint32_t	get_CntlStat()         { return CntlStat_reg; }
    inline void		put_CntlStat( uint32_t rval ) { CntlStat_reg = rval; }

    inline uint32_t	get_ClkDiv()           { return ClkDiv_reg; }
    inline void		put_ClkDiv(   uint32_t rval ) { ClkDiv_reg   = rval; }

    inline uint32_t	get_DmaLen()           { return DmaLen_reg; }
    inline void		put_DmaLen(   uint32_t rval ) { DmaLen_reg   = rval; }

    inline uint32_t	get_Lossi()            { return Lossi_reg; }
    inline void		put_Lossi(    uint32_t rval ) { Lossi_reg    = rval; }

    inline uint32_t	get_DmaReq()           { return DmaReq_reg; }
    inline void		put_DmaReq(   uint32_t rval ) { DmaReq_reg   = rval; }


    inline uint32_t	get_bcm_address() { return FeatureAddr; }

};


#endif

