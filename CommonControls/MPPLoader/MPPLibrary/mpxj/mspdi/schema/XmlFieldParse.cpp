#include "stdafx.h"
#include "XmlFieldParse.h"
#include "..\DatatypeConverter.h"
//#include "..\..\GarbageCollector.h"

namespace mspdi{
	namespace schema{

		std::map<CString, std::shared_ptr<NewXmlAdapter>> FieldValue::_typeAdapter;

		DatatypeConverter DataConvert;
		//static std::shared_ptr<DatatypeConverter> m_DataConvert;

		//static DatatypeConverter& getDataConvert()
		//{
		//	if (!m_DataConvert)
		//	{
		//		m_DataConvert.reset(new DatatypeConverter());
		//		GarbageCollector::add(new GcSharedPtr<DatatypeConverter>(m_DataConvert));
		//	}

		//	return *m_DataConvert;
		//}

		//// 释放所有 NewXmlAdapterT
		//class FreeXmlAdapter : public IGcRecycle
		//{
		//public:
		//	virtual void gc()
		//	{
		//		FieldValue::freeAllTypeAdapter();
		//	}
		//};

		///////////////////////////////////////////////////////////////////////////////////////////////////
		// FieldValue
		//

		FieldValue::FieldValue(TiXmlElement* element)
		{
			_element = element;
		}

		FieldValue::~FieldValue()
		{
			_cache.clear();
			//_typeAdapter.clear();
		}

		VariantPtr FieldValue::getField(LPCSTR name, int defType)
		{
			auto itMap = _cache.find(CString(name));
			if (itMap != _cache.end())
				return std::dynamic_pointer_cast<Variant>(itMap->second);
//			ObjectPtr cache;
// 			if (_cache.Lookup(CString(name), cache))
// 				return std::dynamic_pointer_cast<Variant>(cache);

			ATLASSERT(_element != NULL);

			VariantPtr cacheValue;

			TiXmlElement* child = _element->FirstChildElement(name);
			if (child)
			{
				const char* text = child->GetText();
				CString value = CA2W(text);
				if (defType == VT_R8)
					cacheValue = VariantPtr(new Variant(_ttof(value)));
				else if (defType == VT_I4)
				{
					if (value.Find('.') >= 0)
						cacheValue = VariantPtr(new Variant(_ttof(value)));
					else
						cacheValue = VariantPtr(new Variant(_ttol(value)));
				}
				else if (defType == VT_BOOL)
					cacheValue = VariantPtr(new Variant(value == L"1" || value == L"true"));
				else
					cacheValue = VariantPtr(new Variant(value));
				//_cache.put(CString(name), cacheValue);
				_cache[CString(name)] = cacheValue;
			}
			return cacheValue;
		}

		int FieldValue::getInt(LPCSTR name)
		{
			return NumberHelper::getInt(getField(name, VT_I4));
		}

		bool FieldValue::getBool(LPCSTR name)
		{
			return NumberHelper::getBool(getField(name, VT_BOOL));
		}

		CString FieldValue::getString(LPCSTR name)
		{
			return NumberHelper::getString(getField(name));
		}

		double FieldValue::getDouble(LPCSTR name)
		{
			return NumberHelper::getDouble(getField(name, VT_R8));
		}

		NumberPtr FieldValue::getNumber(LPCSTR name)
		{
			return getField(name, VT_I4);
		}

		DatePtr FieldValue::getDate(LPCSTR name)
		{
			::CalendarPtr cal = DataConvert.parseDateTime(getString(name));
			if (cal) return cal->getTime();
			return NULL;
		}

		DatePtr FieldValue::getTime(LPCSTR name)
		{
			::CalendarPtr cal = DataConvert.parseTime(getString(name));
			if (cal) return cal->getTime();
			return NULL;
		}

		ObjectPtr FieldValue::getChildObject(LPCSTR name)
		{
			auto itMap = _cache.find(CString(name));
			if (itMap != _cache.end())
				return itMap->second;

 			ObjectPtr cache;
// 			if (_cache.Lookup(CString(name), cache))
// 				return cache;

			ATLASSERT(_element != NULL);

			TiXmlElement* child = _element->FirstChildElement(name);
			if (child)
			{
				cache = instanceType(name, child);
				//_cache.put(CString(name), cache);
				_cache[CString(name)] = cache;
			}
			return cache;
		}

