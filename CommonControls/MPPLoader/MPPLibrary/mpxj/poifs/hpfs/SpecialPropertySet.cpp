#include "stdafx.h"
#include "SpecialPropertySet.h"


/**
    * <p>Creates a <code>SpecialPropertySet</code>.
    *
    * @param ps The property set to be encapsulated by the
    * <code>SpecialPropertySet</code>
    */
SpecialPropertySet::SpecialPropertySet(PropertySet* ps)
{
    //deleg.reset (new MutablePropertySet(ps));
	deleg = ps;
}



///**
//    * <p>Creates a <code>SpecialPropertySet</code>.
//    *
//    * @param ps The mutable property set to be encapsulated by the
//    * <code>SpecialPropertySet</code>
//    */
//SpecialPropertySet::SpecialPropertySet(MutablePropertySetPtr ps)
//{
//    deleg = ps;
//}
//
//
//
///**
//    * @see PropertySet#getByteOrder
//    */
//int SpecialPropertySet::getByteOrder()
//{
//    return deleg->getByteOrder();
//}
//
//
//
///**
//    * @see PropertySet#getFormat
//    */
//int SpecialPropertySet::getFormat()
//{
//    return deleg->getFormat();
//}
//
//
//
///**
//    * @see PropertySet#getOSVersion
//    */
//int SpecialPropertySet::getOSVersion()
//{
//    return deleg->getOSVersion();
//}
//
//
//
///**
//    * @see PropertySet#getClassID
//    */
//ClassIDPtr SpecialPropertySet::getClassID()
//{
//    return deleg->getClassID();
//}
//
//
//
///**
//    * @see PropertySet#getSectionCount
//    */
//int SpecialPropertySet::getSectionCount()
//{
//    return deleg->getSectionCount();
//}
//
//
//
///**
//    * @see PropertySet#getSections
//    */
//SectionListPtr SpecialPropertySet::getSections()
//{
//    return deleg->getSections();
//}
//
//
//
///**
//    * @see PropertySet#isSummaryInformation
//    */
//bool SpecialPropertySet::isSummaryInformation()
//{
//    return deleg->isSummaryInformation();
//}
//
//
//
///**
//    * @see PropertySet#isDocumentSummaryInformation
//    */
//bool SpecialPropertySet::isDocumentSummaryInformation()
//{
//    return deleg->isDocumentSummaryInformation();
//}
//
//
//
///**
//    * @see PropertySet#getSingleSection
//    */
//SectionPtr SpecialPropertySet::getFirstSection()
//{
//    return deleg->getFirstSection();
//}
//
//
///**
//    * @see org.apache.poi.hpsf.MutablePropertySet#addSection(org.apache.poi.hpsf.SectionPtr)
//    */
//void SpecialPropertySet::addSection(SectionPtr section)
//{
//    deleg->addSection(section);
//}
//
//
//
///**
//    * @see org.apache.poi.hpsf.MutablePropertySet#clearSections()
//    */
//void SpecialPropertySet::clearSections()
//{
//    deleg->clearSections();
//}
//
//
//
///**
//    * @see org.apache.poi.hpsf.MutablePropertySet#setByteOrder(int)
//    */
//void SpecialPropertySet::setByteOrder(int byteOrder)
//{
//    deleg->setByteOrder(byteOrder);
//}
//
//
//
///**
//    * @see org.apache.poi.hpsf.MutablePropertySet#setClassID(org.apache.poi.hpsf.ClassIDPtr)
//    */
//void SpecialPropertySet::setClassID(ClassIDPtr classID)
//{
//    deleg->setClassID(classID);
//}
//
//
//
///**
//    * @see org.apache.poi.hpsf.MutablePropertySet#setFormat(int)
//    */
//void SpecialPropertySet::setFormat(int format)
//{
//    deleg->setFormat(format);
//}
//
//
//
///**
//    * @see org.apache.poi.hpsf.MutablePropertySet#setOSVersion(int)
//    */
//void SpecialPropertySet::setOSVersion(int osVersion)
//{
//    deleg->setOSVersion(osVersion);
//}
//
//
//
///**
//    * @see org.apache.poi.hpsf.MutablePropertySet#toInputStream()
//    */
//BwProj::toolkit::DataReadStreamPtr SpecialPropertySet::toInputStream()
//{
//    return deleg->toInputStream();
//}
//
//
//
///**
//    * @see org.apache.poi.hpsf.MutablePropertySet#write(org.apache.poi.poifs.filesystem.DirectoryEntry, java.lang.CString)
//    */
//void SpecialPropertySet::write(DirectoryEntryPtr dir, CString name)
//{
//    deleg->write(dir, name);
//}
//
///**
//    * @see org.apache.poi.hpsf.MutablePropertySet#write(java.io.OutputStream)
//    */
//void SpecialPropertySet::write(IDataWriteStream* out)
//{
//    deleg->write(out);
//}
//
///**
//    * @see org.apache.poi.hpsf.PropertySet#equals(java.lang.Object)
//    */
//bool SpecialPropertySet::equals(Object* o)
//{
//    return deleg->equals(o);
//}
//
///**
//    * @see org.apache.poi.hpsf.PropertySet#getProperties()
//    */
//Property2ArrayPtr SpecialPropertySet::getProperties()
//{
//    return deleg->getProperties();
//}

