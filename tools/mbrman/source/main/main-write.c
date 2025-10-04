#include <main.h>

int main_write(
	const char* path,
	size_t i,
	uint8_t flags,
	uint16_t start_cyl, uint8_t start_head, uint8_t start_sector,
	uint8_t sys_id,
	uint16_t end_cyl, uint8_t end_head, uint8_t end_sector,
	uint32_t offset,
	uint32_t length
) {
	if (i > 3) {
		printf("Error: the index(%zu) is out of bounds!\r\n", i);
		return EINVAL;
	}

	FILE* f = fopen(path, "rb");
	if (!f) {
		printf("Error: failed to open `%s`!\r\n", path);
		return EACCES;
	}

	fseek(f, 0, SEEK_END);
	size_t sz = (size_t)ftell(f);
	fseek(f, 0, SEEK_SET);

	if (sz < sizeof(mbr_t)) {
		fclose(f);
		printf("Error: image size < %u bytes!\r\n", sizeof(mbr_t));
		return EINVAL;
	}

	unsigned char* data = (unsigned char*)malloc(sz);
	if (!data) {
		fclose(f);
		printf("Error: failed to allocate %zu bytes!\r\n", sz);
		return ENOBUFS;
	}

	if (fread(data, 1, sz, f) != sz) {
		free(data);
		fclose(f);
		printf("Error: failed to read %zu bytes!\r\n", sz);
		return EBUSY;
	}

	fclose(f);

	mbr_t* mbr = (mbr_t*)data;
	if (mbr->signature != MBR_SIGNATURE) {
		free(data);
		printf("Error: MBR signature != %#x\r\n", MBR_SIGNATURE);
		return EINVAL;
	}

	mbr_init_partition(&mbr->partitions[i], flags, start_cyl, start_head, start_sector, sys_id, end_cyl, end_head, end_sector, offset, length);

	f = fopen(path, "wb+");
	if (!f) {
		free(data);
		printf("Error: failed to reopen `%s`!\r\n", path);
		return EACCES;
	}

	if (fwrite(data, 1, sz, f) != sz) {
		free(data);
		fclose(f);
		printf("Error: failed to write %zu bytes!\r\n", sz);
		return EBUSY;
	}

	fclose(f);
	free(data);
	return 0;
}