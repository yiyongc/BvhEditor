/*****************************************************************************
Filename    : SheetId.h

*****************************************************************************/
#ifndef SHEETID_H
#define SHEETID_H
#pragma once

#include "LayerId.h"
#include "FrameId.h"


namespace cacani { namespace data {

	struct FrameId;
	class LayerId;

	template <typename T> class MetaObj;
	//typedef MetaObj<CacObj> MetaObjCacObj;

	/*!
	 * \brief A light weight value to locate a sheet within the cel facade.
	 *
	 */
	class SheetId
	{
		//friend struct SerializeInfo<SheetId>; 
		//template<class Archive> 
		//void serialize(Archive & ar, version_t /*version*/, bool /*verbose*/) 
		/*{
			ar & makeNvp("layer", m_lid);
			ar & makeNvp("frame", m_fid);
		}*/

		public:
			typedef LayerId LidType;
			typedef FrameId  FidType;

		public:
			inline SheetId() : m_fid(0) {}

			inline SheetId(const LidType& lid, FidType fid)
				: m_lid(lid), m_fid(fid) {}

			inline const LidType& lid() const { return m_lid; }
			inline LidType& lid() { return m_lid; }
			inline FidType fid() const { return m_fid; }
			inline FidType& fid() { return m_fid; }

			template <class Root, class SheetType>
			void resolve(const Root& root, const SheetType*& result) const
			{

				result = dynamic_cast<SheetType*>(resolve(root));
			}

			template <class Root, class SheetType>
			void resolve(Root& root, SheetType*& result) const
			{
				result = dynamic_cast<SheetType*>(resolve(root));
			}

			template <class Root, class SheetType>
			void resolveNoAutoLink(const Root& root, const SheetType*& result) const
			{
				result = dynamic_cast<SheetType*>(resolveNoAutoLink(root));
			}

			template <class Root, class SheetType>
			void resolveNoAutoLink(Root& root, SheetType*& result) const
			{
				result = dynamic_cast<SheetType*>(resolveNoAutoLink(root));
			}


			bool operator == (const SheetId& rhs) const;

		private:
			//CacObj* resolve(const LayerGroup& root) const;
			//CacObj* resolveNoAutoLink(const LayerGroup& root) const;
			//CacObj* resolveOrNull(const Group& root) const;

		private:
			LidType m_lid;
			FidType m_fid;
};

class InvalidSheetId : public std::runtime_error   

{
public:
    InvalidSheetId(const SheetId& sid) : runtime_error("invalid sheet id"), m_sid(sid) { }
    InvalidSheetId(const SheetId& sid, const std::string& msg) 
        : runtime_error(msg), m_sid(sid) { }
    virtual ~InvalidSheetId() throw() {}
private:
    SheetId m_sid;
};

//CAC_BASIC_TYPE_INFO_EXTERNAL(SheetId, TID_SHEET_ID, "SheetId", 0)

//typedef ValueObj<SheetId> SheetIdObj;

//CAC_POLYMORPH_TYPE_INFO_EXTERNAL(SheetIdObj, TID_SHEET_ID_OBJECT, "SheetIdObject", 0)

}}
#endif
std::ostream& operator << (std::ostream& os, const cacani::data::SheetId& sid);
