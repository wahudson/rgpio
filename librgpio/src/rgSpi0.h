// 2019-10-21  William A. Hudson

#ifndef rgSpi0_P
#define rgSpi0_P

#include "rgRegister.h"

//--------------------------------------------------------------------------
// rGPIO SPI0 Master class.  Spi0
//--------------------------------------------------------------------------

class rgSpi0_CntlStat : public rgRegister {
  public:

    inline
    uint32_t	get_LossiWord_1()         { return  get_field( 25, 0x1    ); }
    void	put_LossiWord_1(     uint32_t v ) { put_field( 25, 0x1, v ); }

    uint32_t	get_LossiDmaEn_1()        { return  get_field( 24, 0x1    ); }
    void	put_LossiDmaEn_1(    uint32_t v ) { put_field( 24, 0x1, v ); }

    uint32_t	get_CsPolarity_3()        { return  get_field( 21, 0x7    ); }
    void	put_CsPolarity_3(    uint32_t v ) { put_field( 21, 0x7, v ); }

    uint32_t	get_RxFullStop_1()        { return  get_field( 20, 0x1    ); }
    void	put_RxFullStop_1(    uint32_t v ) { put_field( 20, 0x1, v ); }

    uint32_t	get_RxHalf_1()            { return  get_field( 19, 0x1    ); }
    void	put_RxHalf_1(        uint32_t v ) { put_field( 19, 0x1, v ); }

    uint32_t	get_TxHasSpace_1()        { return  get_field( 18, 0x1    ); }
    void	put_TxHasSpace_1(    uint32_t v ) { put_field( 18, 0x1, v ); }

    uint32_t	get_RxHasData_1()         { return  get_field( 17, 0x1    ); }
    void	put_RxHasData_1(     uint32_t v ) { put_field( 17, 0x1, v ); }

    uint32_t	get_TxEmpty_1()           { return  get_field( 16, 0x1    ); }
    void	put_TxEmpty_1(       uint32_t v ) { put_field( 16, 0x1, v ); }

    uint32_t	get_LossiEnable_1()       { return  get_field( 13, 0x1    ); }
    void	put_LossiEnable_1(   uint32_t v ) { put_field( 13, 0x1, v ); }

    uint32_t	get_ReadEnable_1()        { return  get_field( 12, 0x1    ); }
    void	put_ReadEnable_1(    uint32_t v ) { put_field( 12, 0x1, v ); }

    uint32_t	get_DmaEndCs_1()          { return  get_field( 11, 0x1    ); }
    void	put_DmaEndCs_1(      uint32_t v ) { put_field( 11, 0x1, v ); }

    uint32_t	get_IrqRxHalf_1()         { return  get_field( 10, 0x1    ); }
    void	put_IrqRxHalf_1(     uint32_t v ) { put_field( 10, 0x1, v ); }

    uint32_t	get_IrqTxEmpty_1()        { return  get_field(  9, 0x1    ); }
    void	put_IrqTxEmpty_1(    uint32_t v ) { put_field(  9, 0x1, v ); }

    uint32_t	get_DmaEnable_1()         { return  get_field(  8, 0x1    ); }
    void	put_DmaEnable_1(     uint32_t v ) { put_field(  8, 0x1, v ); }

    uint32_t	get_RunActive_1()         { return  get_field(  7, 0x1    ); }
    void	put_RunActive_1(     uint32_t v ) { put_field(  7, 0x1, v ); }

    uint32_t	get_CsPolarity_1()        { return  get_field(  6, 0x1    ); }
    void	put_CsPolarity_1(    uint32_t v ) { put_field(  6, 0x1, v ); }

    uint32_t	get_ClearRxTxFifo_2()     { return  get_field(  4, 0x3    ); }
    void	put_ClearRxTxFifo_2( uint32_t v ) { put_field(  4, 0x3, v ); }

    uint32_t	get_ClockPolarity_1()     { return  get_field(  3, 0x1    ); }
    void	put_ClockPolarity_1( uint32_t v ) { put_field(  3, 0x1, v ); }

