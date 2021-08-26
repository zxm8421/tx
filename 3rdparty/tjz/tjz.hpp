/**
 * @brief 
 * 
 * @file tjz.hpp
 */
#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "ttype.h"

class tJZ
{
public:
	tJZ();
	tJZ(tJZ &&) = default;
	tJZ(const tJZ &) = default;
	tJZ &operator=(tJZ &&) = default;
	tJZ &operator=(const tJZ &) = default;
	~tJZ();

	/**
	 * @brief 
	 * 0b'0000'0000 ~ 0b'0011'1111
	 */
	enum struct Type
	{
		none = 0,

		jz = 10,
		bo,
		i,
		f,
		b,
		c,

		jzs = 20,
		bos,
		is,
		fs,
		bs,
		cs,
	};

	struct Error
	{
		te e;
		std::string msg;

		explicit operator bool() const
		{
			if (e != 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	ti clear();

	ti load(const tb *byte, const ti len);
	ti load(const std::vector<tb> byte);
	ti dump(tb *byte);
	std::vector<tb> dump();

	Type getType();

	Error getLastError();
	ti getLen();
	ti getSize();
	std::string getName();

	ti add(const tJZ &jz);
	ti add(const tc *path, const tJZ &jz);
	ti add(const std::string path, const tJZ &jz);

	ti remove(const tc *name);
	ti remove(const std::string &name);

	ti from_tbo(const tbo &v);
	ti from_ti(const ti &v);
	ti from_tf(const tf &v);
	ti from_tb(const tb &v);
	ti from_tc(const tc &v);

	tbo to_tbo();
	ti to_ti();
	tf to_tf();
	tb to_tb();
	tc to_tc();

	ti from_tbs(const tb *v);
	ti from_tbs(const tb *v, const ti len);
	ti from_tbs(const std::vector<tb> &v);
	std::vector<tb> to_tbs();
	ti from_tbs_hex(const tc *v);
	ti from_tbs_hex(const tc *v, const ti len);
	ti from_tbs_hex(const std::string &v);
	std::string to_tbs_hex();

	ti from_tcs(const tc *v);
	ti from_tcs(const tc *v, const ti len);
	ti from_tcs(const std::vector<tc> &v);
	std::vector<tc> to_tcs();
	std::string to_tcs_str();

private:
	std::string name;
	std::vector<tb> rawByte;

	std::weak_ptr<tJZ> super;
	std::map<std::string, std::shared_ptr<tJZ>> child;

	union Var
	{
		tbo vbo;
		ti vi;
		tf vf;
		tb vb;
		tc vc;
	};
	std::vector<Var> var;

	Error lastError;
	Type type;
	ti len;
};
