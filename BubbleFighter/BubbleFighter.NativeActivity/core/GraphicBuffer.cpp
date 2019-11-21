#include "GraphicBuffer.h"


namespace core
{

	unsigned int GraphicBuffer::getRemainingBytes() const
	{
		return localBuffer.size() - bufferCurrentPos;
	};


	unsigned char GraphicBuffer::getElementTypeMultiplier(GLenum _elementType) const
	{
		switch (_elementType)
		{
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			return 1;

		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
			return 2;

		case GL_FIXED:
		case GL_FLOAT:
		default:
			return 4;
		};
	};


	GraphicBuffer::GraphicBuffer(GLenum _bufferUsageType, GLenum _bufferType, GLenum _elementType) :
		bufferId((GLuint)0),
		bufferUsageType(_bufferUsageType),
		bufferType(_bufferType),
		elementType(_elementType),
		bufferInitiated(false),
		elemTypeMultiplier(getElementTypeMultiplier(_elementType)),
		localBuffer(0),
		bufferCurrentPos(0)
	{};


	GraphicBuffer::~GraphicBuffer()
	{
		unload();
	};


	void GraphicBuffer::load()
	{
		assert(bufferInitiated && "Buffer already initiated");

		try
		{
			GL_ERROR_CHECK(glGenBuffers(1, &bufferId));
			GL_ERROR_CHECK(glBindBuffer(bufferType, bufferId));
			GL_ERROR_CHECK(glBufferData(bufferType, localBuffer.size(), 0, bufferUsageType));
			glBindBuffer(bufferType, 0);
		}
		catch (const std::exception &e)
		{
			if (bufferId != (GLuint)0)
			{
				glDeleteTextures(1, &bufferId);
				glGetError();
				bufferId = (GLuint)0;
			}

			Logger::getSingleton().write(e.what(), LL_ERROR);
			return;
		}

		bufferInitiated = true;
	};


	void GraphicBuffer::unload()
	{
		if (!bufferInitiated) return;

		bufferInitiated = false;

		glDeleteTextures(1, &bufferId);
		glGetError();

		bufferId = (GLuint)0;
	};


	void GraphicBuffer::reload()
	{
		unload();
		load();
	};


	int GraphicBuffer::uploadData()
	{
		try
		{
			GL_ERROR_CHECK(glBufferSubData(bufferId, 0, (GLsizeiptr)(bufferCurrentPos * elemTypeMultiplier), localBuffer.data()));
		}
		catch (const std::exception &e)
		{
			Logger::getSingleton().write(e.what(), LL_ERROR);
			return -1;
		}

		return bufferCurrentPos;
	};


	void GraphicBuffer::setBufferUsageType(GLenum _usage)
	{
		assert(bufferInitiated || "Cannot change type of initialized buffer");
		assert((_usage != GL_STREAM_DRAW && _usage != GL_STATIC_DRAW && _usage != GL_DYNAMIC_DRAW) || "Incorrect buffer usage type selected");

		bufferUsageType = _usage;
	};


	void GraphicBuffer::setBufferType(GLenum _type)
	{
		assert(bufferInitiated || "Cannot change type of initialized buffer");
		assert((_type != GL_ARRAY_BUFFER && _type != GL_ELEMENT_ARRAY_BUFFER) || "Incorrect buffer type selected");

		bufferType = _type;
	};


	void GraphicBuffer::setElementType(GLenum _elementType)
	{
		assert(bufferInitiated || "Cannot change type of initialized buffer");
		assert((_elementType != GL_UNSIGNED_BYTE && _elementType != GL_UNSIGNED_SHORT && bufferType == GL_ELEMENT_ARRAY_BUFFER) || "Incorrect element type selected");

		if (elementType != _elementType)
		{
			unsigned int size = localBuffer.size() / elemTypeMultiplier;

			elemTypeMultiplier = getElementTypeMultiplier(_elementType);
			elementType = _elementType;

			size *= elemTypeMultiplier;
			if (localBuffer.size() != size)
			{
				localBuffer.resize(size);
				localBuffer.shrink_to_fit();
			}
		}
	};


	void GraphicBuffer::resize(unsigned int _size)
	{
		assert(bufferInitiated || "Cannot change size of initialized buffer");

		_size *= elemTypeMultiplier;
		if (_size < localBuffer.size())
			bufferCurrentPos = _size;
		localBuffer.resize(_size);
		localBuffer.shrink_to_fit();
	};


	void GraphicBuffer::rewind()
	{
		bufferCurrentPos = 0;
	};


	unsigned int GraphicBuffer::seek(unsigned int _pos)
	{
		bufferCurrentPos = (_pos * elemTypeMultiplier) > localBuffer.size() ? (localBuffer.size() / elemTypeMultiplier) : _pos;
		return bufferCurrentPos;
	};


	unsigned int GraphicBuffer::getLength() const
	{
		return localBuffer.size() / elemTypeMultiplier;
	};


	unsigned int GraphicBuffer::getRemainingLength() const
	{
		return (getRemainingBytes()) / elemTypeMultiplier;
	};

	unsigned int GraphicBuffer::position() const
	{
		return bufferCurrentPos;
	};

	unsigned char GraphicBuffer::elementSize() const
	{
		return elemTypeMultiplier;
	};


	unsigned int GraphicBuffer::write(const void* _elements, unsigned int _count)
	{
		if (getRemainingBytes() < (_count))
			return 0;

		memcpy(&localBuffer[bufferCurrentPos * elemTypeMultiplier], _elements, _count);
		bufferCurrentPos += _count * 0.25f;

		return _count;
	};

}
