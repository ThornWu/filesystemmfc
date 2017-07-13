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
		User(const  User&) = default;
		User &operator=(const User&) = default;
		~User() = default;

		/**
		 * 获取用户id
		 */
		inline int id() const { return id_; }

		std::string username_;
		std::string password_;

		static int user_counter;

	private:
		int id_ = 0;
	};

	
}


#endif // !__USER_H
