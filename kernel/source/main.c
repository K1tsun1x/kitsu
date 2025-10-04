

extern void kitsu_kernel_main(void* ptr) {
	__asm__ __volatile__("jmp .");
}