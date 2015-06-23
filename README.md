# K-OS

Ein Betriebssystem, das zurzeit entwickelt wird. Der Assemblercode ist in der üblichen Intel-Schreibweise geschrieben. Alles weitere 
wird mit C geschriben. Es existiert ein Makefile, mit dem man unter Linux extrem einfach nach kleinen Änderungen das fertige Systembild 
bekommt (make run). Zum Emulieren des Systembilds kann QEMU oder Bochs verwendet werden. Die Maschineninstruktionen sind desweiteren nur 
x86 kompatibel. Eine mögliche Erweiterung auf andere Prozessortypen ist möglich.



EMULATION vs. HARDWARE:

Das Projekt kann in Prozessoremulatoren wie QEMU oder Bochs getestet werden. Wenn die Datei os-image direkt emuliert werden soll, gilt 
es folgendes zu beachten:

Wichtig: Der Bootloader lädt den Kernel von der ersten Partition, die in der MBR steht.
 In einer Emulationsumgebung ist eine Partitionstabelle nicht unbedingt vorhanden, weshalb es dadurch zu Fehlern kommen kann.
 Was zu ändern ist: Die Datei boot/16bit/disk/disk_load.asm kopiert CHS Informationen "mov ch, [0x7bf]" aus der MBR. 
				Diese Informationen müssen durch Cylinder = 0, Head = 0, Sector = 2 (0/0/2) ersetzt werden, denn der erste
				Kernelsektor ist in der Direktemulation hinter dem Bootsector, der an (0/0/0) steht.

Wer das nicht ändern möchte, kann auch indirekt emulieren, indem "sudo bash hdd_test" ausgeführt wird, wodurch das os-image
auf die erste Partition der sekundären Festplatte (/dev/sdb) geschrieben wird.

