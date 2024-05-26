// 2024-05-24  William A. Hudson

#ifndef rgsRio_P
#define rgsRio_P

#include "rgsRegAtom.h"
#include "rgsIoBank.h"

//--------------------------------------------------------------------------
// rgsRio - Register Input/Output (RIO) class for RPi5
//--------------------------------------------------------------------------

class rgsRio : public rgsIoBank {
  private:
//  uint32_t		BankNum;	// Bank number {0,1,2}
//  volatile uint32_t	*GpioBase;	// IO base address of Bank

    static const uint32_t  FeatureAddr = 0x400e0000;
			// Bank0, delta= 0x00004000, RP1 doc

  public:	// registers
    rgsRegAtom		RioOut;		// output level
    rgsRegAtom		RioOutEn;	// output enable
    rgsRegAtom		RioIn;		// input direct from pad
    rgsRegAtom		RioInSync;	// input synchronized to clk_sys

  public:
    rgsRio(			// constructor
	rgAddrMap	*xx,
	uint32_t	bank_num = 0	// IO Bank number {0,1,2}
    );

    uint32_t		get_bcm_address()	{ return  FeatureAddr; }

	// base class
//  uint32_t		get_bank_num()		{ return  BankNum; }
//  volatile uint32_t*	get_base_addr()		{ return  GpioBase; }
//  uint32_t		get_doc_address();
//  uint32_t		get_doc_offset();
};

#endif

