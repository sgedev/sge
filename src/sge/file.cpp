//
//
#include <sge/file.hpp>

SGE_BEGIN

file::file(void):
	m_fops(nullptr)
{
}

file::file(const std::string &filename) :
    m_filename(filename),
    m_fops(nullptr)
{
}

file::~file(void)
{
    if (is_open())
        close();
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

    set_seekable(true);

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

int64_t file::read(void *p, int64_t size)
{
	SGE_ASSERT(m_fops != nullptr);

    return SDL_RWread(m_fops, p, 1, size);
}

int64_t file::write(const void *p, int64_t size)
{
	SGE_ASSERT(m_fops != nullptr);

    return SDL_RWwrite(m_fops, p, 1, size);
}

int64_t file::pos(void)
{
    SGE_ASSERT(m_fops != nullptr);

    return SDL_RWtell(m_fops);
}

int64_t file::set_pos(int64_t pos)
{
    SGE_ASSERT(m_fops != nullptr);

    return SDL_RWseek(m_fops, pos, RW_SEEK_SET);
}

SGE_END
