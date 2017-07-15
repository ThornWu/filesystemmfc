#include <iterator>
#include <ctime>
#include "manager.h"
#include "file.h"

/////////////////////////////////////////////////////////////////////////////////
bool z::Manager::format()
{
	data_area_ = new VDisk(DATA_BLOCK_NUM);
	inode_area_ = new VDisk(INODE_BLOCK_NUM);

	User root_user = { "root", "root" };
	users_.clear();
	signup(root_user);
	login(root_user);

	auto root_block_id = data_area_->alloc();
	auto root = DirBlock(data_area_->block(root_block_id));

	for(auto i = 0; i < INODE_NUM; ++i) {
		memset(reinterpret_cast<char *>(&inodes_[i]), 0, INODE_SIZE);
	}

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
	mkdir("/etc");

	save_users();		// 保存用户信息

	// 保存到虚拟盘
	vdisk_.close();
	vdisk_.open("D:/d.fs", std::ios::in | std::ios::out | std::ios::binary);
	if (!vdisk_.is_open())
		return false;

	// 写入超级快
	vdisk_.seekp(0, std::ios::beg);
	vdisk_.write(super_block_, BLOCK_SIZE);

	// 写入inode节点
	vdisk_.seekp(BLOCK_SIZE, std::ios::beg);
	vdisk_.write(reinterpret_cast<char *>(inodes_), INODE_BLOCK_NUM * BLOCK_SIZE);

	// 写入数据和目录
	vdisk_.seekp(BLOCK_SIZE + INODE_BLOCK_NUM * BLOCK_SIZE, std::ios::beg);
	vdisk_.write(data_area_->data_, DATA_BLOCK_NUM * BLOCK_SIZE);

	vdisk_.close();
	return true;
}


/////////////////////////////////////////////////////////////////////////////////
bool z::Manager::login(std::string name, std::string password)
{
	// 匹配登录
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


void z::Manager::save_users()
{
	z::File user_conf("/etc/user.conf", File::in | File::out | File::trunc);
	for (auto user : users_) {
		if (user_conf.is_open()) {
			auto user_string = user.to_string();
			auto buf = user_string.c_str();
			user_conf.write(buf, user_string.size());
		}
	}

	user_conf.close();
}

bool z::Manager::load_users()
{
	// 加载用户信息
	z::File user_conf("/etc/user.conf", z::File::in);
	if (!user_conf.is_open()) {
		std::cout << "打开用户配置文件失败" << std::endl;
		return false;
	}

	users_.clear();

	auto buf = new char[user_conf.size() + 1];
	memset(buf, 0, user_conf.size() + 1);
	user_conf.read(buf, user_conf.size());

	//正则
	std::regex r("<user>\n\t<userid>(\\w+)</userid>\n\t<username>(\\w+)</username>\n\t<password>(\\w+)</password>\n</user>");
	std::smatch res;
	auto str = std::string(buf);
	while (std::regex_search(str, res, r)) {
		users_.push_back({ res[1], res[2], res[3] });
		str = res.suffix();
	}

	return true;
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

			// 如果是文件夹
			if (inodes_[inode_id].flag() & DIR) {
				auto block_id = inodes_[inode_id].blockId(0);
				auto block = data_area_->block(block_id);
				dir_block = DirBlock(block);
			}
			// 如果不是文件夹
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

			// 如果是链接文件
			if (in.link_num() > 0) {
				in.link_num(in.link_num() - 1);

				dir.filename(i, "");
				dir.inodeId(i, 0);
				return;
			}

			// 如果是文件夹
			if(in.flag() & 0x01) {
				
				// 删除子目录
				auto temp = DirBlock(getFileBlockByInodeId(inode_id, 0));
				for(auto j = 0; j < 16; ++j) {
					del(temp, temp.filename(j));
				}

				// 回收
				data_area_->add(in.blockId(0));
			}
			// 如果是文件
			else {
				auto block_num = in.block_num();

				for(uint32_t index = 0; index < block_num; ++index) {
					data_area_->add(in.blockId(index));
				}
			}

			// 删除该文件的inode
			in.flag(in.flag() & 0xfd);

			// 删除这条文件目录
			dir.filename(i, "");
			dir.inodeId(i, 0);

			return;
		}
	}
}


