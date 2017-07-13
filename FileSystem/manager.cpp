#include <iterator>
#include <ctime>
#include "manager.h"
#include "file.h"

/////////////////////////////////////////////////////////////////////////////////
void z::Manager::format()
{
	data_area_ = new VDisk(DATA_BLOCK_NUM);
	inode_area_ = new VDisk(INODE_BLOCK_NUM);

	User root_user = { "root", "root" };
	signup(root_user);
	login(root_user);

	auto root_block_id = data_area_->alloc();
	auto root = DirBlock(data_area_->block(root_block_id));

	inodes_[0].access(0);
	inodes_[0].uid(current_user_.id());
	inodes_[0].size(0);
	inodes_[0].flag(CREATE | DIR);
	inodes_[0].ctime(time(nullptr));
	inodes_[0].mtime(time(nullptr));
	inodes_[0].blockId(0, root_block_id);

	root.add(".", 0);
	root.add("..", 0);

	mkdir("/home");
}


/////////////////////////////////////////////////////////////////////////////////
bool z::Manager::login(std::string name, std::string password)
{
	for(auto user:users_) {
		if(user.username_ == name && user.password_ == password) {
			current_user_ = user;
			return true;
		}
	}
	return false;
}

bool z::Manager::signup(const std::string &name, const std::string &password)
{
	if (name == "" || password == "") return  false;
	for (auto user : users_)
		if (user.username_ == name)
			return false;

	users_.push_back({ name, password });

	return true;
}

bool z::Manager::login(User user)
{
	return login(user.username_, user.password_);
}


bool z::Manager::signup(User user)
{
	return signup(user.username_, user.password_);
}


/////////////////////////////////////////////////////////////////////////////////
char* z::Manager::getFileBlockByInodeId(uint16_t inode_id, uint32_t index) const
{
	auto block_id = inodes_[inode_id].blockId(index);
	return  data_area_->block(block_id);
}

bool z::Manager::find(z::DirBlock& dir, const std::string& file, uint16_t& inode_id) const
{
	if (dir.empty()) return false;

	for(auto i = 0; i < 16; ++i) {
		if(file == dir.filename(i)) {
			inode_id = dir.inodeId(i);
			return true;
		}
	}

	return false;
}

bool z::Manager::search(const std::vector<std::string>& plist, uint16_t& inode_id) const
{
	inode_id = 0;
	auto block = getFileBlockByInodeId(inode_id, 0);
	auto dir_block = DirBlock(block);

	for (auto item : plist) {
		if (find(dir_block, item, inode_id)) {

			// ������ļ���
			if (inodes_[inode_id].flag() & DIR) {
				auto block_id = inodes_[inode_id].blockId(0);
				auto block = data_area_->block(block_id);
				dir_block = DirBlock(block);
			}
			// ��������ļ���
			else {
				return item == plist.back();
			}
		}
		else {
			return false;
		}
	}

	return true;
}

std::vector<std::string> z::Manager::cmd_split(const std::string& cmd)
{
	std::vector<std::string> list;
	auto last = 0;

	for (std::string::size_type i = 0; i < cmd.length(); ++i) {
		if (cmd[i] == ' ') {
			list.push_back(cmd.substr(last, i - last));
			last = i + 1;
		}
	}

	list.push_back(cmd.substr(last, cmd.length() - last));
	return list;
}


std::string z::Manager::getUsernameById(uint16_t id)
{
	for(auto user: users_) 
		if (user.id() == id)
			return user.username_;
	
	return "";
}

/////////////////////////////////////////////////////////////////////////////////
std::vector<z::File> z::Manager::ls(const std::string& path)
{
	uint16_t inode_id = 0;
	std::vector<z::File> files;

	if(search(z::Path::split(path), inode_id)) {
		auto dir_block = new DirBlock(getFileBlockByInodeId(inode_id, 0));

		for(auto i = 0; i < 16; ++i) {
			if(dir_block->filename(i) != "") {
				auto&& in = inodes_[dir_block->inodeId(i)];
				auto username = getUsernameById(in.uid());
				files.push_back(z::File(dir_block->filename(i), username, inodes_[dir_block->inodeId(i)]));
			}
		}
	}

	return files;
}

