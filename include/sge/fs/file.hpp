//
//
#ifndef SGE_FS_FILE_HPP
#define SGE_FS_FILE_HPP

#include <sge/fs/common.hpp>

SGE_FS_BEGIN

class file {
public:
	enum seek_from {
		SEEK_FROM_START = 0,
		SEEK_FROM_CURRENT,
		SEEK_FROM_END
	};

public:
	file(SDL_RWops *ops);
	file(const char *filename);
	file(void *mem, int size);
	file(const void *mem, int size);
	virtual ~file(void);

public:
	virtual void close(void);
	int64_t size(void) const;
	int64_t tell(void) const;
	int64_t seek(int64_t offset, seek_from from = SEEK_FROM_START);
	int64_t read(void *buf, int64_t buf_size);
	int64_t write(const void *dat, int64_t dat_size);
	uint8_t read_u8(void);
	uint16_t read_be16(void);
	uint32_t read_be32(void);
	uint64_t read_be64(void);
	uint16_t read_le16(void);
	uint32_t read_le32(void);
	uint64_t read_le64(void);
	bool write_u8(uint8_t v);
	bool write_be16(uint16_t v);
	bool write_be32(uint32_t v);
	bool write_be64(uint64_t v);
	bool write_le16(uint16_t v);
	bool write_le32(uint32_t v);
	bool write_le64(uint64_t v);

protected:
	SDL_RWops *m_ops;
};

inline file::file(SDL_RWops *ops)
	: m_ops(ops)
{
}

inline file::file(const char *filename)
	: m_ops(SDL_RWFromFile(filename, "wb"))
{
}

inline file::file(void *mem, int size)
	: m_ops(SDL_RWFromMem(mem, size))
{
}

inline file::file(const void *mem, int size)
	: m_ops(SDL_RWFromConstMem(mem, size))
{
}

inline file::~file(void)
{
	if (m_ops != NULL)
		close();
}

inline void file::close(void)
{
	SGE_ASSERT(m_ops != NULL);

	SDL_RWclose(m_ops);
	m_ops = NULL;
}

inline int64_t file::size(void) const
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_RWsize(m_ops);
}

inline int64_t file::tell(void) const
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_RWtell(m_ops);
}

inline int64_t file::read(void *buf, int64_t buf_size)
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_RWread(m_ops, buf, 1, buf_size);
}

inline int64_t file::write(const void *dat, int64_t dat_size)
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_RWwrite(m_ops, dat, 1, dat_size);
}

inline uint8_t file::read_u8(void)
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_ReadU8(m_ops);
}

inline uint16_t file::read_be16(void)
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_ReadBE16(m_ops);
}

inline uint32_t file::read_be32(void)
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_ReadBE32(m_ops);
}

inline uint64_t file::read_be64(void)
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_ReadBE64(m_ops);
}

inline uint16_t file::read_le16(void)
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_ReadLE16(m_ops);
}

inline uint32_t file::read_le32(void)
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_ReadLE32(m_ops);
}

inline uint64_t file::read_le64(void)
{
	SGE_ASSERT(m_ops != NULL);

	return SDL_ReadLE64(m_ops);
}

inline bool file::write_u8(uint8_t v)
{
	SGE_ASSERT(m_ops != NULL);

	return (SDL_WriteU8(m_ops, v) == sizeof(v));
}

inline bool file::write_be16(uint16_t v)
{
	SGE_ASSERT(m_ops != NULL);

	return (SDL_WriteBE16(m_ops, v) == sizeof(v));
}

inline bool file::write_be32(uint32_t v)
{
	SGE_ASSERT(m_ops != NULL);

	return (SDL_WriteBE32(m_ops, v) == sizeof(v));
}

inline bool file::write_be64(uint64_t v)
{
	SGE_ASSERT(m_ops != NULL);

	return (SDL_WriteBE64(m_ops, v) == sizeof(v));
}

inline bool file::write_le16(uint16_t v)
{
	SGE_ASSERT(m_ops != NULL);

	return (SDL_WriteLE16(m_ops, v) == sizeof(v));
}

inline bool file::write_le32(uint32_t v)
{
	SGE_ASSERT(m_ops != NULL);

	return (SDL_WriteLE32(m_ops, v) == sizeof(v));
}

inline bool file::write_le64(uint64_t v)
{
	SGE_ASSERT(m_ops != NULL);

	return (SDL_WriteLE64(m_ops, v) == sizeof(v));
}

SGE_FS_END

#endif // SGE_FS_FILE_HPP
