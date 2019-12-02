// 2019-11-28  William A. Hudson

#ifndef rgPwm_P
#define rgPwm_P

#include "rgRegister.h"

//--------------------------------------------------------------------------
// rGPIO PWM - Pulse Width Modulator class
//--------------------------------------------------------------------------

class rgPwm_Cntl : public rgRegister {
  public:

    inline
    uint32_t	get_Ch2_MsEnable_1()     { return  get_field( 15, 0x1    ); };
    void	put_Ch2_MsEnable_1( uint32_t v ) { put_field( 15, 0x1, v ); };

    inline
    uint32_t	get_Ch2_UseFifo_1()      { return  get_field( 13, 0x1    ); };
    void	put_Ch2_UseFifo_1( uint32_t v )  { put_field( 13, 0x1, v ); };

    inline
    uint32_t	get_Ch2_Invert_1()       { return  get_field( 12, 0x1    ); };
    void	put_Ch2_Invert_1( uint32_t v )   { put_field( 12, 0x1, v ); };

    inline
    uint32_t	get_Ch2_IdleBit_1()      { return  get_field( 11, 0x1    ); };
    void	put_Ch2_IdleBit_1( uint32_t v )  { put_field( 11, 0x1, v ); };

    inline
    uint32_t	get_Ch2_Repeat_1()       { return  get_field( 10, 0x1    ); };
    void	put_Ch2_Repeat_1( uint32_t v )   { put_field( 10, 0x1, v ); };

    inline
    uint32_t	get_Ch2_SerMode_1()      { return  get_field(  9, 0x1    ); };
    void	put_Ch2_SerMode_1( uint32_t v )  { put_field(  9, 0x1, v ); };

    inline
    uint32_t	get_Ch2_Enable_1()       { return  get_field(  8, 0x1    ); };
    void	put_Ch2_Enable_1( uint32_t v )   { put_field(  8, 0x1, v ); };


    inline
    uint32_t	get_ClearFifo_1()        { return  get_field(  6, 0x1    ); };
    void	put_ClearFifo_1( uint32_t v )    { put_field(  6, 0x1, v ); };


    inline
    uint32_t	get_Ch1_MsEnable_1()     { return  get_field(  7, 0x1    ); };
    void	put_Ch1_MsEnable_1( uint32_t v ) { put_field(  7, 0x1, v ); };

    inline
    uint32_t	get_Ch1_UseFifo_1()      { return  get_field(  5, 0x1    ); };
    void	put_Ch1_UseFifo_1( uint32_t v )  { put_field(  5, 0x1, v ); };

    inline
    uint32_t	get_Ch1_Invert_1()       { return  get_field(  4, 0x1    ); };
    void	put_Ch1_Invert_1( uint32_t v )   { put_field(  4, 0x1, v ); };

    inline
    uint32_t	get_Ch1_IdleBit_1()      { return  get_field(  3, 0x1    ); };
    void	put_Ch1_IdleBit_1( uint32_t v )  { put_field(  3, 0x1, v ); };

    inline
    uint32_t	get_Ch1_Repeat_1()       { return  get_field(  2, 0x1    ); };
    void	put_Ch1_Repeat_1( uint32_t v )   { put_field(  2, 0x1, v ); };

    inline
    uint32_t	get_Ch1_SerMode_1()      { return  get_field(  1, 0x1    ); };
    void	put_Ch1_SerMode_1( uint32_t v )  { put_field(  1, 0x1, v ); };

    inline
    uint32_t	get_Ch1_Enable_1()       { return  get_field(  0, 0x1    ); };
    void	put_Ch1_Enable_1( uint32_t v )   { put_field(  0, 0x1, v ); };
};

class rgPwm_Stat : public rgRegister {
  public:

    inline
    uint32_t	get_Ch2_Active_1()       { return  get_field( 10, 0x1    ); };
    void	put_Ch2_Active_1( uint32_t v )   { put_field( 10, 0x1, v ); };

    inline
    uint32_t	get_Ch1_Active_1()       { return  get_field(  9, 0x1    ); };
    void	put_Ch1_Active_1( uint32_t v )   { put_field(  9, 0x1, v ); };

