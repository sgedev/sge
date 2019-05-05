//
//
#include <sge/fs/file.hpp>

SGE_FS_BEGIN

int64_t file::seek(int64_t offset, seek_from from)
{
	SGE_ASSERT(m_ops != NULL);

	int whence;

	switch (from) {
	case SEEK_FROM_START:
		whence = RW_SEEK_SET;
		break;
	case SEEK_FROM_CURRENT:
		whence = RW_SEEK_CUR;
		break;
	case SEEK_FROM_END:
		whence = RW_SEEK_END;
		break;
	default:
		SGE_ASSERT(false);
		break;
	}

	return SDL_RWseek(m_ops, offset, whence);
}

SGE_FS_END
