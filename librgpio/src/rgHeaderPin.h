// 2023-02-05  William A. Hudson

#ifndef rgHeaderPin_P
#define rgHeaderPin_P

//--------------------------------------------------------------------------
// rGPIO Header Pin Names (40-pin header only)
//--------------------------------------------------------------------------

class rgHeaderPin {

  public:
    static const int	MaxPin  = 40;	// max header pin number
    static const int	MaxGpio = 27;	// max gpio bit number on header

  private:
    struct pinRec {
	const int	Gnum;		// gpio bit number, otherwise encode
					//     -1= GND, -2= +5.0V, -3= +3.3V
	const char	Name[8];	// pin name string
    };

    static pinRec	PinMap[ MaxPin+1 ];	// index by header pin number

  public:
    rgHeaderPin();			// constructor

    static const int	pin2gpio_int( int pin );
    static const int	gpio2pin_int( int gpio );

    static const char*	pin2name_cstr( int pin );
};

#endif

