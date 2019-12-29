// 2019-12-10  William A. Hudson

#ifndef rgClk_P
#define rgClk_P

#include "rgClkMan.h"		// base class

//--------------------------------------------------------------------------
// rGPIO  rgClk - Clock Manager enumerated class
//--------------------------------------------------------------------------

class rgClk : public rgClkMan {
  public:
    // Clock manager identifiers.
    //   Clk0, Clk1, Clk2 are guaranteed to be integers {0, 1, 2} respectively.
    //   Values match class lookup tables, should remain constant.
    //   Extension should add new enums on end and adjust cm_MaxEnum.
    //   Intended for use as a packed array index.
    //
    enum rgClk_enum {
	cm_Clk0 = 0,
	cm_Clk1,
	cm_Clk2,
	cm_ClkPcm,
	cm_ClkPwm,
	cm_MaxEnum = cm_ClkPwm		// Maximum enum value
    };

  private:
    volatile uint32_t	*GpioBase;	// IO base address

    rgClk_enum		ClkEnum;	// Clock Number

    static
    const uint32_t	FeatureAddr  = 0x7e101000;	// BCM doc value

  public:
    rgClk(			// constructor
	rgClk_enum	clknum,		// clock identifier
	rgAddrMap	*xx
    );

    inline rgClk_enum	get_clk_enum()	  { return  ClkEnum; }

    const uint32_t	get_Cntl_offset( rgClk_enum clknum );

    const char*		cstr_clk_enum( rgClk_enum clknum );

    rgClk_enum		int2clk_enum( int num );

    inline
    volatile uint32_t*	get_base_addr()   { return  GpioBase; }

		// Test/Debug accessors

    inline uint32_t	get_bcm_address() { return FeatureAddr; }

};

#endif