/**
    * @see org.apache.poi.hpsf.PropertySet#getProperty(int)
    */
ObjectPtr SpecialPropertySet::getProperty(int id)
{
    return deleg->getProperty(id);
}



///**
//    * @see org.apache.poi.hpsf.PropertySet#getPropertyBooleanValue(int)
//    */
//bool SpecialPropertySet::getPropertyBooleanValue(int id)
//{
//    return deleg->getPropertyBooleanValue(id);
//}



/**
    * @see org.apache.poi.hpsf.PropertySet#getPropertyIntValue(int)
    */
int SpecialPropertySet::getPropertyIntValue(int id)
{
    return deleg->getPropertyIntValue(id);
}


    
/**
    * Fetches the property with the given ID, then does its
    *  best to return it as a CString
    * @return The property as a CString, or null if unavailable
    */
CString SpecialPropertySet::getPropertyStringValue(int propertyId)
{
    ObjectPtr propertyValue = getProperty(propertyId);
    return getPropertyStringValue(propertyValue);
}

CString SpecialPropertySet::getPropertyStringValue(ObjectPtr propertyValue)
{
    // Normal cases
    if (!propertyValue) return L"";
	return propertyValue->toString();

	//if (std::dynamic_pointer_cast<Variant>(propertyValue))
	//{
	//	return std::dynamic_pointer_cast<Variant>(propertyValue)->toString();
	//}
 //       
 //   // Do our best with some edge cases
 //   if (propertyValue instanceof byte[]) {
 //       byte[] b = (byte[])propertyValue;
 //       if (b.length == 0) {
 //           return "";
 //       }
 //       if (b.length == 1) {
 //           return Byte.toString(b[0]);
 //       }
 //       if (b.length == 2) {
 //           return Integer.toString( LittleEndian.getUShort(b) );
 //       }
 //       if (b.length == 4) {
 //           return Long.toString( LittleEndian.getUInt(b) );
 //       }
 //       // Maybe it's a string? who knows!
 //       return new CString(b);
 //   }
 //   return propertyValue.toString();
}

//
///**
//    * @see org.apache.poi.hpsf.PropertySet#hashCode()
//    */
//int SpecialPropertySet::hashCode()
//{
//    return deleg->hashCode();
//}
//
//
//
///**
//    * @see org.apache.poi.hpsf.PropertySet#toString()
//    */
//CString SpecialPropertySet::toString()
//{
//    return deleg->toString();
//}
//
//
//
///**
//    * @see org.apache.poi.hpsf.PropertySet#wasNull()
//    */
//bool SpecialPropertySet::wasNull()
//{
//    return deleg->wasNull();
//}