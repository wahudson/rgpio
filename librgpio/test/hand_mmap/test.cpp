// 2024-06-07  William A. Hudson
//
// Explore mmap64(2) system call.
//    % make cpp	# see preprocessor output
//    % make		# execute to see sizeof
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <unistd.h>
#include <sys/mman.h>	// mmap()

using namespace std;

//--------------------------------------------------------------------------

int main()
{
    int		BlockSize   = 4096 * 4;
    int		Dev_fd      = 3;		// file descriptor
//  void*	mem_block;

    off_t	offset_addr = 0x2000;

    cerr << "+ getpagesize()     = " << getpagesize()   <<endl;
    cerr << "+ sizeof( char* )   = " << sizeof( char* ) <<endl;
    cerr << "+ sizeof( off_t )   = " << sizeof( off_t ) <<endl;
    cerr << "+ sizeof( off64_t ) = " << sizeof( off64_t ) <<endl;

    mmap64(
	NULL,			// Any address in our space will do
	BlockSize,		// Map length
	PROT_READ|PROT_WRITE,	// Enable reading & writing to mapped memory
	MAP_SHARED,		// Shared with other processes
	Dev_fd,			// File descriptor to map
	offset_addr		// Offset to GPIO peripheral, page aligned
    );
    // ignore result

}
