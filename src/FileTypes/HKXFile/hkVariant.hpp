#ifndef __FILETYPES__HKXFILE__HKVARIANT_HPP__
#define __FILETYPES__HKXFILE__HKVARIANT_HPP__

class hkClass;

struct hkVariant {
	void* m_object;
	hkClass* m_class;
};

#endif