// misc.h: interface for the misc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISC_H__026C37A7_6E5C_4105_AECC_7F8AE288D588__INCLUDED_)
#define AFX_MISC_H__026C37A7_6E5C_4105_AECC_7F8AE288D588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace me { namespace dbg {
	void message(const char* lpszFormat, ...);
	void error(const char* lpszFormat, ...);
	void saveto(const char* lpszDir);
	void save();
	void tag(const char* lpszFormat, ...);
};}; // namespace me { namespace dbg {


#endif // !defined(AFX_MISC_H__026C37A7_6E5C_4105_AECC_7F8AE288D588__INCLUDED_)