		// 设置对象值
		void FieldValue::set(LPCSTR name, bool value)
		{
			set(name, VariantPtr(new Variant(CString(value ? L"1" : L"0"))));
		}

		void FieldValue::set(LPCSTR name, int value)
		{
			//ATLASSERT(value != -65535);
			set(name, VariantPtr(new Variant(value)));
		}

		void FieldValue::set(LPCSTR name, double value)
		{
			set(name, VariantPtr(new Variant(value)));
		}

		void FieldValue::set(LPCSTR name, LPCTSTR value)
		{
			set(name, VariantPtr(new Variant(value)));
		}

		void FieldValue::set(LPCSTR name, VariantPtr value)
		{
			ObjectPtr cache;
			VariantPtr cache2;
			auto itMap = _cache.find(CString(name));
			if (itMap != _cache.end())
			//if (_cache.Lookup(CString(name), cache))
			{
				//cache2 = std::dynamic_pointer_cast<Variant>(cache);
				cache2 = std::dynamic_pointer_cast<Variant>(itMap->second);
				if (cache2 == NULL)
					throw toolkit::Exception(L"存在相同名称但类型不同的值。");
			}

			if (cache2 != NULL)
			{
				*cache2 = *value;
			}
			else
			{
				//_cache.put(CString(name), value);
				_cache[CString(name)] = value;
			}
		}

		// 设置子对象
		void FieldValue::set(LPCSTR name, FieldValuePtr value)
		{
			ObjectPtr cache;
			FieldValuePtr cache2;
			auto itMap = _cache.find(CString(name));
			if (itMap != _cache.end())
			//if (_cache.Lookup(CString(name), cache))
			{
				//cache2 = std::dynamic_pointer_cast<FieldValue>(cache);
				cache2 = std::dynamic_pointer_cast<FieldValue>(itMap->second);
				if (cache2 == NULL)
					throw toolkit::Exception(L"存在相同名称但类型不同的值。");
			}

			//_cache.put(CString(name), value);
			_cache[CString(name)] = value;
		}

		void  FieldValue::registerType(LPCSTR typeName, NewXmlAdapter* adapter)
		{
			//if (_typeAdapter.GetCount() == 0)
			//{
			//	// 添加到回收站
			//	GarbageCollector::add(new FreeXmlAdapter());
			//}
			//_typeAdapter.put(CString(typeName), std::shared_ptr<NewXmlAdapter>(adapter));
			_typeAdapter[CString(typeName)] = std::shared_ptr<NewXmlAdapter>(adapter);
		}

		bool FieldValue::isRegisterType(LPCSTR typeName)
		{
			return _typeAdapter.find(CString(typeName)) != _typeAdapter.end();
// 			std::shared_ptr<NewXmlAdapter> adapter;
// 			return _typeAdapter.Lookup(CString(typeName), adapter);
		}

		ObjectPtr FieldValue::instanceType(LPCSTR typeName, TiXmlElement* element)
		{
			std::shared_ptr<NewXmlAdapter> adapter;
			auto it = _typeAdapter.find(CString(typeName));
			if(it == _typeAdapter.end())
			//if (!_typeAdapter.Lookup(CString(typeName), adapter))
			{
				throw toolkit::Exception(L"XML结点类型对象不存在");
			}

			adapter = it->second;
			return ObjectPtr(adapter->NewObject(element));
		}

		void FieldValue::freeAllTypeAdapter()
		{
			//_typeAdapter.RemoveAll();
			_typeAdapter.clear();
		}

		// 获取有序的字段名称列表，用NULL结束，用于sortCache函数
		LPCTSTR* FieldValue::getFieldList()
		{
			return NULL;
		}

