#include <ostream>
#include <algorithm>

#include "LayerId.h"
#include "LayerGroup.h"

using namespace cacani::data;


std::ostream& operator << (std::ostream& os, const LayerId& lid)
{
    for(int i = 1; i <= lid.depth(); i++)
    {
        if(i == lid.indent())
        {
            os << "[" << (int)lid[i] << "]";
        }
        else
        {
            os << (int)lid[i];
        }
        os << (i == lid.depth() ? "." : ",");
    }
    return os;
}

void LayerId::initIndices(const IndexType* src)
{
    // assume m_depth is already initialized to correct value
    // assume m_indices.extended is 0 (no memory allocated yet)

    IndexType* dst = 0;
    if(m_depth > fixedSize)
    {
        dst = new IndexType[m_depth];
        m_indices.extended = dst;
    }
    else
    {
        dst = m_indices.native;
    }

    if(src)
    {
        for(int i = 0; i < m_depth; i++)
        {
            *dst++ = *src++;
        }
    }
    else
    {
        for(int i = 0; i < m_depth; i++)
        {
            *dst++ = 0;
        }
    }
}

/*CacObj* LayerId::resolve(const LayerGroup& root) const
{

	
    const CacObj* result;
    if(m_indent == 0)
    {
        result = dynamic_cast<const CacObj*>(&root);
    }
    else
    {
        const LayerGroup* inter = &root;
		if (!inter)
			return 0;
		size_t memberCount = inter->memberCount();
        for(DepthType i = 1; i < m_indent; i++)
        {
            IndexType index = (*this)[i];
			if (index >= memberCount)
				break;
            const CacObj* obj = inter->memberAt(index).rawptr();
            assert(obj);
            inter = dynamic_cast<const LayerGroup*>(obj);
            assert(inter);
        }
		if (inter->memberCount() == 0 || inter->memberCount() <=(*this)[m_indent])
            result=0;
        else
			result = inter->memberAt((*this)[m_indent]).rawptr();
    }
    
    // const_cast, yes!!
    // this fucntion is a private function, not supposed to be called by other classes
    return const_cast<CacObj*>(result);
}

CacObj* LayerId::resolveOrNull(const LayerGroup& root) const
{
    const CacObj* result;
    if(m_indent == 0)
    {
        result = dynamic_cast<const CacObj*>(&root);
    }
    else
    {
        const LayerGroup* inter = &root;
		if (!inter)
			return 0;
        bool failed = false;
        DepthType i;
        IndexType index;
        for(i = 1; i < m_indent; i++)
        {
            index = (*this)[i];
            if(inter && index < inter->memberCount())
            {
                const CacObj* obj = inter->memberAt(index).rawptr();
                assert(obj);
                inter = dynamic_cast<const LayerGroup*>(obj);
            }
            else
            {
                result = 0;
                failed = true;
                break;
            }
        }
        if(!failed)
        {
            index = (*this)[i];
            if(inter && index < inter->memberCount())
            {
                result = inter->memberAt(index).rawptr();
            }
            else
            {
                result = 0;
            }
        }
    }
    // const_cast, yes!!
    // this function is a private function, not supposed to be called by other classes
    return const_cast<CacObj*>(result);
}

MetaObjCacObj* LayerId::metaResolve(const MetaObjLayerGroup& root) const
{
	const MetaObjCacObj* result;
    if(m_indent == 0)
    {
        result = (&root)->cast<CacObj>();
    }
    else
    {
        const LayerGroup* inter = root.rawptr();
		if (!inter)
			return 0;
		size_t memberCount = inter->memberCount();
        for(DepthType i = 1; i < m_indent; i++)
        {
            IndexType index = (*this)[i];
			if (index >= memberCount)
				break;
			const MetaObjCacObj* obj = &(inter->memberAt(index));
            assert(obj);
            inter = dynamic_cast<const LayerGroup*>(obj->rawptr());
            assert(inter);
        }
        if(inter->memberCount() == 0)
            result=0;
        else
			result = &inter->memberAt((*this)[m_indent]);
    }
    
    // const_cast, yes!!
    // this fucntion is a private function, not supposed to be called by other classes
    return const_cast<MetaObjCacObj*>(result);
}*/

