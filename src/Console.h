#pragma once
#include "Include.h"

//#define SetFunc1Var(func) ConsoleSetFunc<double>(new std::function<void(double)>(func))
//#define SetFunc2Var(func) ConsoleSetFunc<double>(new std::function<void(vec2d)>(func))
//#define SetFunc3Var(func) ConsoleSetFunc<double>(new std::function<void(vec3d)>(func))
//#define SetFunc4Var(func) ConsoleSetFunc<double>(new std::function<void(vec4d)>(func))

#define SetFunc1Var(func) ConsoleSetFunc(new std::function<void(double)>(func))
#define SetFunc2Var(func) ConsoleSetFunc(new std::function<void(vec2d)>(func))
#define SetFunc3Var(func) ConsoleSetFunc(new std::function<void(vec3d)>(func))
#define SetFunc4Var(func) ConsoleSetFunc(new std::function<void(vec4d)>(func))
#define SetFunc5Var(func) ConsoleSetFunc(new std::function<void(vec5d)>(func))

#define GetFunc1Var(func) ConsoleGetFunc(new std::function<double(void)>(func))
#define GetFunc2Var(func) ConsoleGetFunc(new std::function<vec2d(void)>(func))
#define GetFunc3Var(func) ConsoleGetFunc(new std::function<vec3d(void)>(func))
#define GetFunc4Var(func) ConsoleGetFunc(new std::function<vec4d(void)>(func))

class ParamCountMismatchEx : public std::exception
{
public:
	ParamCountMismatchEx(std::string id, int expectedCount, int actualCount) 
	{
		message = "Parameter count mismatch, " + id + " expects " + std::to_string(expectedCount) + ". " + std::to_string(actualCount) + " passed.";
	};

	std::string message;

};

class IdNotFoundEx : public std::exception
{
public:
	IdNotFoundEx(std::string id)
	{
		message = id + " not found";
	};

	std::string message;

};

class InvalidFunctionCallEx : public std::exception
{
public:
	InvalidFunctionCallEx()
	{
		message = "Uninitialised function call attempt";
	};

	std::string message;

};

class ConsoleSetFunc
{
public:
	ConsoleSetFunc() : nVar(NVar::INVALID) {}
	ConsoleSetFunc(std::function<void(void*, double)>* f) : nVar(NVar::One) { func.f0 = f; }
	ConsoleSetFunc(std::function<void(double)>* f) : nVar(NVar::One) { func.f1 = f; }
	ConsoleSetFunc(std::function<void(vec2d)>* f) : nVar(NVar::Two) { func.f2 = f; }
	ConsoleSetFunc(std::function<void(vec3d)>* f) : nVar(NVar::Three) { func.f3 = f; }
	ConsoleSetFunc(std::function<void(vec4d)>* f) : nVar(NVar::Four) { func.f4 = f; }
	ConsoleSetFunc(std::function<void(vec5d)>* f) : nVar(NVar::Five) { func.f5 = f; }
	~ConsoleSetFunc()
	{
		switch (nVar) {
		case One:
			delete func.f1;
			break;
		case Two:
			delete func.f2;
			break;
		case Three:
			delete func.f3;
			break;
		case Four:
			delete func.f4;
			break;
		case Five:
			delete func.f5;
			break;
		}
	}

	void operator()(double x, double y = 0.0, double z = 0.0, double w = 0.0, double v = 0.0)
	{
		switch (nVar) {
		case INVALID:
			throw InvalidFunctionCallEx();
			break;
		case One:
			func.f1->operator()(x);
			break;
		case Two:
			func.f2->operator()(vec2d(x, y));
			break;
		case Three:
			func.f3->operator()(vec3d(x, y, z));
			break;
		case Four:
			func.f4->operator()(vec4d(x, y, z, w));
			break;
		case Five:
			func.f5->operator()(vec5d(x, y, z, w, v));
			break;
		}
	}

	const char getNVar() const
	{
		return nVar;
	}

private:

	enum NVar { INVALID, One, Two, Three, Four, Five };
	const NVar nVar;

	union function
	{
		std::function <void(void*,double)>* f0;
		std::function <void(double)>* f1;
		std::function <void(vec2d)>* f2;
		std::function <void(vec3d)>* f3;
		std::function <void(vec4d)>* f4;
		std::function <void(vec5d)>* f5;
	}func;
};

