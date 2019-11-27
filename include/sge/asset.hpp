//
//
#ifndef SGE_ASSET_HPP
#define SGE_ASSET_HPP

#include <sge/common.hpp>
#include <sge/database.hpp>

SGE_BEGIN

class Asset {
public:
	Asset(void);
	virtual ~Asset(void);

private:
	Database::Blob m_blob;
};

SGE_END

#endif // SGE_ASSET_HPP
