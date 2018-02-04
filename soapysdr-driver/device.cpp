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

#include <SoapySDR/Formats.h>

#include "device.hpp"

namespace OTLModem {

Device::Device(const SoapySDR::Kwargs &args) :
    args_(args) {
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
    return 2;
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

}
