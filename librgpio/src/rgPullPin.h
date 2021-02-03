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

  public:			// array of pointers to registers
    rgReg_rw*		PullSel[4];

  public:			// registers
    rgReg_rw		PullSel0;
    rgReg_rw		PullSel1;
    rgReg_rw		PullSel2;
    rgReg_rw		PullSel3;

  public:
    rgPullPin(			// constructor
	rgAddrMap	*xx
    );

    rgPull_enum		read_Pull_bit(
			    int			bit
			);

    void		modify_Pull_bit(
			    int			bit,
			    rgPull_enum		dir
			);

    rgReg_rw*		pullreg_bit(
			    int			bit,
			    int			*pos
			);

				// enum conversion
    static const char*	pull_enum2cstr( rgPull_enum dir );
    static rgPull_enum	int2pull_enum( int num );

    inline			// Test/Debug accessors
    volatile uint32_t*	get_base_addr()		{ return  GpioBase; }
    inline uint32_t	get_bcm_address()	{ return  FeatureAddr; }
};

#endif

