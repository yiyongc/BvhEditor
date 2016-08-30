#pragma once
#include <string>

namespace cacani {

/*!
 * \brief A light weight class to represent a utf-8 string.
 * 
 * Conversion from/to QString.
 * \code
 * void toQString(const utf8string& src)
 * {
 *     QString s = QString::fromUtf8(src.rawData());
 *     // ... 
 * }
 * void fromQString(const QString& src)
 * {
 *     utf8string s(src.toUtf8().constData());
 *     // ...
 * }
 * \endcode
 * The same method can be applied to a string type that supports multi-byte
 * characters, e.g. NSString for Mac OS, and CString for Windows.
 *
 * Conversion from std::string to utf8string.
 * \code
 * void fromStdString(const std::string& src)
 * {
 *     // because std::string is ascii based and ascii is a pure subset of 
 *     // utf-8, so we can just use src as the raw data.
 *     utf8string s(src);
 *     // ...
 * }
 * void toStdString(const utf8string& src)
 * {
 *     // conversion to std::string is not supported as it might cause loss
 *     // of information.
 * }
 * \endcode
 * Note that conversion from utf8string to std::string is not supported 
 * as it would result in data loss.
 */
class utf8string
{
public:
    /*!
     * \brief Get the internal string representation of the raw data.
     * 
     * This function should not be called for normal use.
     */
    static std::string& getRawString(utf8string& s) { return s.m_raw; }
    static const std::string& getRawString(const utf8string& s) { return s.m_raw; }
    typedef std::string::iterator iterator;
    typedef std::string::const_iterator const_iterator;
    typedef std::string::size_type size_type;

    utf8string(){}
    explicit utf8string(const char* raw) : m_raw(raw) {}
    utf8string(const char* raw, size_type size) : m_raw(raw, size) {}
    explicit utf8string(const std::string& raw) : m_raw(raw) {}

    std::string::size_type rawSize() const { return m_raw.size(); }
    const_iterator rawBegin() const { return m_raw.begin(); }
    const_iterator rawEnd() const { return m_raw.end(); }
    iterator rawBegin() { return m_raw.begin(); }
    iterator rawEnd() { return m_raw.end(); }

    const char* rawData() const { return m_raw.c_str(); }
    char& operator[](std::string::size_type index) { return m_raw[index]; }
    const char& operator[](std::string::size_type index) const { return m_raw[index]; }
    bool operator==(const utf8string& other) const {return m_raw==other.m_raw;}
    void resetRaw() { m_raw = std::string(); }
    void resetRaw(const std::string& raw) { m_raw = raw; }
    void resetRaw(const char* raw) { m_raw = std::string(raw); }
    void resetRaw(const char* raw, size_type size) { m_raw = std::string(raw, size); }
    bool empty() const { return m_raw.empty(); }
private:
    std::string m_raw;
};

}
