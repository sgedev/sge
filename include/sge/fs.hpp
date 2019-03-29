//
//
#ifndef SGE_FS_HPP
#define SGE_FS_HPP

#include <miniz_zip.h>
#include <pugixml.hpp>

#include <sge/common.hpp>

SGE_BEGIN

class fs {
public:
	class file {
	public:
		file(void);
		virtual ~file(void);

	private:
		mz_zip_archive &m_reader;
	};

public:
	fs(void);
	virtual ~fs(void);

public:
	bool init(const char *root_image);
	void shutdown(void);

private:
	mz_zip_archive m_reader;
	pugi::xml_document m_manifest;
};

SGE_END

#endif // SGE_FS_HPP