void z::Manager::del(const DirBlock& dir, const std::string& name)
{
	if (name == ".." || name == "." || name == "")
		return;

	for(auto i = 0; i < 16; ++i) {
		if(dir.filename(i) == name) {
			
			auto inode_id = dir.inodeId(i);
			auto && in = inodes_[inode_id];

			// ������ļ���
			if(in.flag() & 0x01) {
				
				// ɾ����Ŀ¼
				auto temp = DirBlock(getFileBlockByInodeId(inode_id, 0));
				for(auto j = 0; j < 16; ++j) {
					del(temp, temp.filename(j));
				}

				// ����
				data_area_->add(in.blockId(0));
			}
			// ������ļ�
			else {
				auto block_num = in.block_num();

				for(uint32_t index = 0; index < block_num; ++index) {
					data_area_->add(in.blockId(index));
				}
			}

			// ɾ�����ļ���inode
			in.flag(in.flag() & 0xfd);

			// ɾ�������ļ�Ŀ¼
			dir.filename(i, "");
			dir.inodeId(i, 0);

			return;
		}
	}
}


/**
 * \breif ��Ҫ�ݹ�ɾ��
 * \attention ��Ҫɾ�� 1. �ļ��б���е���Ϣ 2.inode 3.ʹ�õ�disk block
 */
bool z::Manager::rm(const std::string& path)
{
	auto list = z::Path::split(path);
	auto back = list.back();

	list.pop_back();

	// �ҵ����ļ��ĸ�Ŀ¼
	uint16_t inode_id = 0;
	if(search(list, inode_id)) {
		auto dir = DirBlock(getFileBlockByInodeId(inode_id, 0));
		del(dir, back);
		return true;
	}
	return false;
}

bool z::Manager::copy(const std::string& src, const std::string& dst)
{
	return false;
}


bool z::Manager::cp(const std::string& src, const std::string& dst)
{
	// ���Դ�ļ���������Ϊ����
	uint16_t inode_id = 0;
	if (!search(Path::split(src), inode_id)) return false;

	auto && in = inodes_[inode_id];

	// ���Ŀ�겻����, ����
	if(in.flag() & 0x01) {
		if(!exist(dst)) {
			mkdirs(dst);
		}

		//
		copy(src, dst);

		//
		auto dir = DirBlock(getFileBlockByInodeId(inode_id, 0));
		for(auto i = 0; i < 16; ++i) {
			if(dir.filename(i) != "") {
				cp(src + "/" + dir.filename(i), dst + "/" + dir.filename(i));
			}
		}
	}
	else {
		if (!exist(dst)) {
			touch(dst);
		}
		copy(src, dst);
	}

	return false;
}

bool z::Manager::mkdir(const std::vector<std::string> path, const std::string file)
{
	auto new_inode_id = 0;

	// ����inode��iΪ������inode��id
	for (auto i = 0; i < INODE_NUM; ++i) {
		if (!(inodes_[i].flag() & 0x02)) {
			inodes_[i].access(0);
			inodes_[i].uid(current_user_.id());
			inodes_[i].size(0);
			inodes_[i].flag(CREATE | DIR);
			inodes_[i].ctime(time(nullptr));
			inodes_[i].mtime(time(nullptr));

			new_inode_id = i;

			break;
		}
	}

	if (new_inode_id == INODE_NUM)
		return false;

	uint16_t parent_inode_id = 0;
	if (search(path, parent_inode_id)) {
		auto dir = DirBlock(getFileBlockByInodeId(parent_inode_id, 0));

		// ����½����ļ���Ŀ¼
		dir.add(file.c_str(), new_inode_id);

		// ������̿ռ�
		auto new_block_id = data_area_->alloc();
		memset(data_area_->block(new_block_id), 0, BLOCK_SIZE);

		// ����Ŀ¼��
		inodes_[new_inode_id].blockId(0, new_block_id);
		auto new_dir = DirBlock(data_area_->block(new_block_id));
		new_dir.add(".", new_inode_id);
		new_dir.add("..", parent_inode_id);

		return true;
	}

	return false;
}

bool z::Manager::mkdir(const std::string& path, const std::string file)
{
	return mkdir(Path::split(path), file);
}

bool z::Manager::mkdir(const std::string& path)
{
	if (exist(path)) return true;

	auto list = Path::split(path);
	auto back = list.back();
	list.pop_back();

	// ��������ڸ�Ŀ¼�����
	if (!exist(list)) return false;

	return mkdir(list, back);
}

bool z::Manager::mkdirs(const std::string& path)
{
	return mkdirs(Path::split(path));
}

bool z::Manager::mkdirs(const std::vector<std::string>& path)
{
	std::string temp_path = "";
	for (std::string::size_type i = 0; i < path.size(); ++i) {
		temp_path += "/" + path[i];

		if (!exist(temp_path)) {
			if (!mkdir(temp_path)) return false;
		}
	}
	return true;
}