		// 把_cache转换成列表，并根据getFieldList的值进行排序
		std::shared_ptr<List<std::pair<CString, ObjectPtr>>> FieldValue::sortCacheFields()
		{
			if (_cache.size() == 0)
				return NULL;

			// 复制一份cache
			std::map<CString, ObjectPtr> tempCache;
			tempCache = _cache;
// 			POSITION pos = _cache.GetStartPosition();
// 			while (pos)
// 			{
// 				Map<CString, ObjectPtr>::CPair* pair = _cache.GetNext(pos);
// 				tempCache.put(pair->m_key, pair->m_value);
// 			}

			std::shared_ptr<List<std::pair<CString, ObjectPtr>>> fieldList (
								new List<std::pair<CString, ObjectPtr>>());

			// 获取取结点的字段名称列表
			LPCTSTR* fieldNames = getFieldList();

			if (fieldNames)
			{
				// 先根据字段顺序获取值
				int index = 0;
				while (fieldNames[index] != NULL)
				{
					LPCTSTR key = fieldNames[index];
// 					pos = tempCache.Lookup(key);
// 					if (pos != NULL)
// 					{
// 						Map<CString, ObjectPtr>::CPair* pair = tempCache.GetAt(pos);
// 						fieldList->Add(std::pair<CString, ObjectPtr>(pair->m_key, pair->m_value));
// 						tempCache.RemoveAtPos(pos);
// 					}
					auto itMap = tempCache.find(CString(key));
					if (itMap != tempCache.end())
					{
						fieldList->Add(std::pair<CString, ObjectPtr>(itMap->first, itMap->second));
						tempCache.erase(itMap);
					}
					index++;
				}
			}

			// 把剩下的字段再存入列表
// 			pos = tempCache.GetStartPosition();
// 			while (pos)
// 			{
// 				Map<CString, ObjectPtr>::CPair* pair = tempCache.GetNext(pos);
// 				fieldList->Add(std::pair<CString, ObjectPtr>(pair->m_key, pair->m_value));
// 			}
// 			tempCache.RemoveAll();

			for(auto itMap = tempCache.begin();  itMap != tempCache.end(); ++itMap)
				fieldList->Add(std::pair<CString, ObjectPtr>(itMap->first, itMap->second));
			tempCache.clear();

			return fieldList;
		}

		// 保存到XML结点中
		void FieldValue::saveElement(TiXmlElement* element)
		{
			saveFieldToXml(shared_from_this(), element);
		}

		void FieldValue::saveVariantToXml(ObjectPtr object, CString name, TiXmlElement* element)
		{
			if (object == NULL)
			{
				return;
			}

			// 添加值
			ATLASSERT(name.GetLength() > 0);
			CString content = object->toString();
			if (content.GetLength() > 0)
			{
				TiXmlElement newChild = TiXmlElement(CW2A(name));
				TiXmlText text = TiXmlText(CW2A(content));
				newChild.InsertEndChild(text);
				element->InsertEndChild(newChild);
			}
		}

		void FieldValue::saveFieldToXml(FieldValuePtr field, TiXmlElement* element)
		{
			std::shared_ptr<List<std::pair<CString, ObjectPtr>>> fieldList = field->sortCacheFields();
			for (size_t i = 0; i < fieldList->GetCount(); i++)
			{
				std::pair<CString, ObjectPtr> pair = fieldList->GetAt(i);
				CString name = pair.first;
				ObjectPtr value = pair.second;
				
				// 保存子结点
				FieldValuePtr testFieldValue = std::dynamic_pointer_cast<FieldValue>(value);
				if (testFieldValue != NULL)
				{
					TiXmlElement newChild = TiXmlElement(CW2A(name));
					testFieldValue->saveElement(&newChild);
					element->InsertEndChild(newChild);
					continue;
				}

				// 保存列表结点
				ListIteratorPtr testListIterator = std::dynamic_pointer_cast<ListIterator>(value);
				if (testListIterator != NULL)
				{
					saveListToXml(testListIterator, name, element);
					continue;
				}

				// 保存变量结点
				saveVariantToXml(value, name, element);
			}
		}

		void FieldValue::saveListToXml(ListIteratorPtr list, CString name, TiXmlElement* element)
		{
			if (list->get_count() == 0) return;
			for (size_t i = 0; i < list->get_count(); i++)
			{
				ObjectPtr listItem = list->get_item(i);
				if (listItem == NULL) continue;

				FieldValuePtr testFieldValue = std::dynamic_pointer_cast<FieldValue>(listItem);

				if (testFieldValue)
				{
					TiXmlElement newChild = TiXmlElement(CW2A(name));
					saveFieldToXml(testFieldValue, &newChild);
					element->InsertEndChild(newChild);
				}
				else
				{
					saveVariantToXml(listItem, name, element);
				}
					
			}
		}
	}
}