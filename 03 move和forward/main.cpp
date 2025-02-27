#pragma warning( disable : 4996)
#include<iostream>

using namespace std;

// ################################# CMyString #################################
class CMyString {
public:
	// Ĭ�Ϲ���
	CMyString(const char* p = nullptr) {
		cout << "CMyString(const char*)" << endl;
		if (p == nullptr) {
			mptr = new char[1];
			*mptr = '\0';
		}
		else {
			mptr = new char[strlen(p) + 1];  // ��Ч�ַ�����+ĩβ'\0'
			strcpy(mptr, p);
		}
	}

	// Ĭ������
	~CMyString() {
		cout << "~CMyString()" << endl;
		delete[] mptr;
		mptr = nullptr;
	}

	// ����ֵ���ò����Ŀ�������
	CMyString(const CMyString& str) {
		cout << "CMyString(const CMyString&)" << endl;
		mptr = new char[strlen(str.mptr) + 1];
		strcpy(mptr, str.mptr);
	}

	// ����ֵ���ò����Ŀ�������
	CMyString(CMyString&& str) {
		// �����������ʱ����Ļ����ͻ���ô���ֵ���ò����Ŀ�������
		// ��ʱ�������Ͼ�Ҫ���������ˣ����.....
		cout << "CMyString(CMyString&&)" << endl;
		mptr = str.mptr;
		str.mptr = nullptr;
	}

	// ����ֵ���ò�����operator=
	CMyString& operator=(const CMyString& str) {
		cout << "CMyString::operator=(const CMyString& str)" << endl;
		if (this == &str) {
			return *this;
		}
		delete[] mptr;
		mptr = new char[strlen(str.mptr) + 1];
		strcpy(mptr, str.mptr);
		return *this;
	}

	// ����ֵ���ò�����operator=
	CMyString& operator=(CMyString&& str) {
		cout << "CMyString::operator=(CMyString&& str)" << endl;
		if (this == &str) {
			return *this;
		}
		delete[] mptr;
		mptr = str.mptr;
		str.mptr = nullptr;
		return *this;
	}

	// >
	bool operator>(const CMyString& s2) const {
		return strcmp(mptr, s2.mptr) > 0;
	}

	// <
	bool operator<(const CMyString& s2) const {
		return strcmp(mptr, s2.mptr) < 0;
	}

	// ==
	bool operator==(const CMyString& s2) const {
		return strcmp(mptr, s2.mptr) == 0;
	}

	int length() const {
		return strlen(mptr);
	}

	// ����c_Str;
	const char* c_str() const {
		return mptr;
	}

	// ����
	char& operator[](int index) {  // �ɶ��������������޸�
		return mptr[index];
	}

private:
	char* mptr;

	// operator+
	friend CMyString operator+(const CMyString& s1, const CMyString& s2);
	// cout
	friend ostream& operator<<(ostream& cout, const CMyString& str);
};

CMyString operator+(const CMyString& s1, const CMyString& s2) {
	CMyString tmpStr;
	tmpStr.mptr = new char[strlen(s1.mptr) + strlen(s2.mptr) + 1];
	strcpy(tmpStr.mptr, s1.mptr);
	strcat(tmpStr.mptr, s2.mptr);
	return tmpStr; 
}

// cout
ostream& operator<<(ostream& cout, const CMyString& str) {
	cout << str.mptr;
	return cout;
}

CMyString GetString(CMyString& str) {  
	const char* pstr = str.c_str();
	CMyString tmpStr(pstr);  
	return tmpStr;
}					

CMyString GetString(CMyString& str, int label) {
	const char* pstr = str.c_str();
	return CMyString(pstr);
}

// ############################### allocator ####################################
template<typename T>
class Allocator {
public:
	T* allocate(size_t size) {  // �����ڴ濪��
		return (T*)malloc(size * sizeof(T));
	}

	void deallocate(void* p) {  // �����ڴ��ͷ�
		free(p);
	}

	//void construct(T* p, const T& val) {  // ���������
	//	cout << "construct(const T&)" << endl;
	//	new (p) T(val);   // ��ָ���Ķ��ڴ��ϴ�������
	//}

	//void construct(T* p, T&& val) {  // ���������
	//	cout << "construct(T&&)" << endl;
	//	new (p) T(std::move(val));   // ��ָ���Ķ��ڴ��ϴ�������
	//
	//}

	template<typename Ty>
	void construct(T* p, Ty&& val) {  // ���������
		cout << "construct(T&&)" << endl;
		new (p) T(std::forward<Ty>(val));   // ��ָ���Ķ��ڴ��ϴ�������
	}


