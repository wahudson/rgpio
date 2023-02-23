// 2017-06-10  William A. Hudson

#ifndef rgFselPin_P
#define rgFselPin_P

#include "rgIoPins.h"		// class rgReg_rw

//--------------------------------------------------------------------------
// rGPIO Pin Function Select class
//--------------------------------------------------------------------------

class rgFselPin {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

    static const uint32_t	FeatureAddr  = 0x7e200000;	// BCM doc

    static const char*	ModeStr[];	// Fsel mode string names,
					//     indexed by rgFsel_enum.
  public:
    enum rgFsel_enum {		// Function Select mode, 3-bit field value
	f_In   = 0,	// 000 binary
	f_Out  = 1,	// 001
	f_Alt0 = 4,	// 100
	f_Alt1 = 5,	// 101
	f_Alt2 = 6,	// 110
	f_Alt3 = 7,	// 111
	f_Alt4 = 3,	// 011
	f_Alt5 = 2	// 010
    };

    class rgFselPin_reg : public rgReg_rw {
      public:
	void		modify_field( unsigned field,  rgFsel_enum mode );
	void		modify_mask(  uint32_t mask,   rgFsel_enum mode );

	rgFsel_enum	read_field(   unsigned field );
	uint32_t	read_mask( rgFsel_enum mode   );
    };

  public:			// Registers
    rgFselPin_reg	Fsel0;		// Gpio[9:0]
    rgFselPin_reg	Fsel1;		// Gpio[19:10]
    rgFselPin_reg	Fsel2;		// Gpio[29:20]
    rgFselPin_reg	Fsel3;		// Gpio[39:30]
    rgFselPin_reg	Fsel4;		// Gpio[49:40]
    rgFselPin_reg	Fsel5;		// Gpio[57:50]   8 bits RPi4

  public:
    rgFselPin( rgAddrMap  *xx );	// constructor
    rgFselPin( rgIoPins  *xx );		// constructor (Deprecated)

		// Top level Read/Modify Fsel pins

    rgFsel_enum		read_Fsel_bit(   unsigned bit );
    void		modify_Fsel_bit( unsigned bit,  rgFsel_enum mode );

    uint32_t		read_Fsel_w0( rgFsel_enum mode );
    uint32_t		read_Fsel_w1( rgFsel_enum mode );

    void		modify_Fsel_w0( uint32_t mask,  rgFsel_enum mode );
    void		modify_Fsel_w1( uint32_t mask,  rgFsel_enum mode );

		// Enum string conversion

    static const char*	rgFsel_enum2cstr( rgFsel_enum mode );

    static rgFsel_enum	find_rgFsel_enum( const char *name );

    inline	// Test/Debug accessors
    volatile uint32_t*	get_base_addr()		{ return  GpioBase; }
    inline uint32_t	get_bcm_address()	{ return  FeatureAddr; }
};

#endif

