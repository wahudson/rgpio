// 2017-05-17  William A. Hudson

// rGPIO Address Map class for Raspberry Pi.
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

#include <unistd.h>
#include <fcntl.h>	// open()
#include <sys/mman.h>	// mmap()
#include <sys/stat.h>	// stat()

#include <string.h>	// strerror()
#include <errno.h>	// errno

using namespace std;

#include "rgAddrMap.h"


#define BLOCK_SIZE	(4*1024)

		// IO address space conversion:
#define BCM2835_IO_PERI		0x7e000000	// BCM2835 ARM Peripherals
#define BCM2708_PERI_BASE	0x3f000000	// Rpi3, (0x20000000 Rpi1)


/*
* Convert address in "BCM2835 ARM Peripherals" datasheet to RPi.
*    Range check for added safety.
*    Currently only for RPi3.
*    Review code when #defines change.
*/
uint32_t
rgAddrMap::bcm2rpi_addr(
    uint32_t		addr	// "BCM2835 ARM Peripherals" address
)
{
    // Check address is in BCM2835 ARM Peripherals IO space.
    if ( (addr <   BCM2835_IO_PERI) ||
	 (addr >= (BCM2835_IO_PERI + 0x01000000)) )
    {
	std::ostringstream	css;
	css << "rgAddrMap:: address range check:  0x"
	    <<hex << addr <<endl
	    << "    not in 'BCM2835 ARM Peripherals' IO space" ;
    	throw std::range_error ( css.str() );
    }

    return  (addr - BCM2835_IO_PERI + BCM2708_PERI_BASE);
}


/*
* Constructor.
*    Bare uninitialized object.
*/
rgAddrMap::rgAddrMap()
{
    Dev_fd  = -1;		// not open
    FakeMem = 0;		// using real memory
    ModeStr = NULL;
    Prot    = PROT_READ | PROT_WRITE;
    Debug   = 0;
}


/*
*#!! not used
* Constructor.
*    Check if on RPi and select appropriate kernel memory device.
*    Open memory device.
*    Lower capabilities so remainder of process is normal user privelege.
*
*    Access thru /dev/gpiomem, for normal users belonging to group 'gpio'.
*    If it did not exist, use a fake memory region for testing.
* call:
*    rgAddrMap		object with full heuristic initialization
*    rgAddrMap  (0)	object with no initialization
*/
rgAddrMap::rgAddrMap( int	init )
{
    Dev_fd  = -1;		// not open
    ModeStr = NULL;
    Prot    = PROT_READ | PROT_WRITE;
    Debug   = 0;

    if ( init == 0 ) {
	return;
    }

    if ( init != 1 ) {
    	throw std::invalid_argument ( "rgAddrMap() bad init arg" );
    }

  // Heuristic to select memory device mode:
//    if ( ! is_this_RPi() ) {
//	ModeStr = "fake_mem";
//    }
//    if ( stat( devfile, &statbuf ) == 0 ) {	// exists
//	ModeStr = "/dev/gpiomem";
//    }

}


/*
* Get debug text.
*    No trailing new-line.
*/
std::string
rgAddrMap::text_debug()
{
    std::ostringstream		css;		// constructor
    const char			*mode = "NULL";

    if ( ModeStr != NULL ) { mode = ModeStr; }

    css << "ModeStr= "    << mode
        << "  Dev_fd= "   << Dev_fd
        << "  FakeMem= "  << FakeMem;

    return css.str();
}


/*
* Init to use fake memory.
*/
void
rgAddrMap::use_fake_mem()
{
    if ( ModeStr != NULL ) {
    	throw std::runtime_error ( "use_fake_mem() mode already specified" );
    }

    ModeStr = "fake_mem";
    FakeMem = 1;
}


