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

  public:	// private, for test only
    rgReg_rw		FselReg[6];	// Function Select registers

  private:
    static const char*	ModeStr[];	// Fsel mode string names,
					//     indexed by rgFsel_enum.
  public:
    enum rgFsel_enum {		// Function Select mode, 3-bit octal
	f_In   = 00,
	f_Out  = 01,
	f_Alt0 = 04,
	f_Alt1 = 05,
	f_Alt2 = 06,
	f_Alt3 = 07,
	f_Alt4 = 03,
	f_Alt5 = 02
    };
    // Values are the BCM Function Select register mode values.

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

    rgFsel_enum		read_Fsel_bitO(   unsigned bit );
    void		modify_Fsel_bitO( unsigned bit,  rgFsel_enum mode );

    rgFsel_enum		read_Fsel_bit(   unsigned bit );
    void		modify_Fsel_bit( unsigned bit,  rgFsel_enum mode );

    uint32_t		read_Fsel_w0( rgFsel_enum mode );
    uint32_t		read_Fsel_w1( rgFsel_enum mode );

    void		modify_Fsel_w0( uint32_t mask,  rgFsel_enum mode );
    void		modify_Fsel_w1( uint32_t mask,  rgFsel_enum mode );

		// Register field position

    rgReg_rw*		fselreg_bit(
			    int		bit,
			    int		*pos
			);

		// Enum string conversion

    static const char*	str_rgFsel_enum(
			    rgFsel_enum		mode
			);

    static rgFsel_enum	find_rgFsel_enum(
			    const char		*name
			);

    inline	// Test/Debug accessors
    volatile uint32_t*	get_base_addr()		{ return  GpioBase; }
    inline uint32_t	get_bcm_address()	{ return  FeatureAddr; }
};

#endif

