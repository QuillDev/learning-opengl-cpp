#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:m_Count(count) //set m_count to the count
{
	glGenBuffers(1, &m_RendererID); //amount to create & buffer to output to.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); //select the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count *sizeof(unsigned int), data, GL_STATIC_DRAW); //give the binded buffer the size for drawing the geometry
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); //select the buffer
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //select no buffer
}
