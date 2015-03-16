#define PIC1			0x20		//Master PIC
#define PIC2			0xA0		//Slave PIC
#define PIC1_COMMAND	PIC1
#define PIC1_DATA		(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA		(PIC2+1)

#define PIC_EOI		0x20

void PIC_sendEIO(unsigned char irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	
	outb(PIC1_COMMAND, PIC_EOI);
}

//ICW2 = Vector Offset
//ICW3 = Master/Slave configuration

#define ICW1_ICW4		0x01		/* ICW4 (not needed) */
#define ICW1_SINGLE		0x02		/* Single (cascade) mode*/
#define ICW1_INTERVAL4	0x04		/* Call address intervall 4*/
#define ICW1_LEVEL		0x08		/* Level triggered mode */
#define ICW1_INIT		0x10		/* Initialisation	*/

#define ICW4_8086		0x01		/* 8086/88 mode*/
#define ICW4_AUTO		0x02		/* Auto EOI */
#define ICW4_BUF_SLAVE	0x08		
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM		0x10		/* Special fully nested*/

void PIC_remap(int offset1, int offset2)
{
	unsigned char a1, a2;
	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMAND, ICW1_INIT+ICW1_ICW4);
	io_wait();
	outb(PIC2_COMAND, ICW1_INIT+ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
	
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}	
