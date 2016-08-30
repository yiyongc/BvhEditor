/*****************************************************************************
Filename    : LayerId.h

*****************************************************************************/

#pragma once

#include <stdint.h>//1st run//

using namespace std;

namespace cacani { namespace data {
	 
class LayerGroup;

/*!
 * \brief A light weight value to locate a layer/Group within a cut.
 *
 * A LayerId is multi-level indices in nature. Just like normal indices, 
 * they might become obsolete or invalid after the targeting object's 
 * states changed.
 *
 * Therefore, be very cautious when storing a LayerId as a state
 * of another object, because its correctness has to be maintained 
 * constantly.
 *
 * Note that the full state of a LayerId is determined by all indices of
 * count its depth, rather than its indent.
 * For example, a LayerId with depth 4, and indent 2, it should have 4 
 * valid indices instead of 2. Incrementing/decrementing its indent within
 * the range of [0,4] does not affect the value of indices.
 */
class LayerId
{
    //friend struct SerializeInfo<LayerId>; 
    //template<class Archive> 
    //void serialize(Archive & ar, version_t /*version*/, bool /*verbose*/) 
    /*{
        using namespace cacani::serialization;

        ar & makeNvp("depth", m_depth);
        ar & makeNvp("indent", m_indent);

        if(m_depth > fixedSize)
        {
            assert(Archive::isOArchive || !m_indices.extended);
            assert(Archive::isIArchive || m_indices.extended);
            ar & makeNvpConst("indices", makeArray(m_indices.extended, m_depth));
        }
        else
        {
            IndexType* ptr = m_indices.native;
            ar & makeNvpConst("indices", makeArray(ptr, m_depth));
        }
    }*/
 
public:
    typedef uint16_t           DepthType;
    typedef uint16_t           IndexType; // save space
    enum IndexPosition
    {
        kFront,
        kBack
    };

private:

	//used to be 4, now changed to 2 based on limited needs
    //static const DepthType fixedSize = 4;
	static const DepthType fixedSize = 2;

    //BOOST_STATIC_ASSERT(fixedSize >= 4);

    /*!
     * Serves as either a fixed size array of indices OR a pointer to extended
     * index array.
     *
     * When m_depth is less than LayerId::native, m_indices is used as an fixed
     * size array of IndexType, otherwise it is used as normal i.e. a pointer to
     * a dynamically size array.
     *
     * Since depth is likely to be not more than fixedSize, dynamic memory allocation
     * is unlikely to happen, which effectively improves the performance.
     */
    union IndexUnion
    {
        IndexType* extended;      // pointer to extended index array
        IndexType  native[fixedSize]; // native index array
    };

public:
    ~LayerId()
    {
        if(m_depth > fixedSize && m_indices.extended)
        {
            delete [] m_indices.extended;
        }
    }

    LayerId() : m_depth(0), m_indent(0)
    {
        m_indices.extended = 0;
    }

    explicit LayerId(DepthType indent, const IndexType* indices = 0)
        : m_depth(indent), m_indent(indent)
    {
        m_indices.extended = 0;
        initIndices(indices);
    }

    LayerId(const LayerId& r)
        : m_depth(r.m_depth), m_indent(r.m_indent)
    {
        m_indices.extended = 0;
        if(r.m_depth > fixedSize)
        {
            initIndices(r.m_indices.extended);
        }
        else
        {
            initIndices(r.m_indices.native);
        }
    }

    LayerId& operator = (const LayerId& rhs);
    bool operator == (const LayerId& rhs) const;
    bool operator > (const LayerId& rhs) const;
    bool operator >= (const LayerId& rhs) const;
    bool operator < (const LayerId& rhs) const;
    bool operator <= (const LayerId& rhs) const;
    bool operator != (const LayerId& rhs) const;
    
    inline DepthType depth() const { return m_depth; }

    inline DepthType indent() const { return m_indent; }

    inline IndexType index() const
    {
        return (*this)[m_indent];
    }

	inline IndexType forceRootIndex() const
	{
		return (*this)[1];
	}

    inline void incrementIndex()
    {
        if(m_indent == 0)
        {
            throw runtime_error("indent is 0; no index to increment");
        }
        ++(indexPointer()[m_indent - 1]);
    }

    inline void decrementIndex()
    {
        if(m_indent == 0)
        {
            throw std::runtime_error("indent is 0; no index to increment");
        }
        --(indexPointer()[m_indent - 1]);
    }

    LayerId deepestCommonId(const LayerId another);

    inline void setIndex(IndexType index)
    {
        if(m_indent == 0)
        {
            throw runtime_error("indent is 0; no index to increment");
        }
        indexPointer()[m_indent - 1] = index;
    }

