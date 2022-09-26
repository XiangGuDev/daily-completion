
template<class Data>
CSecurityStackImpl<Data>::CSecurityStackImpl (void)
{
	_PeakValue = 0;
}
template<class Data>
void CSecurityStackImpl<Data>::Append (Data val)
{
	CCriticalSectionSync cs (_CSC);
	_Array.Add (val);
	_PeakValue = _Array.GetSize ();

}
template<class Data>
bool CSecurityStackImpl<Data>::RemoveTop (Data& val)
{
	CCriticalSectionSync cs (_CSC);

	if (!_Array.GetSize ())
		return false;

	val = _Array[0];
	_Array.RemoveAt (0);
	return true;
}
template<class Data>
bool CSecurityStackImpl<Data>::RemoveLast (Data& val)
{
	CCriticalSectionSync cs (_CSC);

	if (!_Array.GetSize ())
		return false;

	val = _Array[_Array.GetSize () - 1];
	_Array.RemoveAt (_Array.GetSize () - 1);
	return true;
}

template<class Data>
void CSecurityStackImpl<Data>::RemoveAll (void)
{
	CCriticalSectionSync cs (_CSC);

	_Array.RemoveAll ();
}

template<class Data>
int CSecurityStackImpl<Data>::GetPeakValue (void)
{
	return _PeakValue;
}

template<class Data>
int CSecurityStackImpl<Data>::GetCount (void)
{
	CCriticalSectionSync cs (_CSC);
	return _Array.GetSize ();
}
template<class Data>
Data& CSecurityStackImpl<Data>::GetAt (int nIndex)
{
	CCriticalSectionSync cs (_CSC);

	return _Array[nIndex];
}

template<class KeyType, class ValueType>
CSecurityMapImpl<KeyType, ValueType>::CSecurityMapImpl (void)
{
	_PeakValue = 0;
}
template<class KeyType, class ValueType>
int CSecurityMapImpl<KeyType, ValueType>::FindKey (const KeyType& key)
{
	CCriticalSectionSync cs (_CSC);

	return _Map.FindKey (key);
}
template<class KeyType, class ValueType>
BOOL CSecurityMapImpl<KeyType, ValueType>::Add (const KeyType& key, const ValueType& val)
{
	CCriticalSectionSync cs (_CSC);

	_PeakValue++;
	return _Map.Add (key, val);	
}
template<class KeyType, class ValueType>
KeyType& CSecurityMapImpl<KeyType, ValueType>::GetKeyAt (int nIndex)
{
	CCriticalSectionSync cs (_CSC);

	return _Map.GetKeyAt (nIndex);
}
template<class KeyType, class ValueType>
ValueType& CSecurityMapImpl<KeyType, ValueType>::GetValueAt (int nIndex)
{
	CCriticalSectionSync cs (_CSC);

	return _Map.GetValueAt (nIndex);
}
template<class KeyType, class ValueType>
int CSecurityMapImpl<KeyType, ValueType>::GetSize ()
{
	CCriticalSectionSync cs (_CSC);

	return _Map.GetSize ();
}
template<class KeyType, class ValueType>
bool CSecurityMapImpl<KeyType, ValueType>::RemoveLast (KeyType& key, ValueType& value)
{
	CCriticalSectionSync cs (_CSC);

	if (_Map.GetSize () == 0) return false;

	key = _Map.GetKeyAt (_Map.GetSize () -1);
	value = _Map.GetValueAt (_Map.GetSize () -1);
	return _Map.Remove (key) == TRUE ? true : false;
}

template<class KeyType, class ValueType>
bool CSecurityMapImpl<KeyType, ValueType>::RemoveTop (KeyType& key, ValueType& value)
{
	CCriticalSectionSync cs (_CSC);

	if (_Map.GetSize () == 0) return false;

	key = _Map.GetKeyAt (0);
	value = _Map.GetValueAt (0);
	return _Map.Remove (key) == TRUE ? true : false;
}

template<class KeyType, class ValueType>
int CSecurityMapImpl<KeyType, ValueType>::GetPeakValue (void)
{
	return _PeakValue;
}


template<class T, T DefaultValue>
CSecurityNumberImpl<T,DefaultValue>::CSecurityNumberImpl (void)
{
	_value = DefaultValue;
}

template<class T, T DefaultValue>
CSecurityNumberImpl<T,DefaultValue>::CSecurityNumberImpl (const T& d)
{
	_value = d;
}

template<class T, T DefaultValue>
typename CSecurityNumberImpl<T,DefaultValue>::ThisDefines& CSecurityNumberImpl<T,DefaultValue>::operator = (const T& d)
{
	CCriticalSectionSync cs (_CSC);

	_value = d;
	return *this;
}

template<class T, T DefaultValue>
typename CSecurityNumberImpl<T,DefaultValue>::ThisDefines& CSecurityNumberImpl<T,DefaultValue>::operator ++ (int d)
{
	CCriticalSectionSync cs (_CSC);

	_value ++;
	return *this;
}

template<class T, T DefaultValue>
typename CSecurityNumberImpl<T,DefaultValue>::ThisDefines& CSecurityNumberImpl<T,DefaultValue>::operator -- (int d)
{
	CCriticalSectionSync cs (_CSC);

	_value --;
	return *this;
}

template<class T, T DefaultValue>
CSecurityNumberImpl<T,DefaultValue>::operator T()
{
	CCriticalSectionSync cs (_CSC);

	return _value;
}
