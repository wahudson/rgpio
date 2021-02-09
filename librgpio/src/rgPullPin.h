// 2021-01-28  William A. Hudson

#ifndef rgPullPin_P
#define rgPullPin_P

#include "rgIoPins.h"		// class rgReg_rw

//--------------------------------------------------------------------------
// rgPullPin IO Pin Pull Up/Down for RPi4
//--------------------------------------------------------------------------

class rgPullPin {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

    static const uint32_t	FeatureAddr  = 0x7e200000;	// BCM doc

  public:
    // Pull direction identifiers.  (Different than rgPudPin)
    enum  rgPull_enum {
	pd_Off     = 0,
	pd_Up      = 1,
	pd_Down    = 2,
	pd_Unknown = 3
    };

    class rgPullPin_reg : public rgReg_rw {
      public:
	void		modify_field( unsigned field,  rgPull_enum dir );
	void		modify_mask(  uint32_t mask,   rgPull_enum dir );

	rgPull_enum	read_field(   unsigned field );
	uint32_t	read_mask( rgPull_enum dir   );
    };

  public:			// registers
    rgPullPin_reg	PullSel0;
    rgPullPin_reg	PullSel1;
    rgPullPin_reg	PullSel2;
    rgPullPin_reg	PullSel3;

  public:
    rgPullPin(			// constructor
	rgAddrMap	*xx
    );

    rgPull_enum		read_Pull_bit(   unsigned bit );
    void		modify_Pull_bit( unsigned bit,  rgPull_enum dir );

    uint32_t		read_Pull_w0( rgPull_enum dir );
    uint32_t		read_Pull_w1( rgPull_enum dir );

    void		modify_Pull_w0( uint32_t mask,  rgPull_enum dir );
    void		modify_Pull_w1( uint32_t mask,  rgPull_enum dir );

				// enum conversion
    static const char*	pull_enum2cstr( rgPull_enum dir );
    static rgPull_enum	int2pull_enum( int num );

    inline			// Test/Debug accessors
    volatile uint32_t*	get_base_addr()		{ return  GpioBase; }
    inline uint32_t	get_bcm_address()	{ return  FeatureAddr; }
};

#endif

