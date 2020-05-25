#ifndef CgssWrapper_H
#define CgssWrapper_H
#include <string>
#include <memory>
#include "ruby_common.h"

template <class T>
class CgssInstance {
public:
    CgssInstance() = default;
    ~CgssInstance() = default;

    template <class ... Args>
    void init(Args&& ... args) {
        data = std::make_unique<T>(std::forward<Args>(args)...);
    }

    T* instance() {
        return data.get();
    }

    T* operator->() {
        protect();
        return instance();
    }

private:
	void protect() const {
		if(data == nullptr) { 
			auto errorMessage = std::string { "Disposed "}; 
			errorMessage += cgss::meta::Log<T>::classname;
			errorMessage += ".";
			rb_raise(rb_eRGSSError, "%s", errorMessage.c_str());
			throw std::runtime_error(errorMessage);
		}
	}

    std::unique_ptr<T> data;
};

#endif