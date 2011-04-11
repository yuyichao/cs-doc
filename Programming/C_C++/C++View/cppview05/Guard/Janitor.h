#ifndef JANITOR_H_
#define JANITOR_H_

#include <memory>
#include "ScopeGuard.h"

struct ICmd_
{
	virtual void Dismiss() const throw() = 0;
	virtual ~ICmd_() throw() {}
};

template<typename T>
class CmdAdaptor : public ICmd_, protected T
{
public:
	template<typename Fun>
		CmdAdaptor(Fun fun) : T(fun) {}
	template<typename Fun, typename P1>
		CmdAdaptor(Fun fun, P1 p1) : T(fun, p1) {}
	template<typename Fun, typename P1, typename P2>
		CmdAdaptor(Fun fun, P1 p1, P2 p2) : T(fun, p1, p2) {}
	template<typename Fun, typename P1, typename P2, typename P3>
		CmdAdaptor(Fun fun, P1 p1, P2 p2, P3 p3) : T(fun, p1, p2, p3) {}
	void Dismiss() const throw()
	{
		T::Dismiss();
	}
};

class Janitor
{
public:
	Janitor() throw() {}
	template <typename F>
		Janitor(F pFun) : spCmd_(
		new CmdAdaptor<ScopeGuardImpl0<F> >(pFun)) {}
	template <typename F, typename P1>
		Janitor(F pFun, P1 p1) : spCmd_(
		new CmdAdaptor<ScopeGuardImpl1<F, P1> >(pFun, p1)) {}
	template <typename F, typename P1, typename P2>
		Janitor(F pFun, P1 p1, P2 p2) : spCmd_(
		new CmdAdaptor<ScopeGuardImpl2<F, P1, P2> >(pFun, p1, p2)) {}
	template <typename F, typename P1, typename P2, typename P3>
		Janitor(F pFun, P1 p1, P2 p2, P3 p3) : spCmd_(
		new CmdAdaptor<ScopeGuardImpl3<F, P1, P2, P3> >(pFun, p1, p2, p3)) {}

	Janitor(const Janitor& other) throw() : spCmd_(other.spCmd_) {} //VC++, Comeau need it!
	Janitor& operator =(const Janitor& other) throw()
	{
		if (spCmd_.get())
			spCmd_->Dismiss();
		spCmd_ = other.spCmd_;
		return *this;
	}
	void Dismiss() const throw()
	{
		spCmd_->Dismiss();
	}
protected:
	mutable std::auto_ptr<ICmd_> spCmd_;
};

template<typename T>
class ObjCmdAdaptor : public ICmd_, protected T
{
public:
	template<typename Obj, typename MemFun>
		ObjCmdAdaptor(Obj& obj, MemFun memFun) : T(obj, memFun) {}
	template<typename Obj, typename MemFun, typename P1>
		ObjCmdAdaptor(Obj& obj, MemFun memFun, P1 p1) : T(obj, memFun, p1) {}
	template<typename Obj, typename MemFun, typename P1, typename P2>
		ObjCmdAdaptor(Obj& obj, MemFun memFun, P1 p1, P2 p2) : T(obj, memFun, p1, p2) {}
	void Dismiss() const throw()
	{
		T::Dismiss();
	}
};

class ObjJanitor : protected Janitor
{
public:
	using Janitor::Dismiss;
	ObjJanitor() throw() {}

	template <typename Obj, typename MemFun>
		ObjJanitor(Obj& obj, MemFun memFun)
	{
		std::auto_ptr<ICmd_> spTmp(
			new ObjCmdAdaptor<ObjScopeGuardImpl0<Obj, MemFun> >(obj, memFun));
		spCmd_ = spTmp;
	}
	template <typename Obj, typename MemFun, typename P1>
		ObjJanitor(Obj& obj, MemFun memFun, P1 p1)
	{
		std::auto_ptr<ICmd_> spTmp(
			new ObjCmdAdaptor<ObjScopeGuardImpl1<Obj, MemFun, P1> >(obj, memFun, p1));
		spCmd_ = spTmp;
	}
	template <typename Obj, typename MemFun, typename P1, typename P2>
		ObjJanitor(Obj& obj, MemFun memFun, P1 p1, P2 p2)
	{
		std::auto_ptr<ICmd_> spTmp(
			new ObjCmdAdaptor<ObjScopeGuardImpl2<Obj, MemFun, P1, P2> >(obj, memFun, p1, p2));
		spCmd_ = spTmp;
	}
};

#endif //JANITOR_H_
