// 2017-06-10  William A. Hudson

#ifndef rgIoPin_P
#define rgIoPin_P

//--------------------------------------------------------------------------
// Rpi GPIO IO Pin control class
//--------------------------------------------------------------------------

class rgIoPin {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

    uint32_t		FselReg[6];	// Function Select

//    uint32_t		PinLevel_w0;	// read only
//    uint32_t		PinLevel_w1;

  private:
    static const char*	RegStr[];	// Register string names,
					//     indexed by rgIoReg_enum.

  public:
    enum rgFsel_enum {		// Function select values, 3-bit octal
	f_In   = 00,
	f_Out  = 01,
	f_Alt0 = 04,
	f_Alt1 = 05,
	f_Alt2 = 06,
	f_Alt3 = 07,
	f_Alt4 = 03,
	f_Alt5 = 02
    };

    enum rgIoReg_enum {		// Register address offset, word address

	rgFsel0                  = 0x00 /4,	// rw GPFSEL0
	rgFsel1                  = 0x04 /4,	// rw GPFSEL1
	rgFsel2                  = 0x08 /4,	// rw GPFSEL2
	rgFsel3                  = 0x0c /4,	// rw GPFSEL3
	rgFsel4                  = 0x10 /4,	// rw GPFSEL4
	rgFsel5                  = 0x14 /4,	// rw GPFSEL5

	rgPinSet_w0              = 0x1c /4,	// -w GPSET0
	rgPinSet_w1              = 0x20 /4,	// -w GPSET1

	rgPinClr_w0              = 0x28 /4,	// -w GPCLR0
	rgPinClr_w1              = 0x2c /4,	// -w GPCLR1

	rgPinRead_w0             = 0x34 /4,	// r- GPLEV0
	rgPinRead_w1             = 0x38 /4,	// r- GPLEV1

	rgEventStatus_w0         = 0x40 /4,	// rw GPEDS0
	rgEventStatus_w1         = 0x44 /4,	// rw GPEDS1

	rgDetectRising_w0        = 0x4c /4,	// rw GPREN0
	rgDetectRising_w1        = 0x50 /4,	// rw GPREN1

	rgDetectFalling_w0       = 0x58 /4,	// rw GPFEN0
	rgDetectFalling_w1       = 0x5c /4,	// rw GPFEN1

	rgDetectHigh_w0          = 0x64 /4,	// rw GPHEN0
	rgDetectHigh_w1          = 0x68 /4,	// rw GPHEN1

	rgDetectLow_w0           = 0x70 /4,	// rw GPLEN0
	rgDetectLow_w1           = 0x74 /4,	// rw GPLEN1

	rgDetectAsyncRising_w0   = 0x7c /4,	// rw GPAREN0
	rgDetectAsyncRising_w1   = 0x80 /4,	// rw GPAREN1

	rgDetectAsyncFalling_w0  = 0x88 /4,	// rw GPAFEN0
	rgDetectAsyncFalling_w1  = 0x8c /4,	// rw GPAFEN1

	rgPullUpDown             = 0x94 /4,	// rw GPPUD
	rgPullUpDownClk_w0       = 0x98 /4,	// rw GPPUDCLK0
	rgPullUpDownClk_w1       = 0x9c /4	// rw GPPUDCLK1
    };

  public:
    rgIoPin( rgAddrMap  *xx = NULL );	// constructor

    void		init_addr( rgAddrMap  *xx );

    inline volatile uint32_t*	get_base_addr()
    {
	return  GpioBase;
    }

		// Generic registor access

    volatile uint32_t*	addr_reg(
			    rgIoReg_enum	reg
			);

    uint32_t		read_reg(
			    rgIoReg_enum	reg
			);

    void		modify_reg(
			    rgIoReg_enum	reg,
			    uint32_t		mask,
			    uint32_t		value
			);

		// PinLevel registers

    inline volatile uint32_t*	addr_PinRead_w0() {
	return         (GpioBase + rgPinRead_w0);
    }

    inline volatile uint32_t*	addr_PinSet_w0() {
	return         (GpioBase + rgPinSet_w0);
    }

    inline volatile uint32_t*	addr_PinClr_w0() {
	return         (GpioBase + rgPinClr_w0);
    }


    inline uint32_t	read_PinLevel_w0() {
	return  *(GpioBase + rgPinRead_w0);
    }

    inline void		set_PinLevel_w0( uint32_t mask ) {
	*(GpioBase + rgPinSet_w0) = mask;
    }

    inline void		clr_PinLevel_w0( uint32_t mask ) {
	*(GpioBase + rgPinClr_w0) = mask;
    }

		// Event Status register

    inline uint32_t	read_EventStatus_w0()
    {
	return  read_reg( rgEventStatus_w0 );
    }

		// Enum string conversion

    static const char*	str_IoReg_enum(
			    rgIoReg_enum        reg
			);

    static rgIoPin::rgIoReg_enum	find_IoReg_enum(
					    const char		*name
					);

};


#endif

