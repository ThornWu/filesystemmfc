#ifndef __FILE_H__
#define __FILE_H__

#include "core.h"


namespace z
{

	enum FILE_FLAG
	{
		// �Ƿ����ļ�
		FILE = 0x00,
		DIR = 0x01,
		DEL = 0x00,
		CREATE = 0x02,
		LINK = 0x04,

	};
	/**
	* �ļ�
	*/

	class File
	{
	public:
		enum Model
		{
			in = 0x01,		// д�ļ�
			out = 0x02,		// ���ļ�
			ate = 0x04,		// ��ʼλ�ã��ļ�β
			app = 0x08,		// ����������ļ�β
			trunc = 0x16,   // ����ļ���������ɾ�����ļ�
			binary = 0x32	// ������
		};
		enum BasePos
		{
			beg = 0x01,
			cur = 0x02,
			end = 0x03,
		};
	public:
		explicit File(const std::string& filename, const std::string& username, const inode& in);

		explicit File(const std::string & filename, int model = in | out);

		void init(const std::string& filename, const std::string& username, const inode& in);

		// �ļ�����
		void open(const std::string& filename, int model = in | out);

		/**
		 * \breif �ļ����Ƿ�ɹ�
		 */
		bool is_open() const;

		int read(char * const buf, uint32_t count) const;
		int write(const char *buf, uint32_t count);

		/**
		 * \breif �Ƿ�������ļ�β
		 */
		bool eof() const;

		// ������
		void seekg(int64_t pos);
		void seekg(int64_t ofs_pos, BasePos base_pos);

		// �����
		void seekp(int64_t pos);
		void seekp(int64_t ofs_pos, BasePos base_pos);

		int64_t tellp() const { return wpos_; } // �����λ��

		bool is_link() const { return flag_ & 0x04; }
		bool is_file() const { return !(flag_ & 0x01); }
		bool is_dir() const { return !is_file(); }


		/**
		 * \breif [�ļ��Ƿ�Ϊ��]
		 */
		bool empty() const { return !!size_; }


		// �ļ���Ϣ
		std::string info() const;
		void close();

		uint32_t size() const { return size_; }

	
		bool __in__() const { return model & in; }
		bool __out__() const { return model & out; }
		bool __ate__() const { return model & ate; }
		bool __app__() const { return model & app; }
		bool __trunc__() const { return model & trunc; }
		bool __binary__() const { return model & binary; }


		int model = 0;

		uint16_t inode_id_ = 0;

		std::string filename_;
		std::string username_;
		std::string path_;

		uint8_t access_ = 0;
		uint16_t uid_ = 0;
		uint32_t size_ = 0;
		uint8_t flag_ = 0;
		uint64_t ctime_ = 0;
		uint64_t mtime_ = 0;
		std::vector<uint16_t> blocks_{ 0 };

		int64_t rpos_ = 0;
		int64_t wpos_ = 0;
	};
}
#endif // ! __FILE_H__