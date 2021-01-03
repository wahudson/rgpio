// 2017-05-17  William A. Hudson

// rGPIO Address Map class for Raspberry Pi.
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>
#include <map>

#include <unistd.h>
#include <fcntl.h>	// open()
#include <sys/mman.h>	// mmap()
#include <sys/stat.h>	// stat()
#include <sys/capability.h>	// cap_*()

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
*    Uses object BaseAddr, expected to be correct for the running RPi.
*/
uint32_t
rgAddrMap::bcm2rpi_addr(
    uint32_t		bcm_addr	// "BCM2835 ARM Peripherals" address
)
{
    // Check address is in BCM2835 ARM Peripherals IO space.
    if ( (bcm_addr <   BCM2835_IO_PERI) ||
	 (bcm_addr >= (BCM2835_IO_PERI + 0x01000000)) )
    {
	std::ostringstream	css;
	css << "rgAddrMap:: address range check:  0x"
	    <<hex << bcm_addr <<endl
	    << "    not in 'BCM2835 ARM Peripherals' IO space" ;
	throw std::range_error ( css.str() );
    }

    if ( (BaseAddr == 0) && (! FakeMem) ) {	// using a null address
	std::ostringstream	css;
	css << "rgAddrMap::BaseAddr is null and not FakeMem";
	throw std::runtime_error ( css.str() );
    }

    return  (bcm_addr - BCM2835_IO_PERI + BaseAddr);
}


