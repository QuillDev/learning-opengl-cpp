#pragma once

class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count); //constructor for new vertex buffers
	~IndexBuffer();

	void Bind() const; //bind the buffer
	void Unbind() const;//unbind the buffer

	inline unsigned int GetCount() const{ return m_Count; }
};