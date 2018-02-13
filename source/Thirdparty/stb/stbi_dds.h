/*
	adding DDS loading support to stbi
*/

//	is it a DDS file?
static int stbi_dds_test(stbi__context *s);
static int stbi_dds_test_memory(stbi_uc const *buffer, int len);

static stbi_uc *stbi_dds_load(char *filename, int *x, int *y, int *comp, int req_comp);
static stbi_uc *stbi_dds_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);
#ifndef STBI_NO_STDIO
static int stbi_dds_test_file(FILE *f);
static stbi_uc *stbi_dds_load_from_file(FILE *f, int *x, int *y, int *comp, int req_comp);
#endif