    uint32_t	get_ClockPhase_1()        { return  get_field(  2, 0x1    ); }
    void	put_ClockPhase_1(    uint32_t v ) { put_field(  2, 0x1, v ); }

    uint32_t	get_ChipSelectN_2()       { return  get_field(  0, 0x3    ); }
    void	put_ChipSelectN_2(   uint32_t v ) { put_field(  0, 0x3, v ); }
};

class rgSpi0_Fifo   : public rgRegister {
};

class rgSpi0_ClkDiv : public rgRegister {
  public:

    uint32_t	get_ClockDiv_16()         { return  get_field(  0, 0xffff ); }
    void	put_ClockDiv_16(     uint32_t v ) { put_field(  0, 0xffff, v); }
};

class rgSpi0_DmaLen : public rgRegister {
  public:

    uint32_t	get_DmaDataLen_16()       { return  get_field(  0, 0xffff ); }
    void	put_DmaDataLen_16(   uint32_t v ) { put_field(  0, 0xffff, v); }
};

class rgSpi0_Lossi  : public rgRegister {
  public:

    uint32_t	get_LossiHoldDly_4()      { return  get_field(  0, 0xf    ); }
    void	put_LossiHoldDly_4(  uint32_t v ) { put_field(  0, 0xf, v ); }
};

class rgSpi0_DmaReq : public rgRegister {
  public:

    uint32_t	get_DmaRxPanicLev_8()     { return  get_field( 24, 0xff   ); }
    void	put_DmaRxPanicLev_8( uint32_t v ) { put_field( 24, 0xff, v); }

    uint32_t	get_DmaRxReqLev_8()       { return  get_field( 16, 0xff   ); }
    void	put_DmaRxReqLev_8(   uint32_t v ) { put_field( 16, 0xff, v); }

    uint32_t	get_DmaTxPanicLev_8()     { return  get_field(  8, 0xff   ); }
    void	put_DmaTxPanicLev_8( uint32_t v ) { put_field(  8, 0xff, v); }

    uint32_t	get_DmaTxReqLev_8()       { return  get_field(  0, 0xff   ); }
    void	put_DmaTxReqLev_8(   uint32_t v ) { put_field(  0, 0xff, v); }
};


class rgSpi0 {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

  public:
				// Register data
    rgSpi0_CntlStat	CntlStat;	// CS    Control and Status
    rgSpi0_Fifo		Fifo;		// FIFO  TX and RX FIFOs
    rgSpi0_ClkDiv	ClkDiv;		// CLK   Clock Divider
    rgSpi0_DmaLen	DmaLen;		// DLEN  DMA Data Length
    rgSpi0_Lossi	Lossi;		// LTOH  LoSSI mode Control
    rgSpi0_DmaReq	DmaReq;		// DC    DMA DREQ Controls

  private:
    static const uint32_t	FeatureAddr  = 0x7e204000;	// BCM doc value

					// Register word offset in Page
    static const uint32_t	CntlStat_offset  = 0x00 /4;
    static const uint32_t	Fifo_offset      = 0x04 /4;
    static const uint32_t	ClkDiv_offset    = 0x08 /4;
    static const uint32_t	DmaLen_offset    = 0x0c /4;
    static const uint32_t	Lossi_offset     = 0x10 /4;
    static const uint32_t	DmaReq_offset    = 0x14 /4;

  public:
    rgSpi0(			// constructor
	rgAddrMap	*xx
    );

    inline volatile uint32_t*	get_base_addr()  { return  GpioBase; }

		// Direct control:  (modify register fields)
//  void		clear_fifos();	#!! not implemented

		// Object state operations
    void		init_put_reset();

    void		grab_regs();
    void		push_regs();

		// Test/Debug accessors

    void		show_debug( std::ostream&  sout );

    inline uint32_t	get_bcm_address() { return FeatureAddr; }

};

#endif

