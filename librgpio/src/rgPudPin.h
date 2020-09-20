// 2020-09-05  William A. Hudson

#ifndef rgPudPin_P
#define rgPudPin_P

#include "rgRegister.h"
#include "rgIoPins.h"		// class rgReg_rw

//--------------------------------------------------------------------------
// rgPudPin IO Pin Pull Up/Down
//--------------------------------------------------------------------------

class rgPudPin_Mode : public rgRegister {
  public:
    inline
    uint32_t	get_UpDown_2()           { return  get_field(  0, 0x3    ); }
    void	put_UpDown_2( uint32_t v )       { put_field(  0, 0x3, v ); }
};


class rgPudPin_Clk : public rgReg_rw {
};


class rgPudPin {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

    static const uint32_t	FeatureAddr  = 0x7e200000;	// BCM doc

  public:
    // Pull direction identifiers.
    // Enums pd_{Off, Down, Up} are guaranteed to be integers {0, 1, 2}
    // respectively, and should remain constant.
    enum  rgPud_enum {
	pd_Off  = 0,
	pd_Down = 1,
	pd_Up   = 2
    };

  public:			// registers
    rgPudPin_Mode	PudProgMode;
    rgPudPin_Clk	PudProgClk_w0;
    rgPudPin_Clk	PudProgClk_w1;

  public:
    rgPudPin(			// constructor
	rgAddrMap	*xx
    );

  private:
    uint32_t		program_pud(
			    rgPudPin_Clk	*clkreg,
			    rgPud_enum		dir,
			    uint32_t		mask
			);

  public:
				// programming pull resistors
    uint32_t		program_pud_bit(
			    rgPud_enum		dir,
			    uint32_t		num
			);

    uint32_t		program_pud_w0( rgPud_enum dir, uint32_t mask ) {
			    return  program_pud( &PudProgClk_w0, dir, mask );
			};

    uint32_t		program_pud_w1( rgPud_enum dir, uint32_t mask ) {
			    return  program_pud( &PudProgClk_w1, dir, mask );
			};

				// enum conversion
    static const char*	pud_enum2cstr( rgPud_enum dir );
    static rgPud_enum	int2pud_enum( int num );

    inline			// Test/Debug accessors
    volatile uint32_t*	get_base_addr()		{ return  GpioBase; }
    inline uint32_t	get_bcm_address()	{ return  FeatureAddr; }
};

#endif

