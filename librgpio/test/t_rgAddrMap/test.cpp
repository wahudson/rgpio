// 2017-06-01  William A. Hudson
//
// Testing:  rgAddrMap  rGPIO Address Map class for Raspberry Pi.
//    10-14  Constructor, is_fake_mem(), config_BaseAddr()
//    15-19  bcm2rpi_addr()  address conversion
//    20-29  open_dev_file() - fake memory
//    30-39  open_fake_mem(), open_dev_gpiomem(), open_dev_mem()
//    40-49  get_mem_block() - fake memory
//    50-59  get_mem_addr() - fake memory
//    ## TESTONRPI - below are not run unless on RPi
//    60-69  Constructor - real memory
//    70-79  open_dev_file() - real memory
//    80-89  get_mem_block() - real memory, cache
//    90-99  Destructor - device file descriptor get_DevFD()
// Run tests as a normal user.  Should pass on any RPi or non-RPi.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library
#include "rgRpiRev.h"
#include "rgAddrMap.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Setup
//--------------------------------------------------------------------------

uint32_t	TEST_BaseAddr = rgRpiRev::find_BaseAddr();

rgRpiRev::Config.BaseAddr.put( 0x00000000 );	// Config as not on RPi

//--------------------------------------------------------------------------
//## Constructor, is_fake_mem(), config_BaseAddr()
//--------------------------------------------------------------------------

  CASE( "10", "constructor" );
    try {
	rgAddrMap		bx;
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= NULL  Dev_fd= -1  FakeMem= 0", ss.c_str() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "is_fake_mem()" );
    try {
	rgAddrMap		bx;
	CHECK( 0, bx.is_fake_mem() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "12a", "config_BaseAddr() non-zero" );
    try {
	rgAddrMap		bx;
	CHECKX( 0x00000000, bx.config_BaseAddr() );	// default
	CHECK(  0,          bx.is_fake_mem() );
	bx.config_BaseAddr( 0x77000000 );
	CHECKX( 0x77000000, bx.config_BaseAddr() );
	CHECK(  0,          bx.is_fake_mem() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12b", "config_BaseAddr() null" );
    try {
	rgAddrMap		bx;
	CHECKX( 0x00000000, bx.config_BaseAddr() );	// default
	CHECK(  0,          bx.is_fake_mem() );
	bx.config_BaseAddr( 0x00000000 );
	CHECKX( 0x00000000, bx.config_BaseAddr() );
	CHECK(  1,          bx.is_fake_mem() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## bcm2rpi_addr() address conversion
//--------------------------------------------------------------------------

  CASE( "15a", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	bx.config_BaseAddr( 0x3f000000 );
	uint32_t		radd = bx.bcm2rpi_addr( 0x7e200000 );
	CHECKX( 0x3f200000, radd );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15b", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	bx.config_BaseAddr( 0x3f000000 );
	uint32_t		radd = bx.bcm2rpi_addr( 0x7e000000 );
	CHECKX( 0x3f000000, radd );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15c", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	bx.config_BaseAddr( 0x3f000000 );
	uint32_t		radd = bx.bcm2rpi_addr( 0x7effffff );
	CHECKX( 0x3fffffff, radd );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15d", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	bx.config_BaseAddr( 0x3f000000 );
	bx.bcm2rpi_addr(    0x7f000000 );
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

  CASE( "15e", "bcm2rpi_addr()" );
    try {
	rgAddrMap		bx;
	bx.config_BaseAddr( 0x3f000000 );
	bx.bcm2rpi_addr(    0x7dfff000 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgAddrMap:: address range check:  0x7dfff000\n"
	       "    not in 'BCM2835 ARM Peripherals' IO space",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "18a", "bcm2rpi_addr()" );
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

  CASE( "18b", "bcm2rpi_addr() as not on RPi" );
    try {
	rgAddrMap		bx;
	CHECKX( 0x00200000, bx.bcm2rpi_addr( 0x7e200000 ) );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgAddrMap::BaseAddr is null and not FakeMem",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## open_dev_file() - fake memory
//--------------------------------------------------------------------------

//----------------------------------------
  CASE( "20a", "open_dev_file() fake_mem" );
    try {
	rgAddrMap		bx;
	bx.open_dev_file( NULL );
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1", ss.c_str() );
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
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1", ss.c_str() );
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
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1", ss.c_str() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//----------------------------------------
  CASE( "21a", "open_dev_file() NoPi fallback" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 1 );
	bx.open_dev_file( "/dev/gpiomem" );
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1", ss.c_str() );
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

//----------------------------------------
// Test fallback to FakeMem, NOT on RPi.
  CASE( "24a", "open_dev_file() FakeMem=1, should not throw" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 0 );		// enable throw
	bx.config_BaseAddr( 0x00000000 );
	CHECK(  1,          bx.is_fake_mem() );
	CHECKX( 0x00000000, bx.config_BaseAddr() );
	bx.open_dev_file( "/dev/null" );
	CHECK(  1,          bx.is_fake_mem() );
	CHECKX( 0x00000000, bx.config_BaseAddr() );
    }
    catch ( runtime_error& e ) {
	FAIL( "runtime_error exception" );
	FAIL( e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "open_dev_file() FakeMem=0, should throw" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 0 );		// enable throw
	CHECK(  0,          bx.is_fake_mem() );
	CHECKX( 0x00000000, bx.config_BaseAddr() );
	bx.open_dev_file( "/dev/null" );
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

//--------------------------------------------------------------------------
//## open_fake_mem(), open_dev_gpiomem(), open_dev_mem()
//--------------------------------------------------------------------------

  CASE( "31", "open_fake_mem() NoPi, no throw" );
    try {
	rgAddrMap		bx;
	bx.open_fake_mem();
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= fake_mem  Dev_fd= -1  FakeMem= 1", ss.c_str() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "open_fake_mem() already" );
    try {
	rgAddrMap		bx;
	bx.open_fake_mem();
	bx.open_fake_mem();
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
	CHECK(  1,          bx.is_fake_mem() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//----------------------------------------
  CASE( "34", "open_dev_gpiomem() NoPi, throw" );
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

//----------------------------------------
  CASE( "37", "open_dev_mem() NoPi, throw" );
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

//--------------------------------------------------------------------------
//## get_mem_block() - fake memory
//--------------------------------------------------------------------------

  CASE( "40a", "get_mem_block() no device open" );
    try {
	rgAddrMap		bx;
	bx.config_BaseAddr( 0x3f000000 );
	bx.get_mem_block(   0x7e200000 );
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

  CASE( "40b", "get_mem_block() no device open, BaseAddr=0" );
    try {
	rgAddrMap		bx;
	bx.get_mem_block(   0x7e200000 );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgAddrMap::BaseAddr is null and not FakeMem",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41", "get_mem_block() good" );
    try {
	rgAddrMap		bx;
	bx.open_fake_mem();
	bx.get_mem_block( 0x7e200000 );
	PASS( "ok" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "get_mem_block() page alignment" );
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

  CASE( "43", "get_mem_block() address range" );
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

  CASE( "44", "get_mem_block() read write fake" );
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

//----------------------------------------
  CASE( "46a", "get_mem_block() cache - same addr" );
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

  CASE( "46b", "get_mem_block() cache - different addr" );
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

//--------------------------------------------------------------------------
//## get_mem_addr() - fake memory
//--------------------------------------------------------------------------

  CASE( "51", "get_mem_addr() good word offset" );
    try {
	rgAddrMap		bx;
	volatile uint32_t*	v1;
	volatile uint32_t*	v2;
	bx.open_fake_mem();
	v1 = bx.get_mem_addr(  0x7e20044c );
	v2 = bx.get_mem_block( 0x7e200000 );
	CHECKX( 0x00000113, (v1 - v2)   );	// word offset
	CHECKX( 0x0000044c, (v1 - v2)*4 );	// byte offset
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52", "get_mem_addr() word alignment" );
    try {
	rgAddrMap		bx;
	bx.open_fake_mem();
	bx.get_mem_addr( 0x7e200002 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "get_mem_addr() address not word aligned:  0x7e200002",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//==========================================================================
//## TESTONRPI
//==========================================================================
// These tests require a real RPi.

if ( ! TEST_BaseAddr ) {		// Not on RPi, end of tests
    CASE( "99", "Done" );
    return  0;
}

rgRpiRev::Config.BaseAddr.put( TEST_BaseAddr );		// restore true addr

//--------------------------------------
  CASE( "61", "Constructor, real RPi" );
    try {
	rgAddrMap		bx;
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= NULL  Dev_fd= -1  FakeMem= 0", ss.c_str() );
	CHECKX( TEST_BaseAddr, bx.config_BaseAddr() );	// default
	CHECK( 0, bx.is_fake_mem() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## open_dev_file() - real memory
//--------------------------------------------------------------------------

  CASE( "71", "open_dev_file() missing file" );
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

  CASE( "72", "open_dev_file() no write" );
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

  CASE( "73", "open_dev_gpiomem()" );
    try {
	rgAddrMap		bx;
	bx.config_FakeNoPi( 0 );	// throw
	bx.open_dev_gpiomem();
	std::string		ss;
	ss = bx.text_debug();
	CHECK( "ModeStr= /dev/gpiomem  Dev_fd= 3  FakeMem= 0", ss.c_str() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "74", "open_dev_mem()" );
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

//--------------------------------------------------------------------------
//## get_mem_block() - real memory, cache
//--------------------------------------------------------------------------

  CASE( "81a", "get_mem_block() same addr" );
    try {
	rgAddrMap		bx;
	volatile uint32_t*	v1;
	volatile uint32_t*	v2;
	bx.open_dev_gpiomem();
	CHECK( 0, bx.is_fake_mem() );
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

  CASE( "81b", "get_mem_block() different addr" );
    try {
	rgAddrMap		bx;
	volatile uint32_t*	v1;
	volatile uint32_t*	v2;
	bx.open_dev_gpiomem();
	CHECK( 0, bx.is_fake_mem() );
	v1 = bx.get_mem_block( 0x7e200000 );
	CHECK( 1, bx.size_BlkCache() );
	v2 = bx.get_mem_block( 0x7e210000 );
	CHECK( 2, bx.size_BlkCache() );
	CHECK( 1, (v1 != v2) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Destructor - device file descriptor get_DevFD()
//--------------------------------------------------------------------------

// Device file descriptor should be released on destruction and re-acquired.
  CASE( "90", "Destructor - device file descriptor" );
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

//--------------------------------------
  CASE( "99", "Done" );
}

