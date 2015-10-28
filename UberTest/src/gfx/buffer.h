
#ifndef _BUFFER_
#define _BUFFER_

#include <types_v7.h>

typedef uint32 BufferId;

BufferId bufferCreate();

void bufferStaticVertexData(BufferId id, void * data, uint32 size);
void bufferStaticIndexData(BufferId id, void * data, uint32 size);
void bufferDynamicVertexData(BufferId id, void * data, uint32 size);
void bufferDynamicIndexData(BufferId id, void * data, uint32 size);

void bufferVertexSubData(BufferId id, void * data, uint32 size, uint32 offset);
void bufferIndexSubData(BufferId id, void * data, uint32 size, uint32 offset);

void bufferBindVertex(BufferId id);
void bufferBindIndex(BufferId id);

void bufferDelete(BufferId id);

#endif