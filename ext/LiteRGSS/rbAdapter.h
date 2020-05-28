#ifndef L_RBADAPTER_HEADER
#define L_RBADAPTER_HEADER
#include <string>
#include <stdexcept>
#include <functional>
#include <ruby.h>
#include "Common/Meta/metadata.h"

extern VALUE rb_eRGSSError;

namespace rb {
	using ErrorCallback = std::function<void(const std::string&)>;

	template <class T, bool raise>
	std::string CheckDisposed(VALUE self) {
		if(RDATA(self)->data == nullptr) { 
			auto errorMessage = std::string { "Disposed "}; 
			errorMessage += cgss::meta::Log<T>::classname;
			errorMessage += ".";
			if constexpr (raise) {
				rb_raise(rb_eRGSSError, "%s", errorMessage.c_str());
			}
			return errorMessage;
		}
		return "";
	}

	template <class T>
	auto* GetPtr(VALUE self) {
		return reinterpret_cast<T*>(rb_data_object_get(self));
	}

	template <class T>
	auto& GetOr(VALUE self, const ErrorCallback& errorCallback) {
		auto errorMessage = CheckDisposed<T, false>(self);
		if (!errorMessage.empty()) {
			errorCallback(errorMessage);
			throw std::runtime_error(errorMessage);
		}
		return *GetPtr<T>(self);
	}

	template <class T>
	auto& Get(VALUE self) {
		auto errorMessage = CheckDisposed<T>(self);
		if (!errorMessage.empty()) {
			throw std::runtime_error(errorMessage);
		}
		return *GetPtr<T>(self);
	}

	template <class T, bool raise = true>
	std::string CheckType(VALUE self, VALUE expectedType) {
		if (rb_obj_is_kind_of(self, expectedType) != Qtrue) {
			auto errorMessage = std::string { "Expected " };
			errorMessage += cgss::meta::Log<T>::classname;
			errorMessage += " got ";
			errorMessage += RSTRING_PTR(rb_class_name(CLASS_OF(self)));
			errorMessage += ".";
			if constexpr (raise) {
				rb_raise(rb_eTypeError, "%s", errorMessage.c_str());
			}
			return errorMessage;
		}
		return "";
	}

	template <class T>
	auto& GetSafeOr(VALUE self, VALUE expectedType, const ErrorCallback& errorCallback) {
		auto errorMessage = CheckType<T, false>(self, expectedType);
		if (!errorMessage.empty()) {
			errorCallback(errorMessage);
			throw std::runtime_error(errorMessage);
		}

		return GetOr<T>(self, errorCallback);
	}

	template <class T>
	auto& GetSafe(VALUE self, VALUE expectedType) {
		auto errorMessage = CheckType<T>(self, expectedType);
		if (!errorMessage.empty()) {
			throw std::runtime_error(errorMessage);
		}

		return Get<T>(self);
	}

	template <class T>
	auto* GetSafeOrNull(VALUE self, VALUE expectedType) {
		if (!CheckType<T>(self, expectedType).empty() ||
			!CheckDisposed<T>(self).empty()) {
			return nullptr;
		}

		return GetPtr<T>(self);
	}

	template <class T>
	void Free(void* data) {
		delete reinterpret_cast<T*>(data);
	}

	template <class T>
	void Mark(T* data) {}

	template <class T>
	VALUE AllocDrawable(VALUE klass) {
		auto value = new T();
		auto self = Data_Wrap_Struct(klass, Mark<T>, Free<T>, value);
		value->setSelf(self);
		return self;
	}

	template <class T>
	VALUE Alloc(VALUE klass) {
		return Data_Wrap_Struct(klass, Mark<T>, Free<T>, new T());
	}

	template <class T>
	VALUE Dispose(VALUE self) {
		if (RDATA(self)->data == nullptr) {
			return Qnil;
		}
		delete GetPtr<T>(self);
		RDATA(self)->data = nullptr;
		return Qnil;
	}
}
#endif