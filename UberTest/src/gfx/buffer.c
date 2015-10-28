
#include "buffer.h"

#include "gles.h"

static BufferId s_lastBoundVertexBuffer = 0;
static BufferId s_lastBoundIndexBuffer = 0;

BufferId bufferCreate() {
	
	BufferId res = 0;
	glGenBuffers(1, &res);

	return res;
}

void bufferStaticVertexData(BufferId id, void * data, uint32 size) {

	if (id && size) {

		bufferBindVertex(id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
}

void bufferStaticIndexData(BufferId id, void * data, uint32 size) {

	if (id && size) {

		bufferBindIndex(id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
}

void bufferDynamicVertexData(BufferId id, void * data, uint32 size) {

	if (id && size) {

		bufferBindVertex(id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}
}

void bufferDynamicIndexData(BufferId id, void * data, uint32 size) {

	if (id && size) {

		bufferBindIndex(id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}
}


void bufferVertexSubData(BufferId id, void * data, uint32 size, uint32 offset) {

	if (id && data && size) {

		bufferBindVertex(id);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}
}

void bufferIndexSubData(BufferId id, void * data, uint32 size, uint32 offset) {
	
	if (id && data && size) {

		bufferBindIndex(id);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	}
}

void bufferBindVertex(BufferId id) {

	if (s_lastBoundVertexBuffer != id) {

		s_lastBoundVertexBuffer = id;
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
}

void bufferBindIndex(BufferId id) {

	if (s_lastBoundIndexBuffer != id) {

		s_lastBoundIndexBuffer = id;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

}

void bufferDelete(BufferId id) {

	glDeleteBuffers(1, &id);
}