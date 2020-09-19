#ifndef __FILETYPES__HKXFILE__CLASSES__HKPPHYSICSYSTEM_HPP__
#define __FILETYPES__HKXFILE__CLASSES__HKPPHYSICSYSTEM_HPP__

#include "hkReferencedObject.hpp"
#include "hkpRigidBody.hpp"

class hkpPhysicsSystem : public hkReferencedObject {
	HKX::HKXFile* hkfile; unsigned char* fd;
public:

	std::list<hkpRigidBody> m_rigidBodies;
	std::list<void*> m_constraints;
	std::list<void*> m_actions;
	std::list<void*> m_phantoms;
	std::string_view m_name;
	std::uint32_t m_userData;
	bool m_active;

	void Read(HKX::HKXFile* hkfile, unsigned char* fd, std::uint32_t off) {


		this->hkfile = hkfile; this->fd = fd;

		unsigned char* doff = fd + off;

		for (auto itType = hkfile->hkTypes.begin(); itType != hkfile->hkTypes.end(); ++itType) {
			if (itType->name != "hkpPhysicsSystem") continue;

			hkReferencedObject::Read(hkfile, fd, off, itType->parent);

			// m_rigidBodies
			{
				auto member_rigidBodies = itType->members.at(0);
				auto memb_doff = doff + member_rigidBodies.offset;
				auto memb_arrcount = *reinterpret_cast<std::uint32_t*>(memb_doff + 4);
				if(memb_arrcount > 0) {
					auto memb_arrdata = fd + hkfile->GetDataPointerTarget(memb_doff - fd);

					for (int i = 0; i < memb_arrcount; ++i) {
						auto memb_arrelem =  fd + hkfile->GetGlobalDataPointerTarget((memb_arrdata - fd) + i * 4);
						
						hkpRigidBody rigidBody;
						rigidBody.Read(hkfile, fd, memb_arrelem - fd);
						m_rigidBodies.push_back(rigidBody);
					}
				}
			}
			// m_constraints
			{
				auto member_constraints = itType->members.at(1);
				auto memb_doff = doff + member_constraints.offset;
				auto memb_arrcount = *reinterpret_cast<std::uint32_t*>(memb_doff + 4);
				if(memb_arrcount > 0) {
					auto memb_arrdata = fd + hkfile->GetDataPointerTarget(memb_doff - fd);

					for (int i = 0; i < memb_arrcount; ++i) {
						auto memb_arrelem = fd + hkfile->GetGlobalDataPointerTarget((memb_arrdata - fd) + i * 4);
						// TODO
					}
				}
			}
			// m_actions
			{
				auto member_actions = itType->members.at(2);
				auto memb_doff = doff + member_actions.offset;
				auto memb_arrcount = *reinterpret_cast<std::uint32_t*>(memb_doff + 4);
				if(memb_arrcount > 0) {
					auto memb_arrdata = fd + hkfile->GetDataPointerTarget(memb_doff - fd);

					for (int i = 0; i < memb_arrcount; ++i) {
						auto memb_arrelem = fd + hkfile->GetGlobalDataPointerTarget((memb_arrdata - fd) + i * 4);
						// TODO
					}
				}
			}
			// m_phantoms
			{
				auto member_phantoms = itType->members.at(3);
				auto memb_doff = doff + member_phantoms.offset;
				auto memb_arrcount = *reinterpret_cast<std::uint32_t*>(memb_doff + 4);
				if (memb_arrcount > 0) {
					auto memb_arrdata = fd + hkfile->GetDataPointerTarget(memb_doff - fd);

					for (int i = 0; i < memb_arrcount; ++i) {
						auto memb_arrelem = fd + hkfile->GetGlobalDataPointerTarget((memb_arrdata - fd) + i * 4);
						// TODO
					}
				}
			}
			// m_name
			{
				auto member_name = itType->members.at(4);
				auto memb_doff = doff + member_name.offset;
				auto memb_strptr = fd + hkfile->GetDataPointerTarget(memb_doff - fd);
				m_name = std::string_view(reinterpret_cast<char*>(memb_strptr));
			}
			// m_userData
			{
				auto member_userData = itType->members.at(5);
				auto memb_doff = doff + member_userData.offset;
				m_userData = *reinterpret_cast<std::uint32_t*>(memb_doff);
			}
			// m_active
			{
				auto member_active = itType->members.at(6);
				auto memb_doff = doff + member_active.offset;
				m_active = *reinterpret_cast<std::uint32_t*>(memb_doff) == 1;
			}

			break;
		}
	}
};

#endif