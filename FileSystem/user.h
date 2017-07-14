#ifndef __USER_H
#define __USER_H

#include <string>

namespace z
{
	

	/**
	 * \ berif 用户类
	 */
	class User
	{
	public:
		User() = default;
		User(const std::string & username, const std::string & password)
		: username_(username), password_(password) {
			id_ = user_counter++;
		}
		User(const std::string&id, const std::string&name, const std::string&password) : username_(name), password_(password) { id_ = atoi(id.c_str()); }
		User(const  User&) = default;
		User &operator=(const User&) = default;
		~User() = default;

		/**
		 * 获取用户id
		 */
		inline int id() const { return id_; }

		std::string to_string() const
		{
			return "<user>\n\t<userid>" + std::to_string(id()) + "</userid>\n\t<username>" + username_ + "</username>\n\t<password>" + password_ + "</password>\n</user>\n";
		}

		std::string username_;
		std::string password_;

		static int user_counter;

	private:
		int id_ = 0;
	};

	
}


#endif // !__USER_H
