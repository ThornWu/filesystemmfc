#include "core.h"

bool z::Path::isFullPath(const std::string& path)
{
	return path[0] == '/';
}


std::string z::Path::join(const std::string& path)
{
	path_ = parser(path_ + "/" + path);
	return path_;
}


std::vector<std::string> z::Path::split(const std::string& path)
{
	auto temp_path = parser(path);

	// ʹ��'/'�ָ�·��
	std::vector<std::string> list;

	auto last = 0;
	for (auto i = 0; i < temp_path.length(); ++i) {
		if (temp_path[i] == '/') {
			list.push_back(temp_path.substr(last, i - last));
			last = i + 1;
		}
	}
	list.push_back(temp_path.substr(last, temp_path.length() - last));

	return list;
}

std::vector<std::string> z::Path::__split__(const std::string& path)
{
	auto temp_path = path;
	remove_spaces(temp_path);

	// ʹ��'/'�ָ�·��
	std::vector<std::string> list;

	auto last = 0;
	for (auto i = 0; i < temp_path.length(); ++i) {
		if (temp_path[i] == '/') {
			if (i != last)
				list.push_back(temp_path.substr(last, i - last));
			last = i + 1;
		}
	}
	list.push_back(temp_path.substr(last, temp_path.length() - last));

	return list;
}

std::string z::Path::parser(const std::string& path)
{
	auto pathList = __split__(path);

	// ȥ�� "."
	for (auto iter = pathList.begin() + (isFullPath(path) ? 0 : 1); iter != pathList.end();)
		(*iter == ".") ? iter = pathList.erase(iter) : ++iter;

	// ȥ�� ".."
	for (auto iter = pathList.begin(); iter != pathList.end();)
		(*iter == "..") ? iter = pathList.erase(iter), iter = pathList.erase(--iter) : ++iter;

	// ���Ϊ·��
	std::string rpath;

	for(auto item: pathList) {
		rpath += "/" + item;
	}

	if (rpath.empty())
		rpath = "/";

	return isFullPath(path) ? rpath : rpath.substr(1, rpath.length() - 1);
}

/////////////////////////////////////////////////////////////////////////////////
bool z::DirBlock::add(const char* dir, int inode_id) const
{
	for(auto i = 0; i < 16; ++i) {
		if(filename(i) == "") {
			filename(i, dir);
			inodeId(i, inode_id);
			return true;
		}
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////////////
void z::remove_spaces(std::string& str)
{
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}