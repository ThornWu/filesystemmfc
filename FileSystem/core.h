#ifndef __CORE_H
#define __CORE_H

#include <iostream>
#include <algorithm>
#include <vector>


#define VIRTUAL_DISK_SIZE	(1024 * 1024)	//B
#define BLOCK_SIZE			512				//B

#define INODE_BLOCK_NUM		32
#define INODE_SIZE			128
#define INODE_NUM			(BLOCK_SIZE/INODE_SIZE)*INODE_BLOCK_NUM

#define DATA_BLOCK_NUM		512
#define BLOCK_NUM			(INODE_BLOCK_NUM + DATA_BLOCK_NUM + 1)

#define NICFREE				50

namespace z
{
	/**
	 * \breif 处理路径
	 */
	class Path
	{
	public:
		explicit Path(const std::string& path) { path_ = parser(path); }

		/**
		 * \breif 获取和设置路径值
		 */
		std::string val() const { return path_; }
		void val(const std::string& val) { path_ = val; }

		/**
		 * \breif 解析组合路径
		 * \attention 去掉"..", "//"
		 */
		std::string join(const std::string& path);

		/**
		 * \breif 判断是否是绝对路径
		 */
		static bool isFullPath(const std::string& path);

		/**
		 * \breif 解析并处理路径
		 */
		static std::string parser(const std::string& path);

		/**
		 * \brief 分割文件路径
		 * \param path ，文件路径
		 * \return 目录的集合
		 */
		static std::vector<std::string> split(const std::string & path);

	private:
		static std::vector<std::string> __split__(const std::string & path);
		std::string path_;
	};
	
	/**
	 * \breif 删除字符串中的空格
	 */
	void remove_spaces(std::string& str);


	// 128 
	class inode
	{
	public:
		inode() = default;
#define __OFT__(type, offset)	(*(type *)(&data_[offset]))

#define ACCESS				__OFT__(uint8_t,	0)
#define UID					__OFT__(uint16_t,	1)
#define SIZE				__OFT__(uint32_t,	3)
#define FLAG				__OFT__(uint8_t,	7)
#define CTIME				__OFT__(uint64_t,	8)
#define MTIME				__OFT__(uint64_t,	16)
#define LINK_NUM			__OFT__(uint16_t,	24)

		// 用户ID
		uint16_t uid() const { return UID; }
		void uid(uint16_t uid) { UID = uid; }

		// 访问权限
		uint8_t access() const { return ACCESS; }
		void access(uint8_t access) { ACCESS = access; }

		// 文件大小
		uint32_t size() const { return SIZE; }
		void size(uint32_t size) { SIZE = size; }

		uint32_t block_num() const { return (size() / BLOCK_SIZE + (size() % BLOCK_SIZE ? 1 : 0)); }

		// 文件标志
		uint8_t flag() const { return FLAG; }
		void flag(uint8_t flag) { FLAG = flag; }

		// 文件创建时间
		uint64_t ctime() const { return CTIME; }
		void ctime(uint64_t ctime) { CTIME = ctime; }

		// 文件最后修改时间
		uint64_t mtime() const { return MTIME; }
		void mtime(uint64_t mtime) { MTIME = mtime; }

		// 文件的连接数
		uint16_t link_num() const { return LINK_NUM; }
		void link_num(int value) { LINK_NUM = value; }

		// 文件占用的磁盘块
		uint16_t blockId(uint32_t offset) const { return __OFT__(uint16_t, (32 + offset * 2)); }
		void blockId(uint32_t offset, uint16_t value) { __OFT__(uint16_t, (32 + offset * 2)) = value; }


		/**
		 * \breif 判断
		 */
		bool empty() const { return !!size(); }
		bool is_file() const { return !(flag() & 0x01); }
		bool is_link() const { return flag() & 0x04; }

#undef LINK_NUM
#undef MTIME
#undef CTIME
#undef FLAG
#undef SIZE
#undef UID
#undef ACCESS
#undef __OFT__


	private:
		char data_[128] = { 0 };
	};

	/**
	 * \brief 文件目录
	 */
	class DirBlock
	{
	public:

#define __OFT__(type, offset)	(*(reinterpret_cast<type *>(&data_[offset])))
#define FILENAME(_index_)		(data_ + _index_ * 32)
#define INODE_ID(_index_)		__OFT__(uint16_t, (index * 32 + 30))

		DirBlock() = default;
		explicit DirBlock(char * data) : data_(data) { }
		DirBlock(const DirBlock&) = default;
		DirBlock& operator=(const DirBlock&) = default;

		bool empty() const { return !data_; }

		/**
		 * \brief 添加一条目录
		 */
		bool add(const char * dir, int inode_id) const;

		std::string filename(int index) const { return FILENAME(index); }
		void filename(int index, const char * filename) const { memset(FILENAME(index), 0, 30); memcpy(FILENAME(index), filename, strlen(filename) > 30 ? 30 : strlen(filename)); }
		uint16_t inodeId(int index) const { return INODE_ID(index); }
		void inodeId(int index, uint16_t value) const { INODE_ID(index) = value; }

#undef INODE_ID
#undef FILENAME
#undef __OFT__
	private:
		char * data_ = nullptr;
	};

	/**
	* \brief 磁盘管理
	*/
	class VDisk
	{
	public:
		explicit VDisk(long bn)
		{
			block_num_ = bn;
			data_ = new char[BLOCK_SIZE * block_num_];
			memset(data_, 0, BLOCK_SIZE * block_num_);

			counter(0);

			for (auto i = 0; i < block_num_; ++i) {
				add(i);
			}
		}

		explicit VDisk(char * data, long num)
		{
			block_num_ = num;
			data_ = data;
		}

		inline int counter() const { return *(reinterpret_cast<int *>(data_)); }
		inline void counter(int value) const { *(reinterpret_cast<int *>(data_)) = value; }

		inline int data(int offset) const { return *(reinterpret_cast<int *>(data_) + offset); }
		inline void data(int offset, int value) const { *(reinterpret_cast<int *>(data_) + offset) = value; }

		void add(int id) const
		{
			if (counter() >= NICFREE) {

				add_index_block(id);
				return;
			}

			counter(counter() + 1);
			data(counter(), id);
		}

		void add_index_block(int id) const
		{
			memcpy(data_ + id * BLOCK_SIZE, data_, BLOCK_SIZE);
			memset(data_, 0, BLOCK_SIZE);

			counter(1);
			data(counter(), id);
		}

		void delete_index_block(int id) const
		{
			memset(data_, 0, BLOCK_SIZE);
			memcpy(data_, data_ + id*BLOCK_SIZE, BLOCK_SIZE);
		}

		int alloc() const
		{
			if (*(reinterpret_cast<int *>(data_) + 1) == 0) {
				auto id = *(reinterpret_cast<int *>(data_));
				delete_index_block(*(reinterpret_cast<int *>(data_)));
				return id;
			}

			for (auto i = NICFREE; i > 0; --i) {
				if (*(reinterpret_cast<int *>(data_) + i - 1) != 0) {
					auto id = *(reinterpret_cast<int *>(data_) + i - 1);
					*(reinterpret_cast<int *>(data_) + i - 1) = 0;
					return id;
				}
			}

			return 0;
		}


		char * block(int id) const
		{
			return data_ + id * 512;
		}
		char * data_;

	private:
		
		long block_num_ = 0;
	};


}


#endif // !__CORE_H