// 2020-20-22  William A. Hudson

// rgVersion - Version identification class
//--------------------------------------------------------------------------

#include "rgVersion.h"


/*
* Return version string.
*    The file contains the quoted version string, generated in the Makefile
*    by 'git describe'.  Format is  TAG-N-ID, where
*        TAG = git tag name
*        N   = number of following commits
*        ID  = abbreviated git object name
*    e.g.  "v0.11.0-65-g21f2cce"
* call:
*    rgVersion::get_Version()
* return:
*    () =  version string
*/
const char *
rgVersion::get_Version()
{
    return
	#include "version.inc"
    ;
}

