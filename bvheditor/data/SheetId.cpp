#include <ostream> 

#include "SheetId.h"


using namespace cacani::data;

std::ostream& operator << (std::ostream& os, const SheetId& sid)
{
	os << sid.lid() << sid.fid();
    return os;
}

bool SheetId::operator == (const SheetId& rhs) const
{
	return (m_lid == rhs.lid() && m_fid == rhs.fid());
}

//CacObj* SheetId::resolve(const LayerGroup& root) const
//{
//    Sequence* ts = 0;
//	int offset = 0;
	/*if(root.typeId() == TID_CUT)
	{
		if(m_lid.indent() >= 1)
		{
			SheetId::LidType lid = m_lid;
			lid.setIndent(1);
			Cel* parentCel = 0;
			lid.resolve(root, parentCel);
			if(parentCel)
				offset = parentCel->frameOffset();
		}
	}*/

    /*m_lid.resolve(root, ts);
    if(ts && m_fid < static_cast<int>(ts->sheetCount()) && m_fid >= offset)
    {
        CacObj* sheet = &ts->sheetAt(m_fid - offset).rawobj();

		switch(sheet->typeId())
		{
		case TID_LINK_SHEET:
			{
				LinkSheet* ls = dynamic_cast<LinkSheet*>(sheet);
				return ls->getLink();
			}
		case TID_REPEAT_SHEET:
			{
				for(int fid = static_cast<int>(m_fid - offset) - 1; fid >= 0; --fid)
				{
					CacObj& tmp = ts->sheetAt(static_cast<FrameId>(fid)).rawobj();
					if(tmp.typeId() == TID_KEY_SHEET || 
                        tmp.typeId() == TID_NULL_SHEET || 
                        tmp.typeId() == TID_CEL_SHEET ||
                        tmp.typeId() == TID_MERGE_SHEET  ||
                        tmp.typeId() == TID_INBETWEEN_SHEET ||
                        tmp.typeId() == TID_RASTER_SHEET ||
						tmp.typeId() == TID_DRAWING_SHEET ||
						tmp.typeId() == TID_SKETCH_SHEET) 
					{
						return &tmp;
					}
				}
				return 0;
			}

		case TID_DRAWING_SHEET:
			{
				DrawingSheet* ds = dynamic_cast<DrawingSheet* >(sheet);
				if(ds->sheetFlag() == DrawingSheet::kRepeat)
				{
					for(int fid = static_cast<int>(m_fid - offset) - 1; fid >= 0; --fid)
					{
						CacObj& tmp = ts->sheetAt(static_cast<FrameId>(fid)).rawobj();
						if(tmp.typeId() == TID_REPEAT_SHEET)
							continue;
						else if(tmp.typeId() == TID_DRAWING_SHEET)
						{
							DrawingSheet* checkSheet = dynamic_cast<DrawingSheet*>(&tmp);
							if(checkSheet->sheetFlag() == DrawingSheet::kRepeat)
								continue;
							else
								return &tmp;
						}
						else
							return &tmp;
					}
				}
				else
					return sheet;
			}

		default:
			return sheet;
		}
    }
    else
    {
        return 0;
    }
}

CacObj* SheetId::resolveNoAutoLink(const LayerGroup& root) const
{
	Sequence* ts = 0;
	m_lid.resolve(root, ts);
    if(!ts)
        return 0;

	if(m_fid < static_cast<int>(ts->sheetCount()))
	{
		return ts->sheetAt(m_fid).rawptr();
	}
	else
	{
		return 0;
	}
}*/