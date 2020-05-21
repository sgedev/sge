//
//
#include <physfs.h>

#include <SGE/Resource.hpp>

SGE_BEGIN

// ResourceImpl

class ResourceImpl: public Resource {
public:
	ResourceImpl(PHYSFS_File *fp);
	virtual ~ResourceImpl(void);

protected:
	void dummy(void) override;

private:
	PHYSFS_File *m_fp;
};

ResourceImpl::ResourceImpl(PHYSFS_File *fp):
	m_fp(fp)
{
}

ResourceImpl::~ResourceImpl(void)
{
	if (m_fp != NULL)
		PHYSFS_close(m_fp);
}

void ResourceImpl::dummy(void)
{
}

// Resource

Resource::ResourceMap Resource::m_resources;

Resource::Resource(void):
	m_data(NULL),
	m_size(0)
{
}

Resource::~Resource(void)
{
	if (m_data != NULL)
		delete [] m_data;
}

int64_t Resource::read(int64_t offset, void *buf, int size) const
{
	SGE_ASSERT(m_data != NULL);
	SGE_ASSERT(m_size > 0);
	SGE_ASSERT(offset >= 0);
	SGE_ASSERT(offset < m_size);
	SGE_ASSERT(buf != NULL);
	SGE_ASSERT(size > 0);
	
	// TODO
}

int64_t Resource::write(int64_t offset, const void *buf, int size)
{
	SGE_ASSERT(m_data != NULL);
	SGE_ASSERT(m_size > 0);
	SGE_ASSERT(offset >= 0);
	SGE_ASSERT(offset < m_size);
	SGE_ASSERT(buf != NULL);
	SGE_ASSERT(size > 0);

	// TODO
}

bool Resource::init(const std::string &baseDir)
{
	std::promise<bool> state;
	std::future<bool> stateFuture = state.get_future();
	m_backendThread = std::thread(&Resource::backend, this, &stateFuture);

	return true;
}

void Resource::shutdown(void)
{

}

bool Resource::addPackage(const std::string &packagePath, const std::string path = "/")
{

}

bool Resource::removePackage(const std::string &packagePath)
{

}

const ResourcePtr Resource::get(const std::string &path)
{

}

ResourcePtr Resource::create(const std::string &path)
{

}

bool Resource::remove(const std::string &path)
{

}

void Resource::backend(std::future<bool> *state)
{
	int ret;

	SGE_ASSERT(state != NULL);

	ret = PHYSFS_init("sge");
	if (!ret) {
		state->set_value(false);
		return;
	}

	uv_loop_t loop;
	ret = uv_loop_init(&loop);
	if (ret < 0) {
		PHYSFS_deinit();
		state->set_value(false);
		return;
	}

	uv_run(&loop, UV_RUN_DEFAULT);

	uv_loop_close(&loop);

	PHYSFS_deinit();
}

SGE_END