/*
* Init to use /dev/gpiomem
*    Allow normal user access, but limited to only GPIO pins.
*/
void
rgAddrMap::use_dev_gpiomem()
{
    struct stat			statbuf;

    if ( ModeStr != NULL ) {
    	throw std::runtime_error ( "use_dev_gpiomem() mode already specified" );
    }

    if ( stat( "/dev/gpiomem", &statbuf ) != 0 ) {	// not exist
    	cerr << "rgAddrMap:  Using fake memory" <<endl;
	this->use_fake_mem();
	return;
    }

    ModeStr = "/dev/gpiomem";

    if (( Dev_fd = open( ModeStr, O_RDWR|O_SYNC ) ) < 0) {
	int		errv = errno;
	std::string	ss ( "use_dev_gpiomem() cannot open /dev/gpiomem:  " );
	ss += strerror( errv );
    	throw std::runtime_error ( ss );
    }
}


/*
* Init to use /dev/mem
*    Require CAP_DAC_OVERRIDE capability (or root) for access.
* CAUTION:  Be sure running on a Raspberry Pi!
*    Writing random places in /dev/mem may trash the system.
*    Simple test for /dev/gpiomem existance may not always be good enough.
*/
void
rgAddrMap::use_dev_mem()
{
    struct stat			statbuf;

    if ( ModeStr != NULL ) {
    	throw std::runtime_error ( "use_dev_mem() mode already specified" );
    }

    if ( stat( "/dev/gpiomem", &statbuf ) != 0 ) {	// not exist
    	cerr << "rgAddrMap:  Using fake memory" <<endl;
	this->use_fake_mem();
	return;
    }

    ModeStr = "/dev/mem";

    if (( Dev_fd = open( ModeStr, O_RDWR|O_SYNC ) ) < 0) {
	int		errv = errno;
	std::string	ss ( "use_dev_mem() cannot open /dev/mem:  " );
	ss += strerror( errv );
    	throw std::runtime_error ( ss );
    }
}


/*
* Close the device file descriptor.
*    Also checks for error.
*/
void
rgAddrMap::close_dev()
{
    if ( Dev_fd >= 0 ) {

	if ( -1 == close( Dev_fd ) ) {
	    int		errv = errno;
	    std::string	ss ( "close_dev() failed:  " );
	    ss += strerror( errv );
	    throw std::runtime_error ( ss );
	}
	Dev_fd = -1;
    }
}


/*
* Get peripheral memory block.
* call:
*    get_mem_block( p_addr )
*    p_addr = peripheral address, as in BCM datasheet,  block aligned.
*		e.g. 0x7e200000 is GPIO pins
*/
volatile uint32_t*
rgAddrMap::get_mem_block(
    int			p_addr
)
{
    int			r_addr;		// RPi real addr
    void		*mem_block;

    // Check page alignment.
    if ( (p_addr & 0x0fff) != 0 ) {
	std::ostringstream	css;
	css << "get_mem_block() address not aligned:  0x"
	    <<hex << p_addr;
    	throw std::range_error ( css.str() );
    }

    r_addr = bcm2rpi_addr( p_addr );

    if ( FakeMem ) {
	return (volatile uint32_t*)FakeBlock;
    }

    // Check Device file still open.
    if ( Dev_fd == -1 ) {
    	throw std::runtime_error ( "get_mem_block() device not open" );
    }
 
    // map GPIO into our memory
    mem_block = mmap(
	NULL,			// Any adddress in our space will do
	BLOCK_SIZE,		// Map length
	PROT_READ|PROT_WRITE,	// Enable reading & writing to mapped memory
//	PROT_READ,
	MAP_SHARED,		// Shared with other processes
	Dev_fd,			// File descriptor to map
	r_addr			// Offset to GPIO peripheral, page aligned
    );

    if ( mem_block == MAP_FAILED ) {
	int		errv = errno;
	std::string	ss ( "get_mem_block() MAP_FAILED:  " );
	ss += strerror( errv );
    	throw std::runtime_error ( ss );
    }

//#!! cache mem_block

//    cerr << "    mem_block= " << (uint32_t *)mem_block << endl;

    return (volatile uint32_t*)mem_block;
}

