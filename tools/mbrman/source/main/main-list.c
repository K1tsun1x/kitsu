#include <main.h>

int main_list(const char* path, size_t i) {
	if (i != (size_t)-1 && i > 3) {
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

	unsigned char* data = (unsigned char*)malloc(sizeof(mbr_t));
	if (!data) {
		fclose(f);
		printf("Error: failed to allocate %zu bytes!\r\n", sizeof(mbr_t));
		return ENOBUFS;
	}

	if (fread(data, 1, sizeof(mbr_t), f) != sizeof(mbr_t)) {
		free(data);
		fclose(f);
		printf("Error: failed to read %zu bytes!\r\n", sizeof(mbr_t));
		return EBUSY;
	}

	fclose(f);

	mbr_t* mbr = (mbr_t*)data;
	if (mbr->signature != MBR_SIGNATURE) {
		free(data);
		printf("Error: MBR signature != %#x\r\n", MBR_SIGNATURE);
		return EINVAL;
	}

	if (i == (size_t)-1) mbr_print_partitions(
		mbr->partitions,
		0,
		sizeof(mbr->partitions) / sizeof(mbr_partition_t)
	);
	else {
		printf("MBR partition #%zu: ", i + 1);
		mbr_print_partition(&mbr->partitions[i]);
		puts("");
	}

	free(data);
	return 0;
}