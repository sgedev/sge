//
//
#ifndef SGE_ZIP_FS_HPP
#define SGE_ZIP_FS_HPP

#include <memory>

#include <sge/common.hpp>
#include <sge/zip_reader.hpp>
#include <sge/fs.hpp>

SGE_BEGIN

typedef std::shared_ptr<zip_reader> zip_reader_ptr;

class zip_fs: public fs {
public:
	zip_fs(void);
	~zip_fs(void) override;

public:
	bool is_writable(void) override;
	bool init(const std::string &native_path) override;
	type_t type(const std::string &path) override;
	bool enum_dir(const std::string &path, strings_t &result) override;
	bool make_dir(const std::string &path) override;
	io *open_file(const std::string &path, int io_flags) override;
	bool remove(const std::string &path) override;

private:
	zip_reader_ptr m_zip;
};

SGE_END

#endif // SGE_ZIP_FS_HPP