	void destroy(T* p) {  // �����������
		p->~T();  // ~T()��ʾT���͵���������
	}
};

// ############################### Vector ######################################
template<typename T, typename Alloc = Allocator<T>>
class vector {
public:
	// ��Ҫ���ڴ濪�ٺͶ�����ֿ�����
	vector(int size = 10) {
		_first = _allocator.allocate(size); // �ڴ濪��
		_last = _first;
		_end = _first + size;
	}

	// ����������Ч��Ԫ�أ�Ȼ���ͷ�_firstָ��Ķ��ڴ�
	~vector() {
		//delete[] _first;
		for (T* p = _first; p != _last; ++p) {
			_allocator.destroy(p); // ��_firstָ��ָ����������ЧԪ�ؽ�����������
		}
		_allocator.deallocate(_first);  // �ͷŶ��ϵ������ڴ�
		_first = _last = _end = nullptr;
	}

	vector(const vector<T>& vec) {
		//_first(new T[vec._end - vec._fisrt]);
		int size = vec._end - vec._first;
		_first = _allocator.allocate(size);

		int len = vec._last - vec._first;

		for (int i = 0; i < vec._last - vec._first; i++) {
			// _first[i] = vec._first[i];
			_allocator.construct(_first + i, vec._first[i]);  // ��ָ���Ķ��ڴ��ϴ�������
		}
		_last = _first + len;
		_end = _first + size;
	}

	vector<T>& operator=(const vector<T>& vec) {
		if (this == &vec)
			return *this;
		//delete[] _first;
		for (T* p = _first; p != _last; p++) {
			_allocator.destroy(p); // ��_firstָ��ָ����������ЧԪ�ؽ�����������
		}
		_allocator.deallocate(_first);  // �ͷŶ��ϵ������ڴ� 

		int size = vec._end - vec._first;
		_first = _allocator.allocate(size);

		int len = vec._last - vec._first;

		for (int i = 0; i < vec._last - vec._first; i++) {
			// _first[i] = vec._first[i];
			_allocator.construct(_first + i, vec._first[i]);  // ��ָ���Ķ��ڴ��ϴ�������
		}
		_last = _first + len;
		_end = _first + size;
		return *this;
	}

	//void push_back(const T& vec) {
	//	//cout << "push_back(const T&)" << endl;
	//	if (full())
	//		expand();
	//	//verify(_last - 1, _last);  // ���һ���Ƿ���Ч
	//	//*_last++ = vec;
	//	_allocator.construct(_last, vec);
	//	_last++;
	//}

	//void push_back(T&& vec) {  // ������ֵ
	//	//cout << "push_back(T&&)" << endl;
	//	if (full())
	//		expand();
	//	//verify(_last - 1, _last);  // ���һ���Ƿ���Ч
	//	//*_last++ = vec;
	//	_allocator.construct(_last, std::move(vec));
	//	_last++;
	//}
	template<typename Ty>  // ����ģ����������� + �����۵�
	void push_back(Ty&& vec) {  // Ty CMyString&/&&  �����۵���������ֵ���þ�����ֵ����
		                        // ������ֵ���þ�����ֵ����
		if (full())
			expand();
		_allocator.construct(_last, std::forward<Ty>(vec));
		_last++;

		// forward�����͵�����ת�������ʵ������ֵ���ã����صľ�����ֵ����
		//                          ���ʵ������ֵ���ã����صľ�����ֵ����
	}

	void pop_back() {
		if (empty())
			return;
		verify(_last - 1, _last);
		--_last;
		_allocator.destroy(_last);
	}

	T back() const {
		if (empty())
			throw "vector is empty";
		return *(_last - 1);
	}

	void show() const {
		for (int* p = _first; p != _last; p++) {
			cout << *p << " ";
		}cout << endl;
	}

	bool full() const { return _last == _end; }
	bool empty() const { return _last == _first; }
	int size() const { return _last - _first; }

	T operator[](int index) {
		if (index < 0 || index >= size()) {
			throw "OutOfRangeException";
		}
		return _first[index];
	}

