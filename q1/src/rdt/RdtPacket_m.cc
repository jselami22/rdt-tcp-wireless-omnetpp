//
// Generated file, do not edit! Created by opp_msgtool 6.2 from rtd/RdtPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "../rdt/RdtPacket_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(RdtPacket)

RdtPacket::RdtPacket(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

RdtPacket::RdtPacket(const RdtPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

RdtPacket::~RdtPacket()
{
}

RdtPacket& RdtPacket::operator=(const RdtPacket& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void RdtPacket::copy(const RdtPacket& other)
{
    this->seqNum = other.seqNum;
    this->ackNum = other.ackNum;
    this->isData_ = other.isData_;
    this->isAck_ = other.isAck_;
    this->isFin_ = other.isFin_;
    this->isRequest_ = other.isRequest_;
    this->payloadBytes = other.payloadBytes;
}

void RdtPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->seqNum);
    doParsimPacking(b,this->ackNum);
    doParsimPacking(b,this->isData_);
    doParsimPacking(b,this->isAck_);
    doParsimPacking(b,this->isFin_);
    doParsimPacking(b,this->isRequest_);
    doParsimPacking(b,this->payloadBytes);
}

void RdtPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->seqNum);
    doParsimUnpacking(b,this->ackNum);
    doParsimUnpacking(b,this->isData_);
    doParsimUnpacking(b,this->isAck_);
    doParsimUnpacking(b,this->isFin_);
    doParsimUnpacking(b,this->isRequest_);
    doParsimUnpacking(b,this->payloadBytes);
}

int RdtPacket::getSeqNum() const
{
    return this->seqNum;
}

void RdtPacket::setSeqNum(int seqNum)
{
    this->seqNum = seqNum;
}

int RdtPacket::getAckNum() const
{
    return this->ackNum;
}

void RdtPacket::setAckNum(int ackNum)
{
    this->ackNum = ackNum;
}

bool RdtPacket::isData() const
{
    return this->isData_;
}

void RdtPacket::setIsData(bool isData)
{
    this->isData_ = isData;
}

bool RdtPacket::isAck() const
{
    return this->isAck_;
}

void RdtPacket::setIsAck(bool isAck)
{
    this->isAck_ = isAck;
}

bool RdtPacket::isFin() const
{
    return this->isFin_;
}

void RdtPacket::setIsFin(bool isFin)
{
    this->isFin_ = isFin;
}

bool RdtPacket::isRequest() const
{
    return this->isRequest_;
}

void RdtPacket::setIsRequest(bool isRequest)
{
    this->isRequest_ = isRequest;
}

int RdtPacket::getPayloadBytes() const
{
    return this->payloadBytes;
}

void RdtPacket::setPayloadBytes(int payloadBytes)
{
    this->payloadBytes = payloadBytes;
}

class RdtPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_seqNum,
        FIELD_ackNum,
        FIELD_isData,
        FIELD_isAck,
        FIELD_isFin,
        FIELD_isRequest,
        FIELD_payloadBytes,
    };
  public:
    RdtPacketDescriptor();
    virtual ~RdtPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(RdtPacketDescriptor)

RdtPacketDescriptor::RdtPacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(RdtPacket)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

RdtPacketDescriptor::~RdtPacketDescriptor()
{
    delete[] propertyNames;
}

bool RdtPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RdtPacket *>(obj)!=nullptr;
}

const char **RdtPacketDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RdtPacketDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RdtPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 7+base->getFieldCount() : 7;
}

unsigned int RdtPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_seqNum
        FD_ISEDITABLE,    // FIELD_ackNum
        FD_ISEDITABLE,    // FIELD_isData
        FD_ISEDITABLE,    // FIELD_isAck
        FD_ISEDITABLE,    // FIELD_isFin
        FD_ISEDITABLE,    // FIELD_isRequest
        FD_ISEDITABLE,    // FIELD_payloadBytes
    };
    return (field >= 0 && field < 7) ? fieldTypeFlags[field] : 0;
}

