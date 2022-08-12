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
	bash install.bash
	bash build.bash
	cd ${CURR_DIR}

	cd build_linux
	make
	cd ${CURR_DIR}
fi