/*
* Constructor.
*    Bare uninitialized object.
*/
rgAddrMap::rgAddrMap()
{
    Dev_fd  = -1;		// not open
    FakeMem = 0;		// using real memory
    FakeNoPi = 1;		// 1= fake mem, 0= throw error, when not on RPi
    ModeStr = NULL;
//  Prot    = PROT_READ | PROT_WRITE;
    Debug   = 0;
    BaseAddr  = BCM2708_PERI_BASE;
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
* Configure real IO base address.
*    The value set should correspond to the RPi version being run.
*    A null value, meaning not on a RPi, will fall-back to fake memory.
*    Default BaseAddr is defined in the constructor.
*    Intended to be called only once, before any memory is mapped.
* call:
*    config_BaseAddr( 0x20000000 )
*/
void
rgAddrMap::config_BaseAddr( uint32_t  addr )
{
    if ( ! addr ) {
	FakeMem = 1;
    }
    BaseAddr = addr;
}


/*
* Configure fall-back action when not on a RPi.
*    Applies only when open_dev_file() is opening a real file.
* call:
*    config_FakeNoPi( 1 )	fake memory
*    config_FakeNoPi( 0 )	throw error
*/
void
rgAddrMap::config_FakeNoPi( bool v )
{
    FakeNoPi = v;
}


/*
* Open device file for IO memory map.
*    Generic device file access, user wrappers can hide device file name.
*    May require CAP_DAC_OVERRIDE capability (or root) for access.
*    Safety check if not on RPi - use fake memory if FakeNoPi is true, else
*    throw error.
*    #!! Possibly make this simple safety check more robust.
* capabilities:
*    Require capabilities set on the compiled executable file for access
*    to /dev/mem  e.g.
*        sudo setcap 'CAP_DAC_OVERRIDE,CAP_SYS_RAWIO=p'  ../bin/rgpio
*    The drop_cap flag indicates that capabilities required for /dev/mem
*    access should be raised before opening file, and dropped afterward.
*    This avoids the need for setuid or running as root.
*    The flag defaults to 1, and is provided to allow disabling the raise/drop
*    of capabilities in case the user needs to manage it himself.
* call:
*    open_dev_file( "/dev/mem" )	full access, need root
*    open_dev_file( "/dev/gpiomem" )	only GPIO pins, normal user
*    open_dev_file( "" )		use fake memory block
*    User applications should use the corresponding wrapper functions.
* call:
*    open_dev_file( file, drop_cap )
*    file     = file name to open
*    drop_cap = flag, 1= raise/drop capabilities, 0= not
*/
void
rgAddrMap::open_dev_file(
    const char*		file,
    bool		drop_cap	// default = 1
)
{
    struct stat			statbuf;
    cap_t			capx;		// capability set
    int				rv;		// return values

    if ( ModeStr != NULL ) {
	throw std::runtime_error ( "rgAddrMap:  already opened" );
    }

    if ( (file == NULL) || (*file == '\0') || (BaseAddr == 0) || FakeMem ) {
	ModeStr = "fake_mem";
	FakeMem = 1;
	return;
    }

    // Check if on RPi, make safe on other machines.
    if ( stat( "/dev/gpiomem", &statbuf ) != 0 ) {	// not exist
	if ( FakeNoPi ) {
	    this->open_dev_file( NULL );
	    return;
	}
	else {
	    throw std::runtime_error ( "rgAddrMap:  not on a RaspberryPi" );
	}
    }

    if ( stat( file, &statbuf ) != 0 ) {	// not exist
	std::string	ss ( "rgAddrMap:  file not found:  " );
	ss += file;
	throw std::runtime_error ( ss );
    }

    if ( drop_cap ) {		// raise needed capabilities
	capx = cap_from_text( "CAP_DAC_OVERRIDE,CAP_SYS_RAWIO=pe" );
	rv   = cap_set_proc( capx );
	cap_free( capx );
	if ( Debug ) {
	    if ( rv != 0 ) {
		cerr << "rgAddrMap:  raise cap failed" <<endl;
		// let open() fail if it is a problem
	    }
	    capx = cap_get_proc();
	    cerr << "rgAddrMap:  raise cap:  "
		 << cap_to_text( capx, NULL ) <<endl;
	    cap_free( capx );	//#!! and text
	}
    }

    ModeStr = file;

    Dev_fd = open( ModeStr, O_RDWR|O_SYNC );
    if ( Dev_fd < 0) {
	int		errv = errno;
	std::string	ss ( "rgAddrMap:  cannot open " );
	ss += ModeStr;
	ss += ":  ";
	ss += strerror( errv );
	throw std::runtime_error ( ss );
    }

    if ( drop_cap ) {		// drop all capabilities
	capx = cap_init();
	rv   = cap_set_proc( capx );
	cap_free( capx );
	if ( rv != 0 ) {
	    std::string	ss ( "rgAddrMap:  failed to drop capabilities\n" );
	    throw std::runtime_error ( ss );
	}
	if ( Debug ) {
	    capx = cap_get_proc();
	    cerr << "rgAddrMap:  drop  cap:  "
		 << cap_to_text( capx, NULL ) <<endl;
	    cap_free( capx );	//#!! and text
	}
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
*    get_mem_block( bcm_addr )
*    bcm_addr = peripheral address as in BCM datasheet, block aligned.
*		e.g. 0x7e200000 is GPIO pins
*/
volatile uint32_t*
rgAddrMap::get_mem_block(
    uint32_t		bcm_addr
)
{
    uint32_t		r_addr;		// RPi real addr
    void*		mem_block;

    // Check page alignment.
    if ( (bcm_addr & 0x0fff) != 0 ) {
	std::ostringstream	css;
	css << "get_mem_block() address not aligned:  0x"
	    <<hex << bcm_addr;
	throw std::range_error ( css.str() );
    }

    // Convert BCM document address to RPi address.
    r_addr = bcm2rpi_addr( bcm_addr );

    // Check cache to see if it is previously mapped.
    void*&		cache_ref = BlkCache[r_addr];

    if ( cache_ref != NULL ) {
	return  (volatile uint32_t*)cache_ref;
    }

    if ( FakeMem ) {
	cache_ref = (void*)FakeBlock;
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

    // Insert memory block into cache.
    cache_ref = mem_block;

//    cerr << "    mem_block= " << (uint32_t *)mem_block << endl;

    return (volatile uint32_t*)mem_block;
}

