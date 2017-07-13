#ifndef __MANAGER_H
#define __MANAGER_H

#include <vector>
#include "core.h"
#include "user.h"
#include "file.h"


namespace  z
{

	/**
	* �ļ�ϵͳ���ݽṹ
	*/
	class Manager
	{
	public:
		// ����ģʽ
		static Manager& Instance()
		{
			static Manager manager;
			return manager;
		}
		Manager(const Manager&) = delete;
		Manager&operator=(const Manager&) = delete;


		/**
		 * \brief �û�����
		 */
		bool login(std::string name, std::string password);
		bool login(User user);

		bool signup(const std::string&name, const std::string&password);
		bool signup(User user);

		/**
		 * \brief ��ʽ������Ӳ��
		 */
		void format();

		/**
		 * \ brief ���ú���
		 */
		bool exist(const std::vector<std::string>& path) const;
		bool exist(const std::string path) const;
		static std::vector<std::string> cmd_split(const std::string & cmd);

		/**
		 * \brief �ļ�����
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


		User current_user_;					// ��ǰ��¼���û�

	private:
		// ����
		Manager() { }

		char * getFileBlockByInodeId(uint16_t inode_id, uint32_t index) const;

		/**
		 * \breif �ӵ�ǰĿ¼��Ѱ���ļ�
		 */
		bool find(DirBlock& dir, const std::string&file, uint16_t & inode_id) const;
		/**
		* \breif �Ӹ�Ŀ¼��ʼѰ��
		*/
		bool search(const std::vector<std::string> & plist, uint16_t & inode_id) const;

		/**
		 * \breif ɾ��Ŀ¼��ָ���ļ����ļ���(�ݹ�ɾ��)
		 */
		void del(const DirBlock&dir, const std::string & name);

		bool copy(const std::string & src, const std::string& dst);

		
		std::vector<User> users_;			// ���е��û���Ϣ

		// ������
		char super_block_[512];

		// Ŀ¼��������
		VDisk *data_area_ = nullptr;

		// inode
		inode inodes_[INODE_NUM];
		VDisk *inode_area_ = nullptr;

//		int root_block_id_ = 0;
	};

}




#endif // !__MANAGER_H



