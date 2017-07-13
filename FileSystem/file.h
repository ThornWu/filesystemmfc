#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#include <ctime>
#include "core.h"


namespace z
{

	enum FILE_FLAG
	{
		// 是否是文件
		FILE = 0x00,
		DIR = 0x01,
		DEL = 0x00,
		CREATE = 0x02

	};
	/**
	* 文件
	*/

	class File
	{
	public:
		explicit File(const std::string& filename, const std::string& username, const inode& in);

		explicit File(const std::string & filename);

		// 文件操作
		bool open(std::string filename);

		bool read();
		bool write(std::string content);

		bool exist();
		bool create(const char* path);

		std::string filename_;
		std::string username_;
		std::string path_;

		uint8_t access_;
		uint16_t uid_;
		uint32_t size_;
		uint8_t flag_;
		uint64_t ctime_;
		uint64_t mtime_;
		std::vector<uint16_t> blocks_;

		std::string info()
		{
			std::string rstr;

			// 读写权限
			if (flag_ & 0x01) {
				rstr += "d";
			}
			else {
				rstr += "-";
			}

			rstr += "rw--";


			rstr += " ";

			// 创建者
			rstr += username_;

			rstr += "\t";

			// 创建时间
			char buf[20];
			strftime(buf, 20, "%Y-%m-%d %H:%M:%S", localtime(reinterpret_cast<time_t const *>(&mtime_)));

			rstr += std::string(buf);
			rstr += " ";

			// 文件大小
			if (size_ < 10240) {
				rstr += std::to_string(size_);
			}


			rstr += "\t";

			// 文件名
			rstr += filename_;

			return rstr;
		}

	private:



	};
}
#endif // ! __FILE_H__