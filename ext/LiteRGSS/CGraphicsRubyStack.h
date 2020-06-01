#ifndef CGraphicsRubyStack_H
#define CGraphicsRubyStack_H

#include <unordered_set>
#include "RubyValue.h"

class CGraphicsRubyStackItem;
class CGraphicsRubyStack {
public:
	CGraphicsRubyStack(VALUE self);
	CGraphicsRubyStack(const CGraphicsRubyStack&) = delete;
	CGraphicsRubyStack& operator=(const CGraphicsRubyStack&) = delete;
	virtual ~CGraphicsRubyStack();

	void add(VALUE el);
	void remove(VALUE el);
	void disposeAll();

	VALUE self() const { return m_table; }

	static std::unordered_set<VALUE> DisableDispose;

private:
	VALUE m_table = Qnil;
};

#endif