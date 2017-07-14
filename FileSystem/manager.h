#ifndef __MANAGER_H
#define __MANAGER_H

#include <vector>
#include <fstream>
#include <regex>
#include "core.h"
#include "user.h"
#include "file.h"

namespace  z
{

	/**
	* 文件系统数据结构
	*/
	class Manager
	{
	public:
		// 单例模式
		static Manager& Instance()
		{
			static Manager manager;
			return manager;
		}
		Manager(const Manager&) = delete;
		Manager&operator=(const Manager&) = delete;
		~Manager() { }


		void destory()
		{
			save_users();

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
		}

		/**
		 * \brief 用户管理
		 */
		bool login(std::string name, std::string password);
		bool login(User user);

		bool signup(const std::string&name, const std::string&password);
		bool signup(User user);

		inode& getInode(uint16_t id) { return inodes_[id]; }

		/**
		 * \brief 格式化整个硬盘
		 */
		bool format();

		/**
		 * \ brief 公用函数
		 */
		bool exist(const std::vector<std::string>& path) const;
		bool exist(const std::string path) const;
		void clear(const std::string& cs);
		static std::vector<std::string> cmd_split(const std::string & cmd);

		/**
		 * \brief 文件操作
		 */
		std::vector<File> ls(const std::string & path);
		bool rm(const std::string& path);
		bool cp(const std::string& src, const std::string & dst);
		bool mv(const std::string & src, const std::string & dst);
		// 硬链接
		bool ln(const std::string& src, const std::string& dst);

		bool mkdir(const std::vector<std::string> path, const std::string file);
		bool mkdir(const std::string& path, const std::string file);
		bool mkdir(const std::string& path);
		bool mkdirs(const std::string& path);
		bool mkdirs(const std::vector<std::string>& path);
		bool mkfile(const std::string& path, const std::string& file);
		bool mkfile(const std::vector<std::string>& path, const std::string& file);
		
		bool rename(const std::string &path, const std::string &filename) const;
		void touch(const std::string & path);

		/**
		* \breif 从指定位置开始读取或写入内容
		* \attention 适用：追加内容、修改其中一部分内容
		* \attention 不适用：覆盖重写
		* \return [写入/读取]的字节数
		*/
		int write(const std::string& path, const char *buf, uint32_t start, uint32_t count);
		int read(const std::string& path, char * const buf, uint32_t start, uint32_t count);


		std::string getUsernameById(uint16_t id);
		/**
		* \breif 从根目录开始寻找
		*/
		bool search(const std::vector<std::string> & plist, uint16_t & inode_id) const;
		bool load_users();

		User current_user_;					// 当前登录的用户

	private:
		// 隐藏
		Manager() { refresh(); }

		bool refresh()
		{
			vdisk_.open("D:/d.fs", std::ios::in | std::ios::out | std::ios::binary);
			if (!vdisk_.is_open()) {
				std::cout << "打开文件失败" << std::endl;
				return false;
			}

			// 获取文件大小
			vdisk_.seekg(0, std::ios::end);
			if(vdisk_.tellg() < BLOCK_SIZE + BLOCK_SIZE * INODE_BLOCK_NUM + BLOCK_SIZE * DATA_BLOCK_NUM) {
				std::cout << "文件不对" << std::endl;
				return false;
			}

			vdisk_.seekg(0, std::ios::beg);
			vdisk_.read(super_block_, BLOCK_SIZE);

			vdisk_.seekg(BLOCK_SIZE, std::ios::beg);
			vdisk_.read(reinterpret_cast<char *>(inodes_), INODE_BLOCK_NUM * BLOCK_SIZE);

			data_area_ = new VDisk(new char[DATA_BLOCK_NUM * BLOCK_SIZE], DATA_BLOCK_NUM);
			vdisk_.seekg(BLOCK_SIZE + INODE_BLOCK_NUM * BLOCK_SIZE, std::ios::beg);
			vdisk_.read(data_area_->data_, DATA_BLOCK_NUM * BLOCK_SIZE);

//			read_users();

			return true;
		}

		void save_users();


		char * getFileBlockByInodeId(uint16_t inode_id, uint32_t index) const;

		/**
		 * \breif 从当前目录中寻找文件
		 */
		bool find(DirBlock& dir, const std::string&file, uint16_t & inode_id) const;

		/**
		 * \breif 删除目录下指定文件或文件夹(递归删除)
		 */
		void del(const DirBlock&dir, const std::string & name);

		bool copy(const std::string & src, const std::string& dst);


		std::fstream vdisk_;
		
		std::vector<User> users_;			// 所有的用户信息

		// 超级块
		char super_block_[512] = "z";

		// 目录和数据区
		VDisk *data_area_ = nullptr;

		// inode
		inode inodes_[INODE_NUM];
		VDisk *inode_area_ = nullptr;

//		int root_block_id_ = 0;
	};

}




#endif // !__MANAGER_H



