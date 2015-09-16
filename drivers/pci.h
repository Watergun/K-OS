#ifndef PCI_H
#define PCI_H

#include "stdheader.h"

typedef struct{
	ushort vendor_id;
	ushort device_id;
	ushort command;
	ushort status;
	uchar revision_id;
	uchar prog_if;
	uchar subclass;
	uchar class;
	uchar cache_line_size;
	uchar latency_timer;
	uchar header_type;
	uchar bist;
}	pic_common_header;

//All implemented functions -> pci.c for description
ushort pci_config_read_word(uchar bus, uchar slot, uchar func, uchar offset);
ushort pci_check_vendor(ushort bus, ushort slot);
ushort pci_fill_common_header(ushort bus, ushort slot);

#endif
