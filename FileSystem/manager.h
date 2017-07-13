#ifndef __MANAGER_H
#define __MANAGER_H

#include <vector>
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


		/**
		 * \brief 用户管理
		 */
		bool login(std::string name, std::string password);
		bool login(User user);

		bool signup(const std::string&name, const std::string&password);
		bool signup(User user);

		/**
		 * \brief 格式化整个硬盘
		 */
		void format();

		/**
		 * \ brief 公用函数
		 */
		bool exist(const std::vector<std::string>& path) const;
		bool exist(const std::string path) const;
		static std::vector<std::string> cmd_split(const std::string & cmd);

		/**
		 * \brief 文件操作
		 */
		std::vector<File> ls(const std::string & path);
		bool rm(const std::string& path);
		bool cp(const std::string& src, const std::string & dst);
		bool mv(const std::string & src, const std::string & dst);

		bool mkdir(const std::vector<std::string> path, const std::string file);
		bool mkdir(const std::string& path, const std::string file);
		bool mkdir(const std::string& path);
		bool mkdirs(const std::string& path);
		bool mkdirs(const std::vector<std::string>& path);
		bool mkfile(const std::string& path, const std::string& file);
		bool mkfile(const std::vector<std::string>& path, const std::string& file);
		
		bool rename(const std::string &path, const std::string &filename);
		void touch(const std::string & path);

		bool write(const std::string path, char *buf);
		bool read(const std::string path, char *buf, uint32_t size);
		std::string read(const std::string path);

		std::string getUsernameById(uint16_t id);


		User current_user_;					// 当前登录的用户

	private:
		// 隐藏
		Manager() { }

		char * getFileBlockByInodeId(uint16_t inode_id, uint32_t index) const;

		/**
		 * \breif 从当前目录中寻找文件
		 */
		bool find(DirBlock& dir, const std::string&file, uint16_t & inode_id) const;
		/**
		* \breif 从根目录开始寻找
		*/
		bool search(const std::vector<std::string> & plist, uint16_t & inode_id) const;

		/**
		 * \breif 删除目录下指定文件或文件夹(递归删除)
		 */
		void del(const DirBlock&dir, const std::string & name);

		bool copy(const std::string & src, const std::string& dst);

		
		std::vector<User> users_;			// 所有的用户信息

		// 超级块
		char super_block_[512];

		// 目录和数据区
		VDisk *data_area_ = nullptr;

		// inode
		inode inodes_[INODE_NUM];
		VDisk *inode_area_ = nullptr;

//		int root_block_id_ = 0;
	};

}




#endif // !__MANAGER_H



