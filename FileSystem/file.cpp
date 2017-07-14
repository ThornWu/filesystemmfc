#include "file.h"
#include "manager.h"

static auto && manager = z::Manager::Instance();

z::File::File(const std::string& filename, const std::string& username, const inode& in)
{
	init(filename, username, in);
}

z::File::File(const std::string& filename, int model/* = in | out*/)
{
	open(filename, model);
}

void z::File::init(const std::string& filename, const std::string& username, const inode& in)
{
	filename_ = filename;
	username_ = username;

	access_ = in.access();
	uid_ = in.uid();
	size_ = in.size();
	flag_ = in.flag();
	ctime_ = in.ctime();
	mtime_ = in.mtime();

	auto blocks = size_ / 512;
	if (size_ % 512) blocks += 1;

	for (uint32_t i = 0; i < blocks; ++i) {
		blocks_.push_back(in.blockId(i));
	}
}


void z::File::open(const std::string& filename, int model/* = in | out*/)
{
	this->model = model;
	
	// ʧ�ܵ�ģʽ
	if(__trunc__() && __app__()) return;
	if(__trunc__() && !__out__()) return;

	// ����ļ��������򴴽�
	if (!manager.exist(filename)) {
		manager.touch(filename);
	}
	// ����ļ�������out������ļ�
	else if((__out__() && !__ate__() && !__app__()) || __trunc__()) {
		manager.clear(filename);
	}

	// ���ļ�
	manager.search(Path::split(filename), inode_id_);
	auto&& in = manager.getInode(inode_id_);

	init(filename, manager.getUsernameById(in.uid()), in);

	// ��λ���ļ�ĩβ
	if (__app__() || __ate__())
		wpos_ = size_;
}

bool z::File::is_open() const
{
	return !!inode_id_;
}

bool z::File::eof() const
{
	return size_ == rpos_;
}



std::string z::File::info() const
{
	std::string rstr;
	is_dir() ? rstr += "d" : rstr += "-";

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

void z::File::close()
{
}

int z::File::read(char* const buf, uint32_t count) const
{
	return manager.read(filename_, buf, rpos_, count);
}

int z::File::write(const char* buf, uint32_t count)
{
	auto wc = manager.write(filename_, buf, wpos_, count);
	wpos_ += wc;
	return wc;
}

void z::File::seekg(int64_t pos)
{
	rpos_ = pos;

	if (rpos_ < 0) rpos_ = 0;
	if (rpos_ > size_) rpos_ = size_;
}

void z::File::seekg(int64_t ofs_pos, BasePos base_pos)
{
	switch (base_pos) {
	case beg:
		rpos_ = ofs_pos;
		break;

	case cur:
		rpos_ += ofs_pos;
		break;

	case end:
		rpos_ = size_ + ofs_pos;
		break;
	}

	if (rpos_ < 0) rpos_ = 0;
	if (rpos_ > size_) rpos_ = size_;
}

void z::File::seekp(int64_t pos)
{
	if (!__app__())
		wpos_ = pos;

	if (wpos_ < 0) wpos_ = 0;
	if (wpos_ > size_) wpos_ = size_;
}

void z::File::seekp(int64_t ofs_pos, BasePos base_pos)
{
	switch (base_pos) {
	case beg:
		wpos_ = ofs_pos;
		break;

	case cur:
		wpos_ += ofs_pos;
		break;

	case end:
		wpos_ = size_ + ofs_pos;
		break;
	}

	if (wpos_ < 0) wpos_ = 0;
	if (wpos_ > size_) wpos_ = size_;
}
