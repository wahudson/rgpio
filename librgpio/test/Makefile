# 2015-09-12  William A. Hudson

# Run all test suites.

PATH = /usr/bin:/bin

SHELL = /bin/sh

test:
	cd t_rgAddrMap        && make test
	cd t_rgAltFuncName    && make test
	cd t_rgClk            && make test
	cd t_rgFselPin        && make test
	cd t_rgHeaderPin      && make test
	cd t_rgIic            && make test
	cd t_rgIoPins         && make test
	cd t_rgPads           && make test
	cd t_rgPudPin         && make test
	cd t_rgPullPin        && make test
	cd t_rgPwm            && make test
	cd t_rgRegister       && make test
	cd t_rgRpiRev_Code    && make test
	cd t_rgRpiRev_Word    && make test
	cd t_rgRpiRev_a       && make test
	cd t_rgSpi0           && make test
	cd t_rgSysTimer       && make test
	cd t_rgUniSpi         && make test
#	cd t_utLib1           && make test

# on Rpi only - need sudo, test.init, and hardware jumpers
Live:
	cd v_rgIic_a          && make test
	cd v_rgPudPin_prog    && make test
	cd v_rgSpi0_a         && make test
	cd v_rgSpi0_fifo      && make test
	cd v_rgSpi0_gpio      && make test
	cd v_rgUniSpi         && make test

clean:
	cd t_rgAddrMap        && make clean
	cd t_rgAltFuncName    && make clean
	cd t_rgClk            && make clean
	cd t_rgFselPin        && make clean
	cd t_rgHeaderPin      && make clean
	cd t_rgIic            && make clean
	cd t_rgIoPins         && make clean
	cd t_rgPads           && make clean
	cd t_rgPudPin         && make clean
	cd t_rgPullPin        && make clean
	cd t_rgPwm            && make clean
	cd t_rgRegister       && make clean
	cd t_rgRpiRev_Code    && make clean
	cd t_rgRpiRev_Word    && make clean
	cd t_rgRpiRev_a       && make clean
	cd t_rgSpi0           && make clean
	cd t_rgSysTimer       && make clean
	cd t_rgUniSpi         && make clean
#	cd t_utLib1           && make clean
	cd v_rgIic_a          && make clean
	cd v_rgPudPin_prog    && make clean
	cd v_rgSpi0_a         && make clean
	cd v_rgSpi0_fifo      && make clean
	cd v_rgSpi0_gpio      && make clean
	cd v_rgUniSpi         && make clean

