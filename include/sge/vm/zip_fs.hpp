//
//
#ifndef SGE_VM_ZIP_FS_HPP
#define SGE_VM_ZIP_FS_HPP

#include <memory>
#include <string>
#include <map>

#include <sge/zip_reader.hpp>
#include <sge/vm/common.hpp>
#include <sge/vm/fs.hpp>

SGE_VM_BEGIN

typedef std::shared_ptr<zip_reader> zip_reader_ptr;

class zip_fs: public fs {
public:
    zip_fs(void);
    ~zip_fs(void) override;

public:
    bool is_writable(void) override;
    bool init(const std::string &native_path) override;
    type_t type(const std::string &path) override;
    bool enum_dir(const std::string &path, string_list_t &result) override;
    bool make_dir(const std::string &path) override;
    io *open_file(const std::string &path, int io_flags) override;
    bool remove(const std::string &path) override;

protected:
    struct node {
        std::string name;
        int first_child;
        int next_sibling;

        node(void): first_child(-1), next_sibling(-1) { }
    };

    bool populate_node(const zip_reader::stat_t &st);
    int find_child(const std::string &name, int parent);
    bool enum_children(int parent, string_list_t &result, bool recursive);

private:
    zip_reader_ptr m_reader;
    std::vector<node> m_tree;
};

SGE_VM_END

#endif // SGE_VM_ZIP_FS_HPP
