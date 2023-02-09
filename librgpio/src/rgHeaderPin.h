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
				// index by header pin number
    static const int	GpioBit[   MaxPin+1 ];
    static const char*	PowerName[ MaxPin+1 ];

  public:
    rgHeaderPin();			// constructor

    static const int	pin2gpio( int pin );
//  const int		gpio2pin( int gpio );

    static const char*	pin2power_name( int pin );
//  const char*		pin2name( int pin );
};

#endif

