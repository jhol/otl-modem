##
## This file is part of the otl-modem project.
##
## Copyright (C) 2018 Joel Holdsworth <joel@airwebreathe.org.uk>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

cmake_minimum_required(VERSION 2.6)
project(SoapySDROTLModem CXX)
enable_testing()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "Build type not specified: defaulting to release.")
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")

find_package(SoapySDR REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PKGDEPS REQUIRED libftdi>=0.20 libusb>=0.1.12)

include_directories(${OTL_MODEM_INCLUDE_DIRS} ${PKGDEPS_INCLUDE_DIRS})
link_directories(${PKGDEPS_LIBRARY_DIRS})

SOAPY_SDR_MODULE_UTIL(
    TARGET OTLModem
    SOURCES device.cpp otl-modem-support.cpp
    LIBRARIES ${OTL_MODEM_LIBRARIES} ${PKGDEPS_LIBRARIES}
)
