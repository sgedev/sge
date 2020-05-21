//
//
#ifndef SGE_RESOURCE_HPP
#define SGE_RESOURCE_HPP

#include <map>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <future>

#include <SGE/Common.hpp>

SGE_BEGIN

class Resource;
typedef std::shared_ptr<Resource> ResourcePtr;

class Resource {
public:
	Resource(void);
	virtual ~Resource(void);

public:
	bool ready(void) const;
	int64_t size(void) const;
	int64_t read(int64_t offset, void *buf, int size) const;
	int64_t write(int64_t offset, const void *buf, int size);

public:
	static bool init(const std::string &baseDir);
	static void shutdown(void);
	static bool addPackage(const std::string &packagePath, const std::string path = "/");
	static bool removePackage(const std::string &packagePath);
	static const ResourcePtr get(const std::string &path);
	static ResourcePtr create(const std::string &path);
	static bool remove(const std::string &path);

private:
	static void backend(void);

protected:
	virtual void dummy(void) = 0;

protected:
	uint8_t *m_data;
	int64_t m_size;

private:
	typedef std::map<std::string, ResourcePtr> ResourceMap;
	static ResourceMap m_resources;
	static std::thread m_backendThread;
};

SGE_INLINE bool Resource::ready(void) const
{
	return (m_data != NULL);
}

SGE_INLINE int64_t Resource::size(void) const
{
	return m_size;
}

SGE_END

#endif // SGE_RESOURCE_HPP
