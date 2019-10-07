//
//
#ifndef SGE_DATABASE_HPP
#define SGE_DATABASE_HPP

#include <memory>

#include <pugixml.hpp>

#include <cx/noncopyable.hpp>
#include <cx/readonlyfs.hpp>

#include <sge/common.hpp>

SGE_BEGIN

class Database: public CX::Noncopyable {
public:

	typedef std::shared_ptr<CX::ReadonlyFile> ReadonlyFilePtr;

public:
	Database(void);
	virutal ~Database(void);

public:
	bool init(CX::ReadonlyFS *fs);
	void shutdown(void);
	int getInt(const char *path, int def = 0);
	float getFloat(const char *path, float def = 0.0f);
	double getDouble(const char *path, double def = 0.0);
	const char *getString(const char *path, const char *def = NULL);
	ReadonlyFilePtr getBlob(const char *path);

private:
	CX::ReadonlyFS *m_fs;
	pugi::xml_document m_manifest;
};

SGE_END

#endif // SGE_DATABASE_HPP

