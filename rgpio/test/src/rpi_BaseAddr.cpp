// 2021-01-25  William A. Hudson
//
// rpi_BaseAddr - Test Tool
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>

using namespace std;

#include "rgRpiRev.h"

/*
* Output RPi BaseAddr.
*    Intended as a Test tool helper.
* usage:
*    rpi_BaseAddr
* output:  hex string on stdout
*    0x00000000  = not an RPi
*    0x20000000  = RPi1, Zero
*    0x3f000000  = RPi2,3
*    0xfe000000  = RPi4
*/

int
main( int	argc,
      char	*argv[]
) {

    try {
	cout.fill('0');
	cout << "0x" <<hex <<setw(8) << rgRpiRev::Config.BaseAddr.find() <<endl;
    }
    catch ( std::exception& e ) {
	cerr << "Error:  exception caught:  " << e.what() <<endl;
	return  1;
    }
    catch (...) {
	cerr << "Error:  unexpected exception" <<endl;
	return  1;
    }

    return  0;
}

