# Kitsu
> Kitsu is an educational x86 operating system with its own custom bootloader.
## Progress
### KitsuLoader
- [x] First stage (`pre-bootloader`)
	+ [x] Set video mode to 80x25 text
	+ [x] Reset the disk system
	+ [x] Read the main bootloader header
	+ [x] Verify the header signature
	+ [x] Read the main bootloader
	+ [x] Set stack registers for the main bootloader
	+ [x] Push the drive index
	+ [x] Call the main bootloader entry point
- [ ] Second stage (`main-bootloader`)
	+ [x] Get drive information
	+ [x] Check for the presence of the i8042 controller
	+ [x] Check the A20 line status and enable it if disabled
	+ [x] Retrieve and normalize the memory map
	+ [ ] Locate the kernel file on disk and load it into memory
	+ [ ] Retrieve information about CPUID availability