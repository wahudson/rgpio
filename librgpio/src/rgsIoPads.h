// 2024-04-11  William A. Hudson

#ifndef rgsIoPads_P
#define rgsIoPads_P

#include "rgsRegAtom.h"

//--------------------------------------------------------------------------
// rgsIoPads - IO Pads Interface class for RPi5
//--------------------------------------------------------------------------

class rgsIo_Pad : public rgsRegAtom {
  public:

    uint32_t	get_OutDisable_1()	{ return  get_field(  7, 0x1    ); }
    void	put_OutDisable_1(  uint32_t v ) { put_field(  7, 0x1, v ); }

    uint32_t	get_InEnable_1()	{ return  get_field(  6, 0x1    ); }
    void	put_InEnable_1(    uint32_t v ) { put_field(  6, 0x1, v ); }

    uint32_t	get_DriveStr_2()	{ return  get_field(  4, 0x3    ); }
    void	put_DriveStr_2(    uint32_t v ) { put_field(  4, 0x3, v ); }

    uint32_t	get_PullUpEn_1()	{ return  get_field(  3, 0x1    ); }
    void	put_PullUpEn_1(    uint32_t v ) { put_field(  3, 0x1, v ); }

    uint32_t	get_PullDnEn_1()	{ return  get_field(  2, 0x1    ); }
    void	put_PullDnEn_1(    uint32_t v ) { put_field(  2, 0x1, v ); }

    uint32_t	get_HystEn_1()		{ return  get_field(  1, 0x1    ); }
    void	put_HystEn_1(      uint32_t v ) { put_field(  1, 0x1, v ); }

    uint32_t	get_SlewFast_1()	{ return  get_field(  0, 0x1    ); }
    void	put_SlewFast_1(    uint32_t v ) { put_field(  0, 0x1, v ); }
};

class rgsIo_Volt : public rgsRegAtom {
  public:
    uint32_t	get_LowVolt_1()		{ return  get_field(  0, 0x1    ); }
    void	put_LowVolt_1(     uint32_t v ) { put_field(  0, 0x1, v ); }
};


class rgsIoPads {
  private:
    uint32_t		BankNum;	// Bank number 0..2
    volatile uint32_t	*GpioBase;	// IO base address of Bank

    static const uint32_t	FeatureAddr = 0x400f0000;
			// Bank0, delta= 0x00004000, RP1 doc

    const int		MaxBit = 27;	// Max register array index in any bank

  private:  // register per pin
    rgsIo_Pad		PadReg[28];

  public:
    rgsIo_Pad&		IoPad( int bit );	// Register accessor

    rgsIo_Volt		IoVoltage;	// IO voltage select register

  public:
    rgsIoPads(			// constructor
	rgAddrMap	*xx,
	uint32_t	bank_num = 0	// IO Bank number 0..2
    );

    uint32_t		get_bank_num()		{ return  BankNum; }
    volatile uint32_t*	get_base_addr()		{ return  GpioBase; }
    uint32_t		get_bcm_address()	{ return  FeatureAddr; }
    uint32_t		get_MaxBit()		{ return  MaxBit; }
};

#endif

