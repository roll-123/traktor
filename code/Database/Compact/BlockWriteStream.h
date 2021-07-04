#pragma once

#include "Core/Ref.h"
#include "Core/Io/IStream.h"
#include "Database/Compact/BlockFile.h"

namespace traktor
{

class ChunkMemory;
class ChunkMemoryStream;

	namespace db
	{

class BlockWriteStream : public IStream
{
public:
	BlockWriteStream(BlockFile* blockFile, IStream* stream, BlockFile::Block& outBlock);

	virtual ~BlockWriteStream();

	virtual void close() override final;

	virtual bool canRead() const override final;

	virtual bool canWrite() const override final;

	virtual bool canSeek() const override final;

	virtual int64_t tell() const override final;

	virtual int64_t available() const override final;

	virtual int64_t seek(SeekOriginType origin, int64_t offset) override final;

	virtual int64_t read(void* block, int64_t nbytes) override final;

	virtual int64_t write(const void* block, int64_t nbytes) override final;

	virtual void flush() override final;

private:
	Ref< ChunkMemory > m_memory;
	Ref< ChunkMemoryStream > m_memoryStream;
	Ref< BlockFile > m_blockFile;
	Ref< IStream > m_outputStream;
	BlockFile::Block& m_outBlock;
};

	}
}