void LayerId::resizeIndices(DepthType new_depth, 
                             const IndexType* extra ,
                             IndexPosition pos)
{
    // assume all member variables have not been modified yet
    // assume extra has (new_depth - m_depth) number of elements
    if(new_depth < m_depth)
    {
        if(new_depth <= fixedSize && m_depth > fixedSize)
        {
            IndexType* old_indices = m_indices.extended;
            IndexType* src = old_indices;
            if(pos == kFront)
            {
                src += m_depth - new_depth;
            }
            IndexType* dst = m_indices.native;
            for(DepthType i = 0; i < new_depth; i++)
            {
                *dst++ = *src++;
            }
            delete [] old_indices;
        }
        else // no actual memory reallocation needed
        {
            if(pos == kFront)
            {
                IndexType* src;
                IndexType* dst;
                if(m_depth > fixedSize)
                {
                    dst = m_indices.extended;
                }
                else
                {
                    dst = m_indices.native;
                }
                src = dst + m_depth - new_depth;

                for(DepthType i = 0; i < new_depth; i++)
                {
                    *dst++ = *src++;
                }
            }
            else
            {
                // nothing needs to be done
            }
        }
        if(pos == kFront)
        {
            m_indent += new_depth - m_depth;
        }
        else if(m_indent > new_depth)
        {
            m_indent = new_depth;
        }
        m_depth = new_depth;
    }
    else if(new_depth > m_depth)
    {
        if(new_depth <= fixedSize)
        {
            if(pos == kBack)
            {
                IndexType* dst = m_indices.native + m_depth;
                for(DepthType i = m_depth; i < new_depth; i++)
                {
                    *dst++ = extra ? *extra++ : 0;;
                }
            }
            else // insert at front
            {
                IndexType* dst = m_indices.native + new_depth - 1;
                IndexType* src = m_indices.native + m_depth - 1;
                for(DepthType i = 0; i < m_depth; i++)
                {
                    *dst-- = *src--;
                }
                dst = m_indices.native;
                for(DepthType i = m_depth; i < new_depth; i++)
                {
                    *dst++ = extra ? *extra++ : 0;;
                }
            }
        }
        else // new_depth > fixedSize
        {
            if(m_depth > fixedSize)
            {
                IndexType* old_indices = m_indices.extended;
                IndexType* new_indices = new IndexType[new_depth];
                IndexType* dst = new_indices;
                IndexType* src = old_indices;

                if(pos == kFront)
                {
                    for(int i = m_depth; i < new_depth; i++)
                    {
                        *dst++ = extra ? *extra++ : 0;;
                    }
                }
                for(int i = 0; i < m_depth; i++)
                {
                    *dst++ = *src++;
                }
                if(pos == kBack)
                {
                    for(int i = m_depth; i < new_depth; i++)
                    {
                        *dst++ = extra ? *extra++ : 0;;
                    }
                }
                m_indices.extended = new_indices;
                delete [] old_indices;
            }
            else
            {
                IndexType* src = m_indices.native;
                IndexType* new_indices = new IndexType[new_depth];
                IndexType* dst = new_indices;

                if(pos == kFront)
                {
                    for(int i = m_depth; i < new_depth; i++)
                    {
                        *dst++ = extra ? *extra++ : 0;;
                    }
                }
                for(int i = 0; i < m_depth; i++)
                {
                    *dst++ = *src++;
                }
                if(pos == kBack)
                {
                    for(int i = m_depth; i < new_depth; i++)
                    {
                        *dst++ = extra ? *extra++ : 0;;
                    }
                }
                m_indices.extended = new_indices;
            }
        }
        if(pos == kFront)
        {
            m_indent += new_depth - m_depth;
        }
        m_depth = new_depth;
    }
    else // new_depth == m_depth
    {
        // do nothing
    }
}


LayerId& LayerId::operator = (const LayerId& rhs)
{
    if(m_depth > fixedSize && m_indices.extended)
    {
        delete [] m_indices.extended;
    }
    m_indices.extended = 0;

    m_depth   = rhs.m_depth;
    m_indent = rhs.m_indent;

    initIndices(rhs.indexPointer());
    return *this;
}

bool LayerId::operator > (const LayerId& rhs) const
{
    int minIndent = std::min(this->m_indent, rhs.m_indent);
    for(int i = 1; i <= minIndent; ++i)
    {
        IndexType myIndex = (*this)[i];
        IndexType rhsIndex = rhs[i];
        if(myIndex != rhsIndex)
        {
            return myIndex > rhsIndex;
        }
    }
    // all equal up to now
    if(this->m_indent != rhs.m_indent)
    {
        return this->m_indent > rhs.m_indent;
    }
    else
    {
        return false;
    }
}

bool LayerId::operator < (const LayerId& rhs) const
{
    int minIndent = std::min(this->m_indent, rhs.m_indent);
    for(int i = 1; i <= minIndent; ++i)
    {
        IndexType myIndex = (*this)[i];
        IndexType rhsIndex = rhs[i];
        if(myIndex != rhsIndex)
        {
            return myIndex < rhsIndex;
        }
    }
    // all equal up to now
    if(this->m_indent != rhs.m_indent)
    {
        return this->m_indent < rhs.m_indent;
    }
    else
    {
        return false;
    }
}

bool LayerId::operator >= (const LayerId& rhs) const
{
    return !(*this < rhs);
}

bool LayerId::operator <= (const LayerId& rhs) const
{
    return !(*this > rhs);
}

bool LayerId::operator == (const LayerId& rhs) const
{
    if(m_indent != rhs.m_indent)
    {
        return false;
    }
    else
    {
        const IndexType* left = indexPointer();
        const IndexType* right = rhs.indexPointer();
		for (int i = 0; i < m_indent; i++)
        {
            if(*left++ != *right++)
            {
                return false;
            }
        }
        return true;
    }
}

bool LayerId::operator != (const LayerId& rhs) const
{
    return !(*this == rhs);
}

LayerId LayerId::deepestCommonId(const LayerId another)
{
    const IndexType* a = indexPointer();
    const IndexType* b = another.indexPointer();
    DepthType indent = std::min(m_indent, another.m_indent);
    DepthType i;
    for(i = 0; i < indent && a[i] == b[i]; ++i)
    {
    }
    return LayerId(i, a);
}
