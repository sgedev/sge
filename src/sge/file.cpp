//
//
#include <sge/file.hpp>

SGE_BEGIN

file::file(void):
	m_fops(nullptr)
{
}

file::file(const std::string &filename) :
	m_fops(nullptr),
	m_filename(filename)
{
}

file::~file(void)
{
}

bool file::is_seekable(void)
{
	return true;
}

bool file::open(int flags)
{
	SGE_ASSERT(m_fops == nullptr);

	if (!io::open(flags))
		return false;

	std::string mode;

	if (flags & FLAG_READ)
		mode += 'r';
	if (flags & FLAG_WRITE)
		mode += 'w';

	m_fops = SDL_RWFromFile(m_filename.c_str(), mode.c_str());
	if (m_fops == nullptr) {
		io::close();
		return false;
	}

	return true;
}

void file::close(void)
{
	SGE_ASSERT(m_fops != nullptr);

	SDL_RWclose(m_fops);
	m_fops = nullptr;

	io::close();
}

int64_t file::size(void)
{
	SGE_ASSERT(m_fops != nullptr);

	return SDL_RWsize(m_fops);
}

int64_t file::seek(int64_t offset, seek_from from)
{
	SGE_ASSERT(m_fops != nullptr);

	int w;

	switch (from) {
	case SEEK_FROM_START:
		w = RW_SEEK_SET;
		break;
	case SEEK_FROM_CURRENT:
		w = RW_SEEK_CUR;
		break;
	case SEEK_FROM_END:
		w = RW_SEEK_END;
		break;
	default:
		return -1;
	}

	return SDL_RWseek(m_fops, offset, w);
}

int64_t file::read(void *p, int64_t len)
{
	SGE_ASSERT(m_fops != nullptr);

	return SDL_RWread(m_fops, p, 1, len);
}

int64_t file::write(const void *p, int64_t len)
{
	SGE_ASSERT(m_fops != nullptr);

	return SDL_RWwrite(m_fops, p, 1, len);
}

SGE_END
