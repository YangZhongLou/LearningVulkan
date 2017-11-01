#pragma once

namespace yzl
{
#define Singleton(Class) class Class\
	{\
	public:\
		static Class& GetInstance()\
		{\
			static Class instance;\
			return instance;\
		}\
	public:\
		~Class();\
	private:\
		Class() {}\
	public:\
		Class(Class const&) = delete;\
		void operator=(Class const&) = delete;\

}
