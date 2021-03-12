// 2021-03-08  William A. Hudson

#ifndef rgPads_P
#define rgPads_P

#include "rgRegister.h"

//--------------------------------------------------------------------------
// rGPIO  rgPads - GPIO Pads Control class.
//--------------------------------------------------------------------------

class rgPads_Cntl : public rgRegister {
  public:

    uint32_t	get_Passwd_8()            { return  get_field( 24, 0xff    ); }
    void	put_Passwd_8(        uint32_t v ) { put_field( 24, 0xff, v ); }

    uint32_t	get_SlewMax_1()           { return  get_field(  4, 0x1    ); }
    void	put_SlewMax_1(       uint32_t v ) { put_field(  4, 0x1, v ); }

    uint32_t	get_HystEn_1()            { return  get_field(  3, 0x1    ); }
    void	put_HystEn_1(        uint32_t v ) { put_field(  3, 0x1, v ); }

    uint32_t	get_DriveStr_3()          { return  get_field(  0, 0x7    ); }
    void	put_DriveStr_3(      uint32_t v ) { put_field(  0, 0x7, v ); }

    void	put_reset()		{ put( 0x0000001f ); }
};


class rgPads {
  private:
    volatile uint32_t		*GpioBase;	// IO base address
    static const uint32_t	FeatureAddr = 0x7e100000;  // BCM doc value

  public:
				// Registers
    rgPads_Cntl		PadsA_27s0;	// Gpio[27:0],  28 bits
    rgPads_Cntl		PadsB_45s28;	// Gpio[45:28], 18 bits
    rgPads_Cntl		PadsC_53s46;	// Gpio[53:46],  8 bits remainder

    rgPads(			// constructor
	rgAddrMap	*xx
    );

    inline
    volatile uint32_t*	get_base_addr()   { return  GpioBase; }
    inline uint32_t	get_bcm_address() { return  FeatureAddr; }
};

#endif