    inline void setIndent(DepthType indent)
    {
        if(indent > m_depth)
        {
            //throw std::out_of_range("indent out of range");
            resizeIndices(indent); // m_depth gets updated inside
        }
        m_indent = indent;
    }

    inline void decrementIndent()
    {
        setIndent(m_indent - 1);
    }

    inline void incrementIndent()
    {
        setIndent(m_indent + 1);
    }

    void addIndex(IndexType an_index, IndexPosition pos = kBack)
    {
        resizeIndices(m_depth + 1, &an_index, pos);
    }

    void addIndices(const IndexType* indices, DepthType count,
       IndexPosition pos = kBack)
    {
        resizeIndices(m_depth + count, indices, pos);
    }

    void removeIndex(DepthType count, IndexPosition pos = kBack)
    {
        if(m_depth - count < 0)
        {
            throw out_of_range("insufficient indices to remove");
        }
        resizeIndices(m_depth - count, 0, pos);
    }

    /*!
     * \brief Trim off the indices after current indent, if any.
     */
    void trim()
    {
        if(m_indent < m_depth)
        {
            resizeIndices(m_indent, 0, kBack);
        }
    }

    IndexType operator [] (DepthType indent) const
    {
        if(indent == 0)
        {
            throw out_of_range("no index at indent 0");
        }
        else if(indent > m_depth)
        {
            throw out_of_range("indent out of range");
        }
        return indexPointer()[indent - 1];
    }

    IndexType& operator [] (DepthType indent)
    {
        if(indent == 0)
        {
            throw std::out_of_range("no index at indent 0");
        }
        else if(indent > m_depth)
        {
            throw std::out_of_range("indent out of range");
        }
        return indexPointer()[indent - 1];
    }
    
    template<class Root, class Node> 
    void resolve(const Root& root, Node*& result) const
    {
		result = dynamic_cast<Node*>(resolve(root));
    }

    template<class Root, class Node> 
    void resolve(const Root& root, const Node*& result) const
    {
        result = dynamic_cast<Node*>(resolve(root));
    }

    template<class Root, class Node>
    void metaResolve(const Root& root, const Node*& result) const
    {
        const MetaObj<LayerGroup>* layerGroup = root.template cast<LayerGroup>();
        result= reinterpret_cast<Node*>(metaResolve(*layerGroup));
    }

    template<class Root, class Node>
    void metaResolve(Root& root, Node*& result) const
    {
        const MetaObj<LayerGroup>* layerGroup = root.template cast<LayerGroup>();
        result= reinterpret_cast<Node*>(metaResolve(*layerGroup));
    }

    template<class Root, class Node> 
    void resolveOrNull(Root& root, Node*& result) const
    {
        result = dynamic_cast<Node*>(resolveOrNull(root));
    }

    template<class Root, class Node> 
    void resolveOrNull(const Root& root, const Node*& result) const
    {
        result = dynamic_cast<Node*>(resolveOrNull(root));
    }

	public:
	inline const IndexType* indexPointer() const
	{
		if (m_depth > fixedSize)
		{
			return m_indices.extended;
		}
		else
		{
			return m_indices.native;
		}
	}


    

    inline IndexType* indexPointer()
    {
        if(m_depth > fixedSize)
        {
            return m_indices.extended;
        }
        else
        {
            return m_indices.native;
        }
    }

	private:

    void initIndices(const IndexType* src);

    void resizeIndices(DepthType new_depth, 
                        const IndexType* extra = 0,
                        IndexPosition pos = kBack);

    //CacObj* resolve(const LayerGroup& root) const;
    //CacObj* resolveOrNull(const LayerGroup& root) const;

    //MetaObjCacObj* metaResolve(const MetaObj<LayerGroup>& root) const;

private:
    DepthType m_depth;  // number of levels of depth
    DepthType m_indent; // current node indent

    /*!
     * \brief Indices or pointer to indices
     */
    IndexUnion m_indices;
};


class invalid_layerid : public std::runtime_error   
{
public:
    invalid_layerid(const LayerId& lid) : runtime_error("invalid layer id"), m_lid(lid) { }
    invalid_layerid(const LayerId& lid, const std::string& msg) 
        : runtime_error(msg), m_lid(lid) { }
    ~invalid_layerid() throw() {}
private:
    LayerId m_lid;
};


//CAC_BASIC_TYPE_INFO_EXTERNAL(LayerId, TID_LAYER_ID, "LayerId", 0)


//typedef ValueObj<LayerId> LayerIdObj;


//CAC_POLYMORPH_TYPE_INFO_EXTERNAL(LayerIdObj, TID_LAYER_ID_OBJECT, "LayerIdObject", 0)

}}

std::ostream& operator << (std::ostream& os, const cacani::data::LayerId& lid);
