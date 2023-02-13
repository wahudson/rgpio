# 2015-09-12  William A. Hudson

# Run all test suites.

PATH = /usr/bin:/bin

SHELL = /bin/sh

test:
	cd src                && make
	cd t_Error            && make test
	cd t_clk              && make test
	cd t_fsel             && make test
	cd t_header           && make test
	cd t_iic              && make test
	cd t_info             && make test
	cd t_io               && make test
	cd t_pud              && make test
	cd t_pull             && make test
	cd t_pwm              && make test
	cd t_rgpio            && make test
	cd t_spi0             && make test
	cd t_uspi             && make test
	cd t_yUtil            && make test

clean:
	cd t_Error            && make clean
	cd t_clk              && make clean
	cd t_fsel             && make clean
	cd t_header           && make clean
	cd t_iic              && make clean
	cd t_info             && make clean
	cd t_io               && make clean
	cd t_pud              && make clean
	cd t_pull             && make clean
	cd t_pwm              && make clean
	cd t_rgpio            && make clean
	cd t_spi0             && make clean
	cd t_uspi             && make clean
	cd t_yUtil            && make clean