//template<class T>
//class ConsoleSetFunc
//{
//public:
//	ConsoleSetFunc() : nVar(NVar::INVALID) {}
//	ConsoleSetFunc(std::function<void(void*, T)>* f) : nVar(NVar::One) { func.f0 = f; }
//	ConsoleSetFunc(std::function<void(T)>* f) : nVar(NVar::One) { func.f1 = f; }
//	ConsoleSetFunc(std::function<void(vec2<T>)>* f) : nVar(NVar::Two) { func.f2 = f; }
//	ConsoleSetFunc(std::function<void(vec3<T>)>* f) : nVar(NVar::Three) { func.f3 = f; }
//	ConsoleSetFunc(std::function<void(vec4<T>)>* f) : nVar(NVar::Four) { func.f4 = f; }
//	~ConsoleSetFunc()
//	{
//		switch (nVar) {
//		case One:
//			delete func.f1;
//			break;
//		case Two:
//			delete func.f2;
//			break;
//		case Three:
//			delete func.f3;
//			break;
//		case Four:
//			delete func.f4;
//			break;
//		}
//	}
//
//	void operator()(T x, T y = 0.0, T z = 0.0, T w = 0.0)
//	{
//		switch (nVar) {
//		case INVALID:
//			throw InvalidFunctionCallEx();
//			break;
//		case One:
//			func.f1->operator()(x);
//			break;
//		case Two:
//			func.f2->operator()(vec2<T>(x, y));
//			break;
//		case Three:
//			func.f3->operator()(vec3<T>(x, y, z));
//			break;
//		case Four:
//			func.f4->operator()(vec4<T>(x, y, z, w));
//			break;
//		}
//	}
//
//	const char getNVar() const
//	{
//		return nVar;
//	}
//
//private:
//
//	enum NVar { INVALID, One, Two, Three, Four };
//	const NVar nVar;
//
//	union function
//	{
//		std::function <void(void*, T)>* f0;
//		std::function <void(T)>* f1;
//		std::function <void(vec2<T>)>* f2;
//		std::function <void(vec3<T>)>* f3;
//		std::function <void(vec4<T>)>* f4;
//	}func;
//};

class ConsoleGetFunc
{
public:
	ConsoleGetFunc(std::function<double(void)>* f) : nVar(NVar::One) { func.f1 = f; }
	ConsoleGetFunc(std::function<vec2d(void)>* f) : nVar(NVar::Two) { func.f2 = f; }
	ConsoleGetFunc(std::function<vec3d(void)>* f) : nVar(NVar::Three) { func.f3 = f; }
	ConsoleGetFunc(std::function<vec4d(void)>* f) : nVar(NVar::Four) { func.f4 = f; }
	~ConsoleGetFunc()
	{
		switch (nVar) {
		case One:
			delete func.f1;
			break;
		case Two:
			delete func.f2;
			break;
		case Three:
			delete func.f3;
			break;
		case Four:
			delete func.f4;
			break;
		}
	}

	std::string operator()()
	{
		switch (nVar) {
		case One: {
			auto ret1 = func.f1->operator()();
			return std::to_string(ret1);
			break; }
		case Two: {
			auto ret2 = func.f2->operator()();
			return "(" + std::to_string(ret2.x) + "," + std::to_string(ret2.y) + ")";
			break; }
		case Three: {
			auto ret3 = func.f3->operator()();
			return "(" + std::to_string(ret3.x) + "," + std::to_string(ret3.y) + "," + std::to_string(ret3.z) + ")";
			break; }
		case Four: {
			auto ret4 = func.f4->operator()();
			return "(" + std::to_string(ret4.x) + "," + std::to_string(ret4.y) + "," + std::to_string(ret4.z) + "," + std::to_string(ret4.w) + ")";
			break; }
		}
	}

	const char getNVar() const
	{
		return nVar + 1;
	}

private:

	enum NVar { One, Two, Three, Four };
	const NVar nVar;

	union function
	{
		std::function <double(void)>* f1;
		std::function <vec2d(void)>* f2;
		std::function <vec3d(void)>* f3;
		std::function <vec4d(void)>* f4;
	}func;
};

class Console
{
public:
	Console();
	~Console() {}

	static void init();

	static std::string interpretCommand(std::string cmd);

	static std::string set(std::string id, std::string params);
	static std::string get(std::string id);

	static void addSetter(std::string name, ConsoleSetFunc* func)
	{
		setters.insert(std::make_pair(name, func));
	}

	static void addGetter(std::string name, ConsoleGetFunc* func)
	{
		getters.insert(std::make_pair(name, func));
	}

private:

	static std::map<std::string, ConsoleSetFunc*> setters;
	static std::map<std::string, ConsoleGetFunc*> getters;

};

//#undef SetFunc1Var
//#undef SetFunc2Var
//#undef SetFunc3Var
//#undef SetFunc4Var
//
//#undef GetFunc1Var
//#undef GetFunc2Var
//#undef GetFunc3Var
//#undef GetFunc4Var