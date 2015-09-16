//PCI driver
#include "stdheader.h"
#include "io_ports.h"

//Reads a word on the specified bus slot offset
ushort pci_config_read_word(uchar bus, uchar slot, uchar func, uchar offset)
{
	uint address;
	uint ibus 	= (uint) bus;
	uint islot 	= (uint) slot;
	uint ifunc 	= (uint) func;
	uint ioffset 	= (uint) offset;

	address = (uint) ((ibus << 16) | (islot << 11) | (ifunc << 8) | (ioffset & 0xFC) | ((uint)0x80000000));
	
	port_dword_out(0xCF8, address);
	
	//read in data
	ushort tmp = (ushort) ((port_dword_in(0xCFC) >> (offset & 2)*8) & 0xFFFF);
	return tmp;
}

//Checks if there is a device connected to the specified bus slot
ushort pci_check_vendor(ushort bus, ushort slot)
{
	ushort vendor, device;
	if((vendor = pci_config_read_word(bus,slot,0,0)) != 0xFFFF)
	{
		device = pci_config_read_word(bus,slot,0,2);
		//. . .
		return vendor;
	}
	return 0;
}

//
ushort pci_fill_common_header(ushort bus, ushort slot)
{
	return 0;
}
