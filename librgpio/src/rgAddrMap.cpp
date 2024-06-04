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

#include "rgRpiRev.h"
#include "rgAddrMap.h"


#define PAGE_SIZE	(4*1024)	// in bytes


/*
* Convert documentation address to physical RPi address.
*    i.e. "BCM2835 ARM Peripherals" datasheet.
*    Range check for added safety.
*    Uses object BaseAddr, expected to be correct for the running RPi.
*/
uint64_t
rgAddrMap::bcm2rpi_addr(
    uint32_t		bcm_addr	// "BCM2835 ARM Peripherals" address
)
{
    // Check address is in the peripherals IO space.
    if ( (bcm_addr <   DocBase) ||
	 (bcm_addr >= (DocBase + 0x01000000)) )
    {
	std::ostringstream	css;
	css << "rgAddrMap:: address range check:  0x"
	    <<hex << bcm_addr <<endl
	    << "    not in ARM Peripherals IO space 0x" << DocBase;
	throw std::domain_error ( css.str() );
    }

    if ( (BaseAddr == 0) && (! FakeMem) ) {	// using a null address
	std::ostringstream	css;
	css << "rgAddrMap::BaseAddr is null and not FakeMem";
	throw std::runtime_error ( css.str() );
    }

    return  (bcm_addr - DocBase + BaseAddr);
}


/*
* Fake memory block - Class Data.
*    Size:  Was one page (4096 byte) on RPi4 and earlier;  Now is 4 pages
*        (4 * 4096 byte) on RPi5 to accommodate "Atomic register access".
*    Alignment:  Is word aligned, NOT page aligned.
*    Address pointers are byte addresses, word aligned.
*    A static array is guaranteed to start off zero, providing reproducible
*    results.
*/
volatile uint32_t    rgAddrMap::FakeBlock[MaxBlock_w];


/*
* Constructor.
*    Construction is initialization.
*    Intend that rgRpiRev is accessed only in the constructor.
* exceptions:
*    std::domain_error
*    std::logic_error
*/
rgAddrMap::rgAddrMap()
{
    rgRpiRev::Soc_enum		soc;

    Dev_fd  = -1;		// not open
    FakeNoPi = 1;		// 1= fake mem, 0= throw error, when not on RPi
    ModeStr = NULL;
    Debug   = 0;
    DocBase   = 0x7e000000;	// RPi4 or earlier
    BlockSize = PAGE_SIZE;

    BaseAddr  =   rgRpiRev::Global.BaseAddr.find();
    FakeMem   = ! rgRpiRev::Global.RevCode.find_realpi();
    soc       =   rgRpiRev::Global.SocEnum.find();

    if      ( soc == rgRpiRev::soc_BCM2712 ) {		// RPi5
	DocBase   = 0x40000000;
	BlockSize = 4 * PAGE_SIZE;
    }
    else if ( soc >  rgRpiRev::soc_BCM2712 ) {
	throw std::domain_error (
	    "rgAddrMap:  require RPi5 (soc_BCM2712) or earlier"
	);
    }

    if ( BlockSize > (4 * MaxBlock_w) ) {	// should never happen
	throw std::logic_error (
	    "rgAddrMap:  internal bad BlockSize > (4*MaxBlock_w)"
	);
    }
}


/*
* Destructor.
*    File descriptors are a global process resource, and must be released
*    when the object is destroyed.  They are acquired in open_dev_file().
* Address Maps from mmap(2) are not released:  #!!
*    This allows addresses extracted from Feature register classes to remain
*    valid, but is a potential resource leak.
*    It is intended that one rgAddrMap object be created and last the life of
*    the process.
*/
rgAddrMap::~rgAddrMap()
{
    if ( Dev_fd >= 0 ) {
	close( Dev_fd );
    }
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
rgAddrMap::config_BaseAddr( uint64_t  addr )
{
    FakeMem  = ( 0 == addr );
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

    // explicit fake memory
    if ( (file == NULL) || (*file == '\0') || FakeMem ) {
	ModeStr = "fake_mem";
	FakeMem = 1;
	return;
    }

    // Check if on RPi, make safe on other machines.
    if ( (BaseAddr == 0) || ! (
	     (stat( "/dev/gpiomem",  &statbuf ) == 0) ||	// exist
	     (stat( "/dev/gpiomem0", &statbuf ) == 0)		// RPi5
	 )
    ) {
	if ( FakeNoPi ) {
	    this->open_dev_file( NULL );
	    return;
	}
	else {
	    throw std::domain_error ( "rgAddrMap:  not on a RaspberryPi" );
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
*    Note the object destructor will also close the file descriptor.
*/
void
rgAddrMap::close_dev()
{
    if ( Dev_fd >= 0 ) {

	if ( -1 == close( Dev_fd ) ) {
	    int		errv = errno;
	    std::string	ss ( "rgAddrMap::close_dev() failed:  " );
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
*        e.g. 0x7e200000 is GPIO pins on RPi4
* return:
*    ()  = virtual address of IO memory block, word aligned.
*    Size is 1024 words for RPi4 and earlier;  4096 words RPi5.
*    Only word alignment is guaranteed.
*
*    The underlying real IO memory block is block aligned, but the resulting
*    mapped virtual address might be page aligned at best.  See mmap(2).
*    Fake memory is definitely only word aligned.
*    Thus offsets should be computed by addition, not bit masking.
* exceptions:
*    std::range_error
*    std::runtime_error
*/
volatile uint32_t*
rgAddrMap::get_mem_block(
    uint32_t		bcm_addr
)
{
    uint64_t		r_addr;		// RPi real addr
    void*		mem_block;

    // Check block alignment.
    if ( (bcm_addr & (BlockSize - 1)) != 0 ) {
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
	return  FakeBlock;
    }

    // Check Device file still open.
    if ( Dev_fd == -1 ) {
	throw std::runtime_error ( "get_mem_block() device not open" );
    }

    // map GPIO into our memory
    mem_block = mmap(
	NULL,			// Any address in our space will do
	BlockSize,		// Map length
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


/*
* Get peripheral memory block address - word aligned.
*    Calls get_mem_block() and returns the address within that block.
*    The given address need be only word aligned.  For use with new RPi4
*    register groups.
* Note:  Register pointers (uint32_t*) are byte addresses, word aligned.
*    Word pointers add word offset, not byte offset.
*    Fake memory is word aligned, NOT block (or page) aligned.
* call:
*    get_mem_addr( bcm_addr )
*    bcm_addr = peripheral address as in BCM datasheet, word aligned.
*        e.g. 0x7e205a80 is iic3 register group
* return:
*    ()  = virtual address in IO memory block, word aligned.
*/
volatile uint32_t*
rgAddrMap::get_mem_addr(
    uint32_t		bcm_addr
)
{
    uint32_t		offset;		// word offset within block
    volatile uint32_t*	addr;		// return address

    // Check word alignment.
    if ( (bcm_addr & 0x0003) != 0 ) {
	std::ostringstream	css;
	css << "get_mem_addr() address not word aligned:  0x"
	    <<hex << bcm_addr;
	throw std::range_error ( css.str() );
    }

    uint32_t		mask = BlockSize - 1;	// e.g. 0x00000fff

    offset =            ( bcm_addr &    mask  ) >> 2;
    addr = get_mem_block( bcm_addr & (~ mask) );

    addr += offset;	// word pointer calculation

    return  addr;
}

