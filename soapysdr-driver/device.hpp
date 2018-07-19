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

#include <SoapySDR/Device.hpp>

struct ftdi_context;

namespace OTLModem {

class Device : public SoapySDR::Device {
public:
    Device(const SoapySDR::Kwargs &args);

    virtual ~Device();

public:
    std::string getDriverKey() const;
    std::string getHardwareKey() const;
    SoapySDR::Kwargs getHardwareInfo() const;

    size_t getNumChannels(const int dir) const;

    std::vector<std::string> getStreamFormats(const int direction,
        const size_t channel) const;
    std::string getNativeStreamFormat(const int direction,
        const size_t channel, double &fullScale) const;
    SoapySDR::RangeList getSampleRateRange(
        const int direction, const size_t channel) const;

    SoapySDR::Stream* setupStream(const int direction,
        const std::string &format, const std::vector<size_t> &channels,
        const SoapySDR::Kwargs &args);
    void closeStream(SoapySDR::Stream *stream);
    size_t getStreamMTU(SoapySDR::Stream *stream) const;
    int readStream(SoapySDR::Stream *stream, void *const *buffs,
        const size_t numElems, int &flags, long long &timeNs,
        const long timeoutUs);

private:
    const SoapySDR::Kwargs args_;
    ftdi_context *ftdic_;
};

}
