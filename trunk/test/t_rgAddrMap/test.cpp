// 2017-06-01  William A. Hudson
//
// Testing:  rgAddrMap  rGPIO Address Map class for Raspberry Pi.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library
#include "rgAddrMap.h"

using namespace std;
 
//--------------------------------------------------------------------------
 
int main()
{

  CASE( "10", "constructor" );
    try {
	rgAddrMap		bx;
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= NULL  Dev_fd= -1  FakeMem= 0",
	    ss.c_str()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "is_fake_mem()" );
    try {
	rgAddrMap		bx;
	CHECK( 0,
	    bx.is_fake_mem()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## bcm2rpi_addr()  address conversion
//--------------------------------------------------------------------------

  CASE( "18a", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	uint32_t		radd = bx.bcm2rpi_addr( 0x7e200000 );
	CHECK( 0x3f200000, radd );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "18b", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	uint32_t		radd = bx.bcm2rpi_addr( 0x7e000000 );
	CHECK( 0x3f000000, radd );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "18c", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	uint32_t		radd = bx.bcm2rpi_addr( 0x7effffff );
	CHECK( 0x3fffffff, radd );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "18d", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	uint32_t		radd = bx.bcm2rpi_addr( 0x7f000000 );
	CHECK( 0x3f000000, radd );
    }
    catch ( range_error& e ) {
        CHECK( "rgAddrMap:: address range check:  0x7f000000\n"
	       "    not in 'BCM2835 ARM Peripherals' IO space",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "18e", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	uint32_t		radd = bx.bcm2rpi_addr( 0x7dffffff );
	CHECK( 0x3f000000, radd );
    }
    catch ( range_error& e ) {
        CHECK( "rgAddrMap:: address range check:  0x7dffffff\n"
	       "    not in 'BCM2835 ARM Peripherals' IO space",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## use_fake_mem()
//--------------------------------------------------------------------------

  CASE( "21", "use_fake_mem()" );
    try {
	rgAddrMap		bx;
	bx.use_fake_mem();
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1",
	    ss.c_str()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "use_fake_mem() already" );
    try {
	rgAddrMap		bx;
	bx.use_fake_mem();
	bx.use_fake_mem();
	std::string		ss;
	ss = bx.text_debug();
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
        CHECK( "use_fake_mem() mode already specified",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "is_fake_mem()" );
    try {
	rgAddrMap		bx;
	bx.use_fake_mem();
	CHECK( 1,
	    bx.is_fake_mem()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## use_dev_*mem()  Fallback to fake_mem.  Non-RPi
//--------------------------------------------------------------------------

  CASE( "24", "use_dev_gpiomem()" );
    try {
	rgAddrMap		bx;
	bx.use_dev_gpiomem();
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1",
	    ss.c_str()
	);
    }
    catch ( runtime_error& e ) {
        CHECK( "use_dev_gpiomem() cannot open /dev/gpiomem:  No such file or directory",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25", "use_dev_mem()" );
    try {
	rgAddrMap		bx;
	bx.use_dev_mem();
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1",
	    ss.c_str()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## get_mem_block() Fake memory
//--------------------------------------------------------------------------

  CASE( "30", "get_mem_block() no mode" );
    try {
	rgAddrMap		bx;
	bx.get_mem_block( 0x7e200000 );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
        CHECK( "get_mem_block() device not open",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "get_mem_block() good" );
    try {
	rgAddrMap		bx;
	bx.use_fake_mem();
	bx.get_mem_block( 0x7e200000 );
	PASS( "ok" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "get_mem_block() page alignment" );
    try {
	rgAddrMap		bx;
	bx.use_fake_mem();
	bx.get_mem_block( 0x7f200004 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
        CHECK( "get_mem_block() address not aligned:  0x7f200004",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "get_mem_block() address range" );
    try {
	rgAddrMap		bx;
	bx.use_fake_mem();
	bx.get_mem_block( 0x7f000000 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
        CHECK( "rgAddrMap:: address range check:  0x7f000000\n"
	       "    not in 'BCM2835 ARM Peripherals' IO space",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "get_mem_block() read write fake" );
    try {
	rgAddrMap		bx;
	volatile uint32_t*	vadd;
	int			vv;
	bx.use_fake_mem();
	vadd = bx.get_mem_block( 0x7e200000 );
	vv   = *vadd;
	CHECK( 0, vv );
	*vadd = 42;
	vv   = *vadd;
	CHECK( 42, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "99", "Done" );
}

