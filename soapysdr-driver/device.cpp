/*
 * This file is part of the otl-modem project.
 *
 * Copyright (C) 2018 Joel Holdsworth <joel@airwebreathe.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <stdexcept>

#include <ftdi.h>
#include <SoapySDR/Formats.h>

#include "device.hpp"

namespace OTLModem {

static const size_t MTU = 16 * 1024;

static std::string makeDeviceString(const SoapySDR::Kwargs &args) {
    std::ostringstream ss;
    ss << "d:" << args.at("busnum") << '/' << args.at("devnum");
    return ss.str();
}

Device::Device(const SoapySDR::Kwargs &args) :
    args_(args),
    ftdic_(nullptr) {
}


Device::~Device() {
}

std::string Device::getDriverKey() const {
    return "OTLModem";
}

std::string Device::getHardwareKey() const {
    return "OTLModem";
}

SoapySDR::Kwargs Device::getHardwareInfo() const {
    SoapySDR::Kwargs info;
    info["origin"] = "https://github.com/jhol/otl-modem";
    return info;
}

size_t Device::getNumChannels(const int dir) const {
    (void)dir;
    return (dir == SOAPY_SDR_RX) ? 1 : 0;
}

std::vector<std::string> Device::getStreamFormats(const int direction,
    const size_t channel) const {
    (void)direction;
    (void)channel;
    return std::vector<std::string>{SOAPY_SDR_U8};
}

std::string Device::getNativeStreamFormat(const int direction,
    const size_t channel, double &fullScale) const {
    (void)direction;
    (void)channel;
    fullScale = 255;
    return SOAPY_SDR_U8;
}

SoapySDR::RangeList Device::getSampleRateRange(
    const int direction, const size_t channel) const {
    (void)direction;
    (void)channel;
    return {{8000000.0, 8000000.0}};
}

SoapySDR::Stream* Device::setupStream(const int direction,
    const std::string &format, const std::vector<size_t> &channels,
    const SoapySDR::Kwargs &args) {
    (void)direction;
    (void)format;
    (void)channels;
    (void)args;

    ftdic_ = ftdi_new();
    if (!ftdic_)
        throw std::runtime_error("Failed to create libftdi context");

    try {
        std::ostringstream ss;
        int ret;

        const std::string deviceString = makeDeviceString(args_);
        if ((ret = ftdi_usb_open_string(ftdic_, deviceString.c_str())) != 0) {
            ss << "Failed to open device (" << ret << "): " <<
                ftdi_get_error_string(ftdic_);
            throw std::runtime_error(ss.str());
        }

        if ((ret = ftdi_usb_reset (ftdic_)) != 0) {
            ss << "Failed to reset device (" << ret << "): " <<
                ftdi_get_error_string(ftdic_);
            throw std::runtime_error (ss.str());
        }

        if ((ret = ftdi_set_bitmode(ftdic_, 0xFF, BITMODE_RESET)) != 0) {
            ss << "Failed to reset the FTDI chip bitmode (" << ret << "): " <<
                ftdi_get_error_string(ftdic_);
            throw std::runtime_error(ss.str());
        }

        if ((ret = ftdi_usb_purge_buffers(ftdic_)) != 0) {
            ss << "Failed to purge FTDI RX/TX buffers (" << ret << "): " <<
                ftdi_get_error_string(ftdic_);
            throw std::runtime_error(ss.str());
        }

        if ((ret = ftdi_set_bitmode(ftdic_, 0xFF, BITMODE_SYNCFF)) != 0) {
            ss << "Failed to put FTDI chip into sync-fifo mode (" << ret <<
                "): " << ftdi_get_error_string(ftdic_);
            throw std::runtime_error(ss.str());
        }
    } catch (const std::runtime_error &e) {
        ftdi_usb_close (ftdic_);
        ftdic_ = NULL;
        throw e;
    }

    return nullptr;
}

void Device::closeStream(SoapySDR::Stream *stream) {
    (void)stream;

    ftdi_free(ftdic_);
    ftdic_ = nullptr;
}

size_t Device::getStreamMTU(SoapySDR::Stream *stream) const {
    (void)stream;
    return MTU;
}

}
