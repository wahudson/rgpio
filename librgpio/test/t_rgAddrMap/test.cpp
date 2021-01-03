// 2017-06-01  William A. Hudson
//
// Testing:  rgAddrMap  rGPIO Address Map class for Raspberry Pi.
//    10-17  Constructor, is_fake_mem(), config_BaseAddr()
//    18-19  bcm2rpi_addr()  address conversion
//    20-29  open_dev_file()
//    30-39  open_fake_mem()
//    40-49  open_dev_gpiomem()
//    50-59  open_dev_mem()
//    60-69  get_mem_block() Fake memory
//    70-79  get_mem_block() Cache
//    80-89  Destructor - device file descriptor get_DevFD()
// Env:  TESTONRPI  set when on an RPi.
// Run tests as a normal user.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library
#include "rgAddrMap.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Constructor, is_fake_mem(), config_BaseAddr()
//--------------------------------------------------------------------------

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

//--------------------------------------
  CASE( "12a", "config_BaseAddr() default" );
    try {
	rgAddrMap		bx;
	CHECKX( 0x3f000000, bx.config_BaseAddr() );	// default
	bx.config_BaseAddr( 0xfe000000 );
	CHECKX( 0xfe000000, bx.config_BaseAddr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12b", "config_BaseAddr() null" );
    try {
	rgAddrMap		bx;
	CHECKX( 0x3f000000, bx.config_BaseAddr() );	// default
	CHECK(  0,          bx.is_fake_mem() );
	bx.config_BaseAddr( 0x00000000 );
	CHECKX( 0x00000000, bx.config_BaseAddr() );
	CHECK(  1,          bx.is_fake_mem() );
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

  CASE( "18e", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	uint32_t		radd = bx.bcm2rpi_addr( 0x7dffffff );
	CHECK( 0x3f000000, radd );
	FAIL( "no throw" );
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

//--------------------------------------
  CASE( "19a", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	bx.config_BaseAddr( 0x00000000 );
	CHECKX( 0x00000000, bx.config_BaseAddr() );
	CHECK(  1,          bx.is_fake_mem() );
	CHECKX( 0x00200000, bx.bcm2rpi_addr( 0x7e200000 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## open_dev_file()
//--------------------------------------------------------------------------

//----------------------------------------
  CASE( "20a", "open_dev_file() fake_mem" );
    try {
	rgAddrMap		bx;
	bx.open_dev_file( NULL );
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1",
	    ss.c_str()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20b", "open_dev_file() fake_mem" );
    try {
	rgAddrMap		bx;
	bx.open_dev_file( "" );
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1",
	    ss.c_str()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20c", "open_dev_file() fake_mem, throw enabled, not taken" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 0 );	// throw
	bx.open_dev_file( "" );
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1",
	    ss.c_str()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//----------------------------------------
if ( ! getenv( "TESTONRPI" ) ) {

  CASE( "21a", "open_dev_file() NoPi fallback" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 1 );
	bx.open_dev_file( "/dev/gpiomem" );
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1",
	    ss.c_str()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "open_dev_file() NoPi throw" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 0 );
	bx.open_dev_file( "/dev/gpiomem" );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgAddrMap:  not on a RaspberryPi",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

}

//----------------------------------------
if ( getenv( "TESTONRPI" ) ) {

  CASE( "22a", "open_dev_file() missing file" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 1 );
	bx.open_dev_file( "./missing" );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgAddrMap:  file not found:  ./missing",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23a", "open_dev_file() no write" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 1 );
	bx.open_dev_file( "/etc/passwd" );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgAddrMap:  cannot open /etc/passwd:  Permission denied",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

}

//----------------------------------------
// Test ((BaseAddr == 0) || FakeMem) fallback to FakeMem.
  CASE( "24", "open_dev_file() BaseAddr = 0, should not throw" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 0 );		// enable throw
	bx.config_BaseAddr( 0x00000000 );
	bx.open_dev_file( "/dev/null" );
	CHECKX( 0x00000000, bx.config_BaseAddr() );
	CHECK(  1,          bx.is_fake_mem() );
    }
    catch ( runtime_error& e ) {
	FAIL( "runtime_error exception" );
	FAIL( e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## open_fake_mem()
//--------------------------------------------------------------------------

  CASE( "31", "open_fake_mem()" );
    try {
	rgAddrMap		bx;
	bx.open_fake_mem();
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1",
	    ss.c_str()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "open_fake_mem() already" );
    try {
	rgAddrMap		bx;
	bx.open_fake_mem();
	bx.open_fake_mem();
	std::string		ss;
	ss = bx.text_debug();
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgAddrMap:  already opened",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "is_fake_mem()" );
    try {
	rgAddrMap		bx;
	bx.open_fake_mem();
	CHECK( 1,
	    bx.is_fake_mem()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## open_dev_gpiomem()
//--------------------------------------------------------------------------

if ( ! getenv( "TESTONRPI" ) ) {
  CASE( "41a", "open_dev_gpiomem()" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 0 );	// throw
	bx.open_dev_gpiomem();
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgAddrMap:  not on a RaspberryPi",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
}

if ( getenv( "TESTONRPI" ) ) {
  CASE( "41b", "open_dev_gpiomem()" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 0 );	// throw
	bx.open_dev_gpiomem();
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= /dev/gpiomem  Dev_fd= 3  FakeMem= 0",
	    ss.c_str()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
}

//--------------------------------------------------------------------------
//## open_dev_mem()
//--------------------------------------------------------------------------

if ( ! getenv( "TESTONRPI" ) ) {
  CASE( "51a", "open_dev_mem()" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 0 );	// throw
	bx.open_dev_mem();
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgAddrMap:  not on a RaspberryPi",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
}

if ( getenv( "TESTONRPI" ) ) {
  CASE( "51b", "open_dev_mem()" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 0 );	// throw
	bx.open_dev_mem();
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgAddrMap:  cannot open /dev/mem:  Permission denied",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
}

//--------------------------------------------------------------------------
//## get_mem_block() Fake memory
//--------------------------------------------------------------------------

  CASE( "60", "get_mem_block() no device open" );
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

  CASE( "61", "get_mem_block() good" );
    try {
	rgAddrMap		bx;
	bx.open_fake_mem();
	bx.get_mem_block( 0x7e200000 );
	PASS( "ok" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "62", "get_mem_block() page alignment" );
    try {
	rgAddrMap		bx;
	bx.open_fake_mem();
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

  CASE( "63", "get_mem_block() address range" );
    try {
	rgAddrMap		bx;
	bx.open_fake_mem();
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

  CASE( "64", "get_mem_block() read write fake" );
    try {
	rgAddrMap		bx;
	volatile uint32_t*	vadd;
	int			vv;
	bx.open_fake_mem();
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

//--------------------------------------------------------------------------
//## get_mem_block() Cache
//--------------------------------------------------------------------------

  CASE( "71a", "get_mem_block() same addr" );
    try {
	rgAddrMap		bx;
	volatile uint32_t*	v1;
	volatile uint32_t*	v2;
	bx.open_fake_mem();
	CHECK( 0, bx.size_BlkCache() );
	v1 = bx.get_mem_block( 0x7e200000 );
	CHECK( 1, bx.size_BlkCache() );
	v2 = bx.get_mem_block( 0x7e200000 );
	CHECK( 1, bx.size_BlkCache() );
	CHECK( 1, (v1 == v2) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71b", "get_mem_block() different addr" );
    try {
	rgAddrMap		bx;
	volatile uint32_t*	v1;
	volatile uint32_t*	v2;
	bx.open_fake_mem();
	v1 = bx.get_mem_block( 0x7e200000 );
	CHECK( 1, bx.size_BlkCache() );
	v2 = bx.get_mem_block( 0x7e210000 );
	CHECK( 2, bx.size_BlkCache() );
	CHECK( 1, (v1 == v2) );		// fake memory block
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

if ( getenv( "TESTONRPI" ) ) {

  CASE( "72a", "get_mem_block() same addr" );
    try {
	rgAddrMap		bx;
	volatile uint32_t*	v1;
	volatile uint32_t*	v2;
	bx.open_dev_gpiomem();
	CHECK( 0, bx.size_BlkCache() );
	v1 = bx.get_mem_block( 0x7e200000 );
	CHECK( 1, bx.size_BlkCache() );
	v2 = bx.get_mem_block( 0x7e200000 );
	CHECK( 1, bx.size_BlkCache() );
	CHECK( 1, (v1 == v2) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72b", "get_mem_block() different addr" );
    try {
	rgAddrMap		bx;
	volatile uint32_t*	v1;
	volatile uint32_t*	v2;
	bx.open_dev_gpiomem();
	v1 = bx.get_mem_block( 0x7e200000 );
	CHECK( 1, bx.size_BlkCache() );
	v2 = bx.get_mem_block( 0x7e210000 );
	CHECK( 2, bx.size_BlkCache() );
	CHECK( 0, (v1 == v2) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

}

//--------------------------------------------------------------------------
//## Destructor - device file descriptor get_DevFD()
//--------------------------------------------------------------------------


if ( getenv( "TESTONRPI" ) ) {

// Device file descriptor should be released on destruction and re-aquired.
  CASE( "80", "Destructor - device file descriptor" );
    try {
	{
	    rgAddrMap		bx;
	    bx.open_dev_file( "ref/dummy.mem", 0 );
	    CHECK( 3, bx.get_DevFD() );
	}
	{
	    rgAddrMap		bx;
	    bx.open_dev_file( "ref/dummy.mem", 0 );
	    CHECK( 3, bx.get_DevFD() );
	}
	{
	    rgAddrMap		bx;
	    bx.open_dev_file( "ref/dummy.mem", 0 );
	    CHECK( 3, bx.get_DevFD() );
	}
	{
	    rgAddrMap		bx;
	    bx.open_dev_file( "ref/dummy.mem", 0 );
	    CHECK( 3, bx.get_DevFD() );
	}
    }
    catch ( runtime_error& e ) {
	FAIL( "runtime_error exception" );
	FAIL( e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
}


//--------------------------------------
  CASE( "99", "Done" );
}

