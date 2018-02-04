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

#include <iostream>
#include <iomanip>
#include <memory>

#include <ftdi.h>
#include <SoapySDR/Registry.hpp>
#include <usb.h>

#include "device.hpp"

static SoapySDR::KwargsList listDevices() {
    ftdi_device_list *devlist = nullptr;
    const std::shared_ptr<ftdi_context> ftdic(ftdi_new(), ftdi_free);

    const int ret = ftdi_usb_find_all(ftdic.get(), &devlist, 0x0403, 0x6010);
    if (ret < 0) {
        std::cerr << "Failed to list devices (" << ret << "): " <<
               ftdi_get_error_string(ftdic.get()) << std::endl;
        return SoapySDR::KwargsList();
    }

    SoapySDR::KwargsList devices;
    for (ftdi_device_list *curdev = devlist; curdev;
        curdev = curdev->next) {
        SoapySDR::Kwargs info;

        {
            std::ostringstream ss;
            ss << std::setw(3) << std::setfill('0') <<
                curdev->dev->bus->location;
            info["busnum"] = ss.str();
        }
        {
            std::ostringstream ss;
            ss << std::setw(3) << std::setfill('0') <<
                int(curdev->dev->devnum);
            info["devnum"] = ss.str();
        }

        info["filename"] = curdev->dev->filename;

        devices.push_back(info);
    }

    ftdi_list_free(&devlist);

    return devices;
}

SoapySDR::KwargsList findOTLModem(const SoapySDR::Kwargs &args) {
    const SoapySDR::KwargsList devices = listDevices();
    SoapySDR::KwargsList results;

    for (SoapySDR::Kwargs device : devices) {
        if (!(args.count("busnum") && args.at("busnum") != device.at("busnum")) &&
            !(args.count("devnum") && args.at("devnum") != device.at("devnum")) &&
            !(args.count("filename") && args.at("filename") != device.at("filename")))
            results.push_back(device);
    }

    return results;
}

SoapySDR::Device *makeOTLModem(const SoapySDR::Kwargs &args) {
    (void)args;
    return new OTLModem::Device();
}

static SoapySDR::Registry registerOTLModem("OTLModem",
    &findOTLModem, &makeOTLModem, SOAPY_SDR_ABI_VERSION);
