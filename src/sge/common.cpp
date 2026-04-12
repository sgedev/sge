//
//
#include <sge/common.h>
#include <sge/scopeguard.hpp>

extern "C" int asyncReadPhysFile(uv_loop_t* loop, PHYSFS_File* file, uv_work_t* req, AsyncReadPhysFileCallback cb) {
	SGE_ASSERT(loop != nullptr);
	SGE_ASSERT(file != nullptr);
	SGE_ASSERT(req != nullptr);
	SGE_ASSERT(cb != nullptr);

	auto file_size = PHYSFS_fileLength(file);
	if (file_size > 0) {
	}

	return 0;
}