bool z::Manager::mkfile(const std::string& path, const std::string& file)
{
	return mkfile(Path::split(path), file);
}

bool z::Manager::mkfile(const std::vector<std::string>& path, const std::string& file)
{
	int new_inode_id = 0;

	for (auto i = 0; i < INODE_NUM; ++i) {
		if (!(inodes_[i].flag() & 0x02)) {
			inodes_[i].access(0);
			inodes_[i].uid(current_user_.id());
			inodes_[i].size(0);
			inodes_[i].flag(CREATE | FILE);
			inodes_[i].ctime(time(nullptr));
			inodes_[i].mtime(time(nullptr));

			new_inode_id = i;

			break;
		}
	}

	if (new_inode_id == INODE_NUM)
		return false;

	uint16_t parent_inode_id = 0;
	if (search(path, parent_inode_id)) {
		auto dir = DirBlock(getFileBlockByInodeId(parent_inode_id, 0));
		dir.add(file.c_str(), new_inode_id);

		return true;
	}

	return false;
}

bool z::Manager::exist(const std::vector<std::string>& path) const
{
	uint16_t inode_id;
	return search(path, inode_id);
}

bool z::Manager::exist(const std::string path) const
{
	uint16_t inode_id;
	return search(Path::split(path), inode_id);
}

bool z::Manager::rename(const std::string& path, const std::string& filename)
{
	auto list = Path::split(path);
	auto back = list.back();
	list.pop_back();

	uint16_t inode_id = 0;
	if(search(list, inode_id)) {
		auto dir = DirBlock(getFileBlockByInodeId(inode_id, 0));

		for(auto i = 0; i < 16; ++i) {
			if(dir.filename(i) == back) {
				dir.filename(i, filename.c_str());

				return true;
			}
		}
	}

	return false;
}


void z::Manager::touch(const std::string & path)
{
	// ������ڣ����޶���
	if (exist(path)) return;

	auto list = Path::split(path);
	auto filename = list.back();
	list.pop_back();

	// �����ļ�Ŀ¼
	mkdirs(list);


	// �����ļ�
	mkfile(list, filename);
}

bool z::Manager::write(const std::string path, char* buf)
{
	uint16_t inode_id;
	if(search(Path::split(path), inode_id)) {
		auto&&in = inodes_[inode_id];

		// ������ļ��У�����
		if (in.flag() & 0x01) return false;

		auto len = strlen(buf);
		in.size(len);

		for(uint32_t i = 0; i < in.block_num(); ++i) {
			auto block_id = data_area_->alloc();
			in.blockId(i, block_id);

			auto temp_block = data_area_->block(in.blockId(i));
			memcpy(temp_block, buf + i * 512, len >= 512 ? 512 : len);
			len -= 512;
		}
	}
	return true;
}

bool z::Manager::read(const std::string path, char* buf, uint32_t size)
{
	uint16_t inode_id;
	if (search(Path::split(path), inode_id)) {
		auto&&in = inodes_[inode_id];

		// ������ļ��У�����
		if (in.flag() & 0x01) return false;

		auto len = strlen(buf);
		in.size(len);

		if(in.size() > size) {
			auto read_block = size / BLOCK_SIZE + size % BLOCK_SIZE ? 1 : 0;
			for (auto i = 0; i < read_block; ++i) {
				memcpy(buf + i * BLOCK_SIZE, data_area_->block(in.blockId(i)), size > BLOCK_SIZE ? BLOCK_SIZE:size);
				size -= BLOCK_SIZE;
			}
		}
		else {
			auto size_ = in.size();
			for (uint32_t i = 0; i < in.block_num(); ++i) {
				memcpy(buf + i * BLOCK_SIZE, data_area_->block(in.blockId(i)), size_ > BLOCK_SIZE ? BLOCK_SIZE : size_);
				size_ -= BLOCK_SIZE;
			}
		}
	}

	return false;
}

std::string z::Manager::read(const std::string path)
{
	uint16_t inode_id;
	if (search(Path::split(path), inode_id)) {
		auto&&in = inodes_[inode_id];

		// ������ļ��У�����
		if (in.flag() & 0x01) return "";

		auto size = in.size();
		auto buf = new char[size + 1];
		memset(buf, 0, size + 1);

		for(uint32_t i = 0; i < in.block_num(); ++i) {
			memcpy(buf + i * BLOCK_SIZE, data_area_->block(in.blockId(i)), size >= 512 ? 512 : size);
			size -= 512;
		}

		return buf;
	}

	return "";
}