	class iterator {
	public:
		friend class vector<T, Alloc>;
		iterator(vector<T, Alloc>* pvec, T* ptr = nullptr)
			:_ptr(ptr), _pVec(pvec) {
			Iterator_Base* itb =
				new Iterator_Base(this, _pVec->_head._next);
			_pVec->_head._next = itb;
		}
		// !=  ������Ч������������һ��
		bool operator!=(const iterator& it) const {
			if (_pVec == nullptr || _pVec != it._pVec) {
				throw "iterator incompatable!";
			}
			//cout << (_ptr == nullptr) << " " << (it._ptr == nullptr) << endl;
			return _ptr != it._ptr;
		}
		// ǰ�õ���
		//iterator& operator++() {
		//	if (_pVec == nullptr) {  // ������Ч
		//		throw "iterator invalid!";
		//	}
		//	++_ptr;
		//	return *this;
		//}
		void operator++() {
			_ptr++;
		}
		// ������
		T& operator*() {
			//if (_pVec == nullptr) {  // ������Ч
			//	throw "iterator invalid!";
			//}
			return *_ptr;
		}
	private:
		T* _ptr;
		vector<T, Alloc>* _pVec;  // ��ǰ���������ĸ�������һЩ
	};

	iterator begin() { return iterator(this, _first); }
	iterator end() { return iterator(this, _last); }

	// ���������Ƿ�ʧЧ
	void verify(T* first, T* last) {
		Iterator_Base* pre = &this->_head;     // ͷ
		Iterator_Base* it = this->_head._next;  // ��һ��
		while (it != nullptr) {  // ��������
			if (it->_cur->_ptr > first && it->_cur->_ptr <= last) {
				// ������ʧЧ����iterator���е�����ָ����Ϊnullptr
				it->_cur->_pVec = nullptr;
				// ɾ����ǰ�������ڵ㣬�����жϺ���ĵ������ڵ��Ƿ�ʧЧ
				pre->_next = it->_next;
				delete it;
				it = pre->_next;
			}
			else {
				pre = it;
				it = it->_next;
			}
		}
	}

	// �Զ���vector����insert������ʵ��
	iterator insert(iterator it, const T& val) {
		// ���������ݣ��Լ�it._ptr�ĺϷ���
		verify(it._ptr - 1, _last);
		T* p = _last;
		while (p > it._ptr) {
			_allocator.construct(p, *(p - 1));  // �ڵ�ǰλ��p����һ��ֵΪ*(p-1)���¶���
			_allocator.destroy(p - 1);
			p--;
		}
		_allocator.construct(p, val);
		_last++;
		return iterator(this, p);  // �µĵ�����
	}

	// �Զ���vector����erase������ʵ��
	iterator erase(iterator it) {
		verify(it._ptr - 1, _last);
		T* p = it._ptr;
		while (p < _last - 1) {
			_allocator.destroy(p);
			_allocator.construct(p, *(p + 1));
			++p;
		}
		_allocator.destroy(p);
		_last--;
		return iterator(this, it._ptr);
	}

private:
	T* _first;  // ָ��������ʼ��λ��
	T* _last;  // ָ����������ЧԪ�صĺ��λ��
	T* _end;  // ָ������ռ�ĺ��λ��
	Alloc _allocator;  // ����ռ�������

	// ����������ʧЧ���Ӵ���
	struct Iterator_Base {  // һ�������㣬�����е�������¼����
		Iterator_Base(iterator* c = nullptr, Iterator_Base* n = nullptr)
			:_cur(c), _next(n) {}
		iterator* _cur;  // ά��һ��ָ��ĳ����������ָ��
		Iterator_Base* _next;  // ��һ��Iterator_Base
	};

	Iterator_Base _head;

	void expand() {
		int size = _end - _first;
		//T* _newfirst = new T[newsize];
		T* ptmp = _allocator.allocate(2 * size);

		for (int i = 0; i < size; ++i) {
			//_newfirst[i] = _first[i];
			_allocator.construct(ptmp + i, _first[i]);
		}

		//delete[] _first;
		for (T* p = _first; p != _last; ++p) {
			_allocator.destroy(p); // ��_firstָ��ָ����������ЧԪ�ؽ�����������
		}
		_allocator.deallocate(_first);  // �ͷŶ��ϵ������ڴ� 

		_first = ptmp;
		_last = _first + size;
		_end = _first + 2 * size;
	}
};


int main() {
	CMyString str1 = "aaa";
	vector<CMyString> vec;

	cout << "--------------------------------" << endl;
	vec.push_back(str1); // ��vector�Ͽ�������һ���µĶ���
	cout << "--------------------------------" << endl;
	vec.push_back(CMyString("bbb"));  // ��ʱ������ֵ�����ô���ֵ���ò����Ŀ�������
	cout << "--------------------------------" << endl;

	return 0;
}