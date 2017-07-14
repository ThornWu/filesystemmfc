#include <iostream>
#include "file.h"
#include "manager.h"
#include <regex>

int exe_cmd();

static auto && manager = z::Manager::Instance();

int main(int argc, char *argv[])
{
	std::string username, password;

	manager.load_users();
	if (!manager.login("root", "root"))
		return -1;
//	do {
//		std::cout << "login$ username:"; std::cin >> username;
//		std::cout << "login$ password:"; std::cin >> password;
//	} while (!manager.login(username, password));

	z::File test("/home/test/ttt.txt");
	test.write("Hello", 5);
	test.close();


	auto rv = exe_cmd();

	manager.destory();

	return rv;
}


int exe_cmd()
{
	std::string pwd = "/";
	std::string in = "";

	while (true) {
		auto current_username = manager.current_user_.username_;
		std::cout << std::endl << current_username << "@zfs " << pwd << "$ ";

		std::getline(std::cin, in);
		auto cmd = z::Manager::cmd_split(in);

		// 共享文件
		// 复制
		if (cmd[0] == "cp") {
			// 文件拷贝
			// 文件夹拷贝
			if(cmd.size() < 3) continue;

			auto src_path = cmd[1], dst_path = cmd[2];
			if (!z::Path::isFullPath(cmd[1])) src_path = pwd + "/" + cmd[1];
			if (!z::Path::isFullPath(cmd[2])) dst_path = pwd + "/" + cmd[2];

			if (!manager.cp(src_path, dst_path))
				std::cout << "复制失败" << std::endl;

		}
		else if(cmd[0] == "format") {
			if (!manager.format())
				std::cout << "格式化失败" << std::endl;
		}
		else if(cmd[0] == "ln") {
			if(cmd.size() < 3) continue;


			auto old_path = cmd[1], new_path = cmd[2];
			if (!z::Path::isFullPath(cmd[1])) old_path = pwd + "/" + cmd[1];
			if (!z::Path::isFullPath(cmd[2])) new_path = pwd + "/" + cmd[2];
			if (!manager.ln(old_path, new_path))
				std::cout << "链接文件失败" << std::endl;
		}
		else if(cmd[0] == "adduser") {
			if (cmd.size() < 3) continue;

			if(manager.current_user_.username_ == "root") {
				if (!manager.signup(cmd[1], cmd[2]))
					std::cout << "注册失败" << std::endl;
			}
			else {
				std::cout << "权限不足" << std::endl;
			}
		}
		else if(cmd[0] == "login") {
			if(cmd.size() < 3) continue;

			if(!manager.login(cmd[1], cmd[2])) {
				std::cout << "登录失败" << std::endl;
			}
		}
		else if(cmd[0] == "logout") {
			manager.current_user_ = z::User();
		}
		// 目录操作
		else if (cmd[0] == "cd") {
			if (cmd.size() < 2) continue;

			auto temp_path = cmd[1];
			if(!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[1];

			if (manager.exist(temp_path))
				pwd = z::Path::parser(temp_path);
			else
				std::cout << "不存在的目录." << std::endl;
		}
		else if (cmd[0] == "rm") {
			if (cmd.size() < 2) continue;

			auto temp_path = cmd[1];
			if (!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[1];

			if (!manager.rm(temp_path))
				std::cout << "删除失败." << std::endl;
		}
		else if (cmd[0] == "mv") {
			if(cmd.size() < 3) continue;

			auto dst = cmd.back();
			if (!z::Path::isFullPath(dst)) dst = pwd + "/" + dst;

			for(auto i = 1; i < cmd.size() - 1; ++i) {

				auto temp_path = cmd[i];
				if (!z::Path::isFullPath(cmd[i])) temp_path = pwd + "/" + cmd[i];
				manager.mv(temp_path, dst);
			}
		}
		// 文件列表
		else if (cmd[0] == "ls") {
			auto list = manager.ls(pwd);
			for (auto item : list) 
				std::cout << item.info() << std::endl;
		}
		else if (cmd[0] == "rename") {
			if (cmd.size() < 3) continue;

			auto temp_path = cmd[1];
			if (!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[1];

			if (!manager.rename(temp_path, cmd[2]))
				std::cout << "重命名失败." << std::endl;
		}
		else if (cmd[0] == "touch") {
			if (cmd.size() < 2) continue;

			for (std::string::size_type i = 1; i < cmd.size(); ++i) {
				auto temp_path = cmd[i];
				if (!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[i];

				manager.touch(temp_path);
			}
		}
		// 查看文件
		else if (cmd[0] == "cat") {
			if (cmd.size() < 2) continue;

			z::File file(pwd + "/" + cmd[1], z::File::in);
			if (!file.is_open())
				std::cout << "文件打开失败" << std::endl;

			auto buf_size = file.size() + 1;
			auto buf = new char[buf_size];
			memset(buf, 0, buf_size);
			file.read(buf, file.size());

			std::cout << buf << std::endl;
		}
		else if (cmd[0] == "mkdir") {
			if (cmd.size() < 2) {
				std::cout << "文件夹名不能为空" << std::endl;
				return -2;
			}

			for (std::string::size_type i = 1; i < cmd.size(); ++i) {

				auto temp_path = cmd[1];
				if (!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[1];
				manager.mkdirs(temp_path);
			}
		}
		else if(cmd[0] == "whoami") {
			std::cout << manager.current_user_.username_ << "\t" << manager.current_user_.id() << std::endl;
		}
		else if (cmd[0] == "help") {
			std::cout << "\tcd <path> : 改变当前路径" << std::endl;
			std::cout << "\tls : 列出当前目录的所有文件和文件夹" << std::endl;
			std::cout << "\tcp <path> <path> : 复制" << std::endl;
			std::cout << "\tmv <path> <path> : 移动" << std::endl;
			std::cout << "\trm <path> : 删除" << std::endl;
			std::cout << "\tmkdir <dirname> : 新建文件夹" << std::endl;
			std::cout << "\ttouch <filename> : 新建文件" << std::endl;
			std::cout << "\trename <path> <path> : 重命名" << std::endl;
			std::cout << "\tcat <filename> : 打印文件内容" << std::endl;
		}
		else if(cmd[0] == "exit") {
			return 0;
		}
		else {
			std::cout << "  命令不存在!" << std::endl;
		}
	}
}