//
//
#include <string>

#include <sge/fs.h>

#define SGE_FS_MAGIC 0x20120920U
#define SGE_FS_VERSION 1

SGE_FS_BEGIN

static SDL_RWops *root_file;

bool init(const char *root)
{
	SGE_ASSERT(root != NULL);
	SGE_ASSERT(root_file == NULL);

	SDL_RWops *file = SDL_RWFromFile(root, "rb");
	if (file == NULL)
		return false;

	Uint32 magic = SDL_ReadBE32(file);
	if (magic != SGE_FS_MAGIC) {
		SDL_RWclose(file);
		return false;
	}

	Uint32 version = SDL_ReadBE32(file);
	if (version < SGE_FS_VERSION) {
		SDL_RWclose(file);
		return false;
	}

	root_file = file;

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(root_file != NULL);
	SDL_RWclose(root_file);
	root_file = NULL;
}

SGE_FS_END