/**
 * \breif 需要递归删除
 * \attention 需要删除 1. 文件列表的中的信息 2.inode 3.使用的disk block
 */
bool z::Manager::rm(const std::string& path)
{
	auto list = z::Path::split(path);
	auto back = list.back();

	list.pop_back();

	// 找到该文件的父目录
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
	uint16_t src_inode_id = 0;
	if (!search(Path::split(src), src_inode_id)) return false;

	auto&& src_inode = inodes_[src_inode_id];

	// 文件
	if(src_inode.is_file()) {
		touch(dst);

		z::File src_file(src, File::in);
		auto file_size = src_file.size();
		auto buf = new char[file_size + 1];
		memset(buf, 0, file_size + 1);
		src_file.read(buf, file_size);
		src_file.close();

		z::File dst_file(dst, File::out);
		dst_file.write(buf, file_size);
		dst_file.close();
	}
	else {
		auto dir = DirBlock(getFileBlockByInodeId(src_inode_id, 0));
		for(auto i = 0; i < 16; ++i) {
			if(dir.filename(i) != "" && dir.filename(i) != "." && dir.filename(i) != "..") {
				copy(src + "/" + dir.filename(i), dst + "/" + dir.filename(i));
			}
		}
	}

	return true;
}


bool z::Manager::cp(const std::string& src, const std::string& dst)
{
	return copy(src, dst);
}
// src 文件或目录
// dst 必须是目录
// \ 将新的目录的id指向inode
// \ 删除原先的目录
bool z::Manager::mv(const std::string& src, const std::string& dst)
{
	if (!exist(src)) return false;


	uint16_t src_inode_id = 0;
	auto src_path_list = Path::split(src);
	auto src_name = src_path_list.back(); src_path_list.pop_back();  // 获取源文件(夹)名


	// 父目录
	if (!search(src_path_list, src_inode_id)) return false;
	auto src_dir = DirBlock(getFileBlockByInodeId(src_inode_id, 0));
	
	// 从父目录中删除源文件(夹)目录，并得到源文件(夹)的inode id
	auto src_path_inode_id = 0;
	for(auto i = 0;i < 16; ++i) {
		if (src_name == src_dir.filename(i)) {
			src_dir.filename(i, "");
			src_path_inode_id = src_dir.inodeId(i);
			src_dir.inodeId(i, 0);
		}
	}

	// 目的文件夹不存在则创建
	if (!exist(Path::split(dst))) mkdirs(dst);

	// 找到目标文件夹目录
	uint16_t dst_inode_id = 0;
	if (!search(Path::split(dst), dst_inode_id)) return false;
	auto dst_dir = DirBlock(getFileBlockByInodeId(dst_inode_id, 0));

	// 在目的
	dst_dir.add(src_name.c_str(), src_path_inode_id);

	return true;
}

bool z::Manager::mkdir(const std::vector<std::string> path, const std::string file)
{
	auto new_inode_id = 0;

	// 创建inode，i为创建的inode的id
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

		// 添加新建的文件的目录
		dir.add(file.c_str(), new_inode_id);

		// 分配磁盘空间
		auto new_block_id = data_area_->alloc();
		memset(data_area_->block(new_block_id), 0, BLOCK_SIZE);

		// 设置目录块
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

	// 如果不存在父目录则错误
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

void z::Manager::clear(const std::string& cs)
{
	uint16_t inode_id = 0;
	if(!search(Path::split(cs), inode_id))
		return;

	auto&& in = inodes_[inode_id];

	// 清空文件
	for(uint32_t i = 0; i < in.block_num(); ++i) {
		data_area_->add(in.blockId(i));
	}
	in.size(0);
}

bool z::Manager::rename(const std::string& path, const std::string& filename) const
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
	// 如果存在，则无动作
	if (exist(path)) return;

	auto list = Path::split(path);
	auto filename = list.back();
	list.pop_back();

	// 创建文件目录
	mkdirs(list);


	// 创建文件
	mkfile(list, filename);
}

