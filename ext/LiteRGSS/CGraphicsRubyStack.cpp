#include "ruby_common.h"
#include <cstdio>
#include "CGraphicsRubyStack.h"

extern ID rb_iElementTable;

std::unordered_set<VALUE> CGraphicsRubyStack::DisableDispose;

extern "C" {
	//Handles Ruby side deletion of "m_table" before his C++ side destruction
	void CGraphicsRubyStackTableFinalize(VALUE data) {
		CGraphicsRubyStack::DisableDispose.insert(data);
	}
}

CGraphicsRubyStack::CGraphicsRubyStack(VALUE self) {
	rb_ivar_set(self, rb_iElementTable, m_table = rb_ary_new());
	rb_global_variable(&m_table);
	rb_set_end_proc(CGraphicsRubyStackTableFinalize, m_table);
}

void CGraphicsRubyStack::add(VALUE el) {
	rb_ary_push(m_table, el);
}

void CGraphicsRubyStack::remove(VALUE el) {
	rb_ary_delete(m_table, el);
}

void CGraphicsRubyStack::disposeAll() {
	if(m_table == Qnil) {
		return;
	}

	if(DisableDispose.find(m_table) == DisableDispose.end()) {
		rb_check_type(m_table, T_ARRAY);
		const long sz = RARRAY_LEN(m_table);

		VALUE* ori = RARRAY_PTR(m_table);
		ID dispose_method = rb_intern("dispose");
		for (long i = 0u; i < sz; i++) {
			const auto it = ori[i];
			if(rb_obj_is_kind_of(it, rb_cDrawable) == Qtrue && RDATA(it)->data != nullptr) {
				rb_funcallv(it, dispose_method, 0, NULL);
			}
		}
		rb_ary_clear(m_table);
	}
}

CGraphicsRubyStack::~CGraphicsRubyStack() {
	CGraphicsRubyStack::DisableDispose.erase(m_table);
	m_table = Qnil;
}