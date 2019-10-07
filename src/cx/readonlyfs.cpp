//
//
#include <cx/readonlyfs.hpp>

CX_BEGIN

// ReadonlyNativeFS

ReadonlyNativeFS::ReadonlyNativeFS(void)
{
}

ReadonlyNativeFS::~ReadonlyNativeFS(void)
{
}

bool ReadonlyNativeFS::exists(const char *path)
{
}

ReadonlyFS::Dir *ReadonlyNativeFS::openDir(const char *dirname)
{
}

void ReadonlyNativeFS::closeDir(Dir *dir)
{
}

bool ReadonlyNativeFS::readDir(Dir *dir, Dent *ent)
{
}

int64_t ReadonlyNativeFS::fileSize(const char *filename)
{
}

int64_t ReadonlyNativeFS::fileSize(File *file)
{
}

File *ReadonlyNativeFS::openFile(const char *filename)
{
}

void ReadonlyNativeFS::closeFile(File *file)
{
}

int64_t ReadonlyNativeFS::seekFile(File *file, int64_t offset, SeekMode sm)
{
}

int64_t ReadonlyNativeFS::readFile(File *file, void *buf, int64_t len)
{
}

// ReadonlyZipFS

ReadonlyZipFS::ReadonlyZipFS(void)
{
}

ReadonlyZipFS::~ReadonlyZipFS(void)
{
}

bool ReadonlyZipFS::exists(const char *path)
{
}

ReadonlyFS::Dir *ReadonlyZipFS::openDir(const char *dirname)
{
}

void ReadonlyZipFS::closeDir(Dir *dir)
{
}

bool ReadonlyZipFS::readDir(Dir *dir, Dent *ent)
{
}

int64_t ReadonlyZipFS::fileSize(const char *filename)
{
}

int64_t ReadonlyZipFS::fileSize(File *file)
{
}

File *ReadonlyZipFS::openFile(const char *filename)
{
}

void ReadonlyZipFS::closeFile(File *file)
{
}

int64_t ReadonlyZipFS::seekFile(File *file, int64_t offset, SeekMode sm)
{
}

int64_t ReadonlyZipFS::readFile(File *file, void *buf, int64_t len)
{
}

SGE_END

