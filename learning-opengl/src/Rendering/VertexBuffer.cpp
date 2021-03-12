#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &m_rendererId); //amount to create & buffer to output to.
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId); //select the buffer
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); //give the binded buffer the data for drawing the geometry
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_rendererId);
}

void VertexBuffer::Bind() const 
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId); //select the buffer
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); //select no buffer
}