    inline
    uint32_t	get_BusError_1()         { return  get_field(  8, 0x1    ); };
    void	put_BusError_1( uint32_t v )     { put_field(  8, 0x1, v ); };

    inline
    uint32_t	get_Ch2_GapErr_1()       { return  get_field(  5, 0x1    ); };
    void	put_Ch2_GapErr_1( uint32_t v )   { put_field(  5, 0x1, v ); };

    inline
    uint32_t	get_Ch1_GapErr_1()       { return  get_field(  4, 0x1    ); };
    void	put_Ch1_GapErr_1( uint32_t v )   { put_field(  4, 0x1, v ); };

    inline
    uint32_t	get_FifoReadErr_1()      { return  get_field(  3, 0x1    ); };
    void	put_FifoReadErr_1( uint32_t v )  { put_field(  3, 0x1, v ); };

    inline
    uint32_t	get_FifoWriteErr_1()     { return  get_field(  2, 0x1    ); };
    void	put_FifoWriteErr_1( uint32_t v ) { put_field(  2, 0x1, v ); };

    inline
    uint32_t	get_FifoEmpty_1()        { return  get_field(  1, 0x1    ); };
    void	put_FifoEmpty_1( uint32_t v )    { put_field(  1, 0x1, v ); };

    inline
    uint32_t	get_FifoFull_1()         { return  get_field(  0, 0x1    ); };
    void	put_FifoFull_1( uint32_t v )     { put_field(  0, 0x1, v ); };
};

class rgPwm_DmaConf : public rgRegister {
  public:

    inline
    uint32_t	get_DmaEnable_1()        { return  get_field( 31, 0x1    ); };
    void	put_DmaEnable_1( uint32_t v )    { put_field( 31, 0x1, v ); };

    inline
    uint32_t	get_DmaPanicLev_8()     { return  get_field(  8, 0xff    ); };
    void	put_DmaPanicLev_8( uint32_t v ) { put_field(  8, 0xff, v ); };

    inline
    uint32_t	get_DmaReqLev_8()       { return  get_field(  0, 0xff    ); };
    void	put_DmaReqLev_8( uint32_t v )   { put_field(  0, 0xff, v ); };
};

class rgPwm_Fifo : public rgRegister {
};

class rgPwm_Range : public rgRegister {
};

class rgPwm_Data  : public rgRegister {
};
//#!! Should Ch1 and Ch2 share classes?  Object copy of address??


class rgPwm {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

  public:
				// Register data
    rgPwm_Cntl		Cntl;		// CTL  Control
    rgPwm_Stat		Stat;		// STA	Status
    rgPwm_DmaConf	DmaConf;	// DMAC	DMA Configuration
    rgPwm_Fifo		Fifo;		// FIF1	Fifo input (WO)
    rgPwm_Range		Ch1Range;	// RNG1 cycle length M
    rgPwm_Data		Ch1Data;	// DAT1 data or mark length N
    rgPwm_Range		Ch2Range;	// RNG2 cycle length M
    rgPwm_Data		Ch2Data;	// DAT2 data or mark length N

  private:
    static const uint32_t	FeatureAddr  = 0x7e20c000;	// BCM doc value

					// Register word offset in Page
    static const uint32_t	Cntl_offset     = 0x00 /4;
    static const uint32_t	Stat_offset     = 0x04 /4;
    static const uint32_t	DmaConf_offset  = 0x08 /4;
    static const uint32_t	Ch1Range_offset = 0x10 /4;
    static const uint32_t	Ch1Data_offset  = 0x14 /4;
    static const uint32_t	Fifo_offset     = 0x18 /4;
    static const uint32_t	Ch2Range_offset = 0x20 /4;
    static const uint32_t	Ch2Data_offset  = 0x24 /4;

  public:
    rgPwm(			// constructor
	rgAddrMap	*xx
    );

    inline
    volatile uint32_t*	get_base_addr()  { return  GpioBase; }

		// Object state operations
    void		init_put_reset();

    void		grab_regs();
    void		push_regs();

		// Test/Debug accessors

    void		show_debug( std::ostream&  sout );

    inline uint32_t	get_bcm_address() { return FeatureAddr; }

};

#endif

