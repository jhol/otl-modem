#
# Copyright (c) 2018 Joel Holdsworth <joel@airwebreathe.org.uk>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

PROJ = otl-modem
PIN_DEF = syn/otl-modem.pcf
DEVICE = hx8k

ARACHNE = arachne-pnr 
ARACHNE_ARGS = 
ICEPACK = icepack
ICETIME = icetime
ICEPROG = iceprog

src=

all: $(PROJ).bin

synthesize: otl-modem.bin

%.blif: syn/top.v $(src)
	yosys -p 'synth_ice40 -top top -blif $@' $<
	# yosys -q -p "synth_ice40 -blif otl-modem.blif" syn/top.v $(src)

%.asc: $(PIN_DEF) %.blif syn/otl-modem.pcf
	$(ARACHNE) $(ARACHNE_ARGS) -d $(subst up,,$(subst hx,,$(subst lp,,$(DEVICE)))) -o $@ -p $^

%.rpt: %.asc
	$(ICETIME) -d $(DEVICE) -P tq144 -p syn/otl-modem.pcf -mtr $@ $<

%.bin: %.asc
	$(ICEPACK) $< $@

simulate-ft2232h_interface: ft2232h_interface.vcd
	gtkwave $< >/dev/null 2>/dev/null &

ft2232h_interface-sim: \
	sim/ft2232h_interface.v \
	src/ft2232h_interface.v
	iverilog -o $@ $^

%.vcd: %-sim
	cd $(dir $<); ./$(notdir $<)

clean:
	rm -f $(PROJ).blif $(PROJ).asc $(PROJ).rpt $(PROJ).bin

.SECONDARY:
.PHONY: all prog clean

.PHONY: \
  synthesize \
  simulate-ft2232h_interface \