const char *RdtPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "seqNum",
        "ackNum",
        "isData",
        "isAck",
        "isFin",
        "isRequest",
        "payloadBytes",
    };
    return (field >= 0 && field < 7) ? fieldNames[field] : nullptr;
}

int RdtPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "seqNum") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "ackNum") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "isData") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "isAck") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "isFin") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "isRequest") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "payloadBytes") == 0) return baseIndex + 6;
    return base ? base->findField(fieldName) : -1;
}

const char *RdtPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_seqNum
        "int",    // FIELD_ackNum
        "bool",    // FIELD_isData
        "bool",    // FIELD_isAck
        "bool",    // FIELD_isFin
        "bool",    // FIELD_isRequest
        "int",    // FIELD_payloadBytes
    };
    return (field >= 0 && field < 7) ? fieldTypeStrings[field] : nullptr;
}

const char **RdtPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RdtPacketDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RdtPacketDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    RdtPacket *pp = omnetpp::fromAnyPtr<RdtPacket>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void RdtPacketDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    RdtPacket *pp = omnetpp::fromAnyPtr<RdtPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'RdtPacket'", field);
    }
}

const char *RdtPacketDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    RdtPacket *pp = omnetpp::fromAnyPtr<RdtPacket>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RdtPacketDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    RdtPacket *pp = omnetpp::fromAnyPtr<RdtPacket>(object); (void)pp;
    switch (field) {
        case FIELD_seqNum: return long2string(pp->getSeqNum());
        case FIELD_ackNum: return long2string(pp->getAckNum());
        case FIELD_isData: return bool2string(pp->isData());
        case FIELD_isAck: return bool2string(pp->isAck());
        case FIELD_isFin: return bool2string(pp->isFin());
        case FIELD_isRequest: return bool2string(pp->isRequest());
        case FIELD_payloadBytes: return long2string(pp->getPayloadBytes());
        default: return "";
    }
}

void RdtPacketDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RdtPacket *pp = omnetpp::fromAnyPtr<RdtPacket>(object); (void)pp;
    switch (field) {
        case FIELD_seqNum: pp->setSeqNum(string2long(value)); break;
        case FIELD_ackNum: pp->setAckNum(string2long(value)); break;
        case FIELD_isData: pp->setIsData(string2bool(value)); break;
        case FIELD_isAck: pp->setIsAck(string2bool(value)); break;
        case FIELD_isFin: pp->setIsFin(string2bool(value)); break;
        case FIELD_isRequest: pp->setIsRequest(string2bool(value)); break;
        case FIELD_payloadBytes: pp->setPayloadBytes(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RdtPacket'", field);
    }
}

omnetpp::cValue RdtPacketDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    RdtPacket *pp = omnetpp::fromAnyPtr<RdtPacket>(object); (void)pp;
    switch (field) {
        case FIELD_seqNum: return pp->getSeqNum();
        case FIELD_ackNum: return pp->getAckNum();
        case FIELD_isData: return pp->isData();
        case FIELD_isAck: return pp->isAck();
        case FIELD_isFin: return pp->isFin();
        case FIELD_isRequest: return pp->isRequest();
        case FIELD_payloadBytes: return pp->getPayloadBytes();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'RdtPacket' as cValue -- field index out of range?", field);
    }
}

void RdtPacketDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RdtPacket *pp = omnetpp::fromAnyPtr<RdtPacket>(object); (void)pp;
    switch (field) {
        case FIELD_seqNum: pp->setSeqNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_ackNum: pp->setAckNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_isData: pp->setIsData(value.boolValue()); break;
        case FIELD_isAck: pp->setIsAck(value.boolValue()); break;
        case FIELD_isFin: pp->setIsFin(value.boolValue()); break;
        case FIELD_isRequest: pp->setIsRequest(value.boolValue()); break;
        case FIELD_payloadBytes: pp->setPayloadBytes(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RdtPacket'", field);
    }
}

const char *RdtPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr RdtPacketDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    RdtPacket *pp = omnetpp::fromAnyPtr<RdtPacket>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RdtPacketDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    RdtPacket *pp = omnetpp::fromAnyPtr<RdtPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RdtPacket'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

