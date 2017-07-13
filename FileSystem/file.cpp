#include "file.h"

z::File::File(const std::string& filename, const std::string& username, const inode& in)
{
	filename_ = filename;
	username_ = username;

	access_ = in.access();
	uid_ = in.uid();
	size_ = in.size();
	flag_ = in.flag();
	ctime_ = in.ctime();
	mtime_ = in.mtime();

	auto blocks = size_ / 512;
	if (size_ % 512) blocks += 1;

	for (auto i = 0; i < blocks; ++i) {
		blocks_.push_back(in.blockId(i));
	}
}

z::File::File(const std::string& filename)
{

}

bool z::File::open(std::string filename)
{
	//	if (!exist())
	//		create();

	return false;
}

bool z::File::read()
{
	return false;
}

bool z::File::write(std::string content)
{
	return false;
}

bool z::File::exist()
{
	return false;
}


bool z::File::create(const char *path)
{
	std::string a;

	return false;
}


