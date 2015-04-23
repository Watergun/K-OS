//
int pciforce(int argc, char **argv)
{
	int bus = 0;
	int slot = 0;
	int vendor = 0;
	
	for(; bus < 256; bus++)
	{
		for(; slot < 32; slot++)
		{
			if((vendor = pci_check_vendor(bus,slot)) != 0)
			{	
				tm_print_hex(vendor);
			}
		}
	}
}