int z::Manager::write(const std::string& path, const char* buf, uint32_t start, uint32_t count)
{
	uint16_t inode_id;
	if (!search(Path::split(path), inode_id))
		return 0;

	// 获取文件的inode节点
	auto&&in = inodes_[inode_id];

	// 
	auto old_blcoks = in.block_num();
	auto new_blocks = (start + count) / BLOCK_SIZE + ((start + count) % BLOCK_SIZE ? 1 : 0);

	// 如果原本分配的磁盘不够则分配
	if (old_blcoks < new_blocks) {
		in.size(start + count);
		for (auto i = old_blcoks; i < new_blocks; ++i) {
			in.blockId(i, data_area_->alloc());
		}
	}

	in.size(std::max(in.size(), start + count));

	auto the_first_write_block = true;
	uint32_t write_count = 0;

	// 需要写入的数据块数目
	auto write_blocks = count / BLOCK_SIZE + (count % BLOCK_SIZE ? 1 : 0);

	// 从指定位置开始写入
	for (auto i = 0; i < write_blocks; ++i) {
		auto current_block = data_area_->block(in.blockId(start / BLOCK_SIZE + i));

		// 第一块
		if (the_first_write_block) {
			the_first_write_block = !the_first_write_block;
			auto start_pos = start % BLOCK_SIZE;
			write_count = count > (BLOCK_SIZE - start_pos) ? BLOCK_SIZE - start_pos : count;
			memcpy(current_block + start_pos, buf, write_count);
			count -= write_count;
		}
		else {
			memcpy(current_block, buf + write_count, count > BLOCK_SIZE ? BLOCK_SIZE : count);
			write_count += count > BLOCK_SIZE ? BLOCK_SIZE : count;
			count -= BLOCK_SIZE;
		}
	}

	return write_count;
}

int z::Manager::read(const std::string& path, char* const buf, uint32_t start, uint32_t count)
{
	uint16_t inode_id;
	if (!search(Path::split(path), inode_id))
		return 0;

	// 获取inode节点
	auto&&in = inodes_[inode_id];

	uint32_t read_count = 0;
	auto read_first_block = true;
	auto read_block_num = count / BLOCK_SIZE + (count % BLOCK_SIZE ? 1 : 0);
	auto start_block_id = start / BLOCK_SIZE;

	for(auto i = 0; i < read_block_num; ++i) {
		auto current_block = data_area_->block(in.blockId(start_block_id + i));

		if(read_first_block) {
			read_first_block = !read_first_block;
			auto start_pos = start % BLOCK_SIZE;
			read_count = count >(BLOCK_SIZE - start_pos) ? BLOCK_SIZE - start_pos : count;
			memcpy(buf, current_block + start_pos, read_count);
			count -= read_count;
		}
		else {
			memcpy(buf + read_count, current_block, count > BLOCK_SIZE ? BLOCK_SIZE : count);
			read_count += count > BLOCK_SIZE ? BLOCK_SIZE : count;
			count -= BLOCK_SIZE;
		}
	}

	return read_count;
}

/**
 * \src 老文件
 * \dst 新文件
 */
bool z::Manager::ln(const std::string& src, const std::string& dst)
{
	// 被链接文件不存在则失败
	uint16_t src_inode_id = 0;
	if (!search(Path::split(src), src_inode_id)) return false;

	// 链接文件已经创建则失败
	if (exist(dst)) return false;

	auto list = Path::split(dst);
	auto dst_name = list.back();

	list.pop_back();

	// 找到新建链接的父目录
	uint16_t dst_parent_inode_id = 0;
	if (!search(list, dst_parent_inode_id)) return false;

	auto dir = DirBlock(getFileBlockByInodeId(dst_parent_inode_id, 0));
	dir.add(dst_name.c_str(), src_inode_id);

	auto&& src_inode = inodes_[src_inode_id];
	src_inode.link_num(src_inode.link_num() + 1);

	return true;
}
