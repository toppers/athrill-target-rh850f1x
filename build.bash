#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 {all|clean]"
	exit 1
fi

opt=${1}

if [ ${opt} = "clean" ]
then
	CURR_DIR=`pwd`
	cd hakoniwa-core-cpp-client
	bash build.bash clean
	cd ${CURR_DIR}

	cd build_linux
	make clean
	cd ${CURR_DIR}
else
	CURR_DIR=`pwd`
	cd hakoniwa-core-cpp-client
	bash build.bash
	cd ${CURR_DIR}

	cd build_linux
	make skip_clock_bugfix=true fix_intr_cpu_rcv_clr=true fix_intr_pmr_bugfix=true
	cd ${CURR_DIR}
fi

