#ifndef UTILS_H
#define UTILS_H

#include <psp2/gxm.h>
#include <psp2/display.h>
#include <psp2/kernel/sysmem.h>

#include <string.h>
#include <stdlib.h>

#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

#ifdef __cplusplus
extern "C" {
#endif

void *gpu_alloc_map(SceKernelMemBlockType type, SceGxmMemoryAttribFlags gpu_attrib, size_t size, SceUID *uid);
void gpu_unmap_free(SceUID uid);

void *gpu_vertex_usse_alloc_map(size_t size, SceUID *uid, unsigned int *usse_offset);
void gpu_vertex_usse_unmap_free(SceUID uid);
void *gpu_fragment_usse_alloc_map(size_t size, SceUID *uid, unsigned int *usse_offset);
void gpu_fragment_usse_unmap_free(SceUID uid);

void *shader_patcher_host_alloc_cb(void *user_data, unsigned int size);
void shader_patcher_host_free_cb(void *user_data, void *mem);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H