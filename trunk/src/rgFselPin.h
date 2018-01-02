// 2017-06-10  William A. Hudson

#ifndef rgFselPin_P
#define rgFselPin_P

//--------------------------------------------------------------------------
// GPIO Pin Function Select class
//--------------------------------------------------------------------------

class rgFselPin {
  private:
    rgIoPin		*IoPinX;	// IO register object

    uint32_t		FselReg[6];	// Function Select register values

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

  public:
    rgFselPin( rgIoPin  *xx );		// constructor

  public:
		// Direct access

    rgFsel_enum		read_Fsel_bit(
			    int			bit
			);

    void		modify_Fsel_bit(
			    int			bit,
			    rgFsel_enum		mode
			);

		// Register field position

    static rgIoPin::rgIoReg_enum	fselreg_bit(
					    int		bit,
					    int		*loc
					);

		// Enum string conversion

    static const char*	str_rgFsel_enum(
			    rgFsel_enum		mode
			);

    static rgFsel_enum	find_rgFsel_enum(
			    const char		*name
			);

};

#endif

