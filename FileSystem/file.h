#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#include <ctime>
#include "core.h"


namespace z
{

	enum FILE_FLAG
	{
		// �Ƿ����ļ�
		FILE = 0x00,
		DIR = 0x01,
		DEL = 0x00,
		CREATE = 0x02

	};
	/**
	* �ļ�
	*/

	class File
	{
	public:
		explicit File(const std::string& filename, const std::string& username, const inode& in);

		explicit File(const std::string & filename);

		// �ļ�����
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

			// ��дȨ��
			if (flag_ & 0x01) {
				rstr += "d";
			}
			else {
				rstr += "-";
			}

			rstr += "rw--";


			rstr += " ";

			// ������
			rstr += username_;

			rstr += "\t";

			// ����ʱ��
			char buf[20];
			strftime(buf, 20, "%Y-%m-%d %H:%M:%S", localtime(reinterpret_cast<time_t const *>(&mtime_)));

			rstr += std::string(buf);
			rstr += " ";

			// �ļ���С
			if (size_ < 10240) {
				rstr += std::to_string(size_);
			}


			rstr += "\t";

			// �ļ���
			rstr += filename_;

			return rstr;
		}

	private:



	};
}
#endif // ! __FILE_H__