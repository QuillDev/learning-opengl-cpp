#pragma once

class VertexBuffer {
private:
	unsigned int m_rendererId;
public:
	VertexBuffer(const void* data, unsigned int size); //constructor for new vertex buffers
	~VertexBuffer();

	void Bind() const; //bind the buffer
	void Unbind() const; //unbind the buffer
};