#pragma warning( disable : 4996)
#include<iostream>

using namespace std;

// ################################# CMyString #################################
class CMyString {
public:
	// 默认构造
	CMyString(const char* p = nullptr) {
		cout << "CMyString(const char*)" << endl;
		if (p == nullptr) {
			mptr = new char[1];
			*mptr = '\0';
		}
		else {
			mptr = new char[strlen(p) + 1];  // 有效字符个数+末尾'\0'
			strcpy(mptr, p);
		}
	}

	// 默认析构
	~CMyString() {
		cout << "~CMyString()" << endl;
		delete[] mptr;
		mptr = nullptr;
	}

	// 带左值引用参数的拷贝构造
	CMyString(const CMyString& str) {
		cout << "CMyString(const CMyString&)" << endl;
		mptr = new char[strlen(str.mptr) + 1];
		strcpy(mptr, str.mptr);
	}

	// 带右值引用参数的拷贝构造
	CMyString(CMyString&& str) {
		// 如果参数是临时对象的话，就会调用带右值引用参数的拷贝构造
		// 临时对象马上就要被析构掉了，因此.....
		cout << "CMyString(CMyString&&)" << endl;
		mptr = str.mptr;
		str.mptr = nullptr;
	}

	// 带左值引用参数的operator=
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

	// 带右值引用参数的operator=
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

	// 返回c_Str;
	const char* c_str() const {
		return mptr;
	}

	// 索引
	char& operator[](int index) {  // 可读，读出来可以修改
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
	T* allocate(size_t size) {  // 负责内存开辟
		return (T*)malloc(size * sizeof(T));
	}

	void deallocate(void* p) {  // 负责内存释放
		free(p);
	}

	//void construct(T* p, const T& val) {  // 负责对象构造
	//	cout << "construct(const T&)" << endl;
	//	new (p) T(val);   // 在指定的堆内存上创建变量
	//}

	//void construct(T* p, T&& val) {  // 负责对象构造
	//	cout << "construct(T&&)" << endl;
	//	new (p) T(std::move(val));   // 在指定的堆内存上创建变量
	//
	//}

	template<typename Ty>
	void construct(T* p, Ty&& val) {  // 负责对象构造
		cout << "construct(T&&)" << endl;
		new (p) T(std::forward<Ty>(val));   // 在指定的堆内存上创建变量
	}


	void destroy(T* p) {  // 负责对象析构
		p->~T();  // ~T()表示T类型的析构函数
	}
};

// ############################### Vector ######################################
template<typename T, typename Alloc = Allocator<T>>
class vector {
public:
	// 需要把内存开辟和对象构造分开处理
	vector(int size = 10) {
		_first = _allocator.allocate(size); // 内存开辟
		_last = _first;
		_end = _first + size;
	}

	// 析构容器有效的元素，然后释放_first指向的堆内存
	~vector() {
		//delete[] _first;
		for (T* p = _first; p != _last; ++p) {
			_allocator.destroy(p); // 把_first指针指向的数组的有效元素进行析构操作
		}
		_allocator.deallocate(_first);  // 释放堆上的数组内存
		_first = _last = _end = nullptr;
	}

	vector(const vector<T>& vec) {
		//_first(new T[vec._end - vec._fisrt]);
		int size = vec._end - vec._first;
		_first = _allocator.allocate(size);

		int len = vec._last - vec._first;

		for (int i = 0; i < vec._last - vec._first; i++) {
			// _first[i] = vec._first[i];
			_allocator.construct(_first + i, vec._first[i]);  // 在指定的堆内存上创建变量
		}
		_last = _first + len;
		_end = _first + size;
	}

	vector<T>& operator=(const vector<T>& vec) {
		if (this == &vec)
			return *this;
		//delete[] _first;
		for (T* p = _first; p != _last; p++) {
			_allocator.destroy(p); // 把_first指针指向的数组的有效元素进行析构操作
		}
		_allocator.deallocate(_first);  // 释放堆上的数组内存 

		int size = vec._end - vec._first;
		_first = _allocator.allocate(size);

		int len = vec._last - vec._first;

		for (int i = 0; i < vec._last - vec._first; i++) {
			// _first[i] = vec._first[i];
			_allocator.construct(_first + i, vec._first[i]);  // 在指定的堆内存上创建变量
		}
		_last = _first + len;
		_end = _first + size;
		return *this;
	}

	//void push_back(const T& vec) {
	//	//cout << "push_back(const T&)" << endl;
	//	if (full())
	//		expand();
	//	//verify(_last - 1, _last);  // 检查一下是否有效
	//	//*_last++ = vec;
	//	_allocator.construct(_last, vec);
	//	_last++;
	//}

	//void push_back(T&& vec) {  // 接收右值
	//	//cout << "push_back(T&&)" << endl;
	//	if (full())
	//		expand();
	//	//verify(_last - 1, _last);  // 检查一下是否有效
	//	//*_last++ = vec;
	//	_allocator.construct(_last, std::move(vec));
	//	_last++;
	//}
	template<typename Ty>  // 函数模板的类型推演 + 引用折叠
	void push_back(Ty&& vec) {  // Ty CMyString&/&&  引用折叠，传入左值引用就是左值引用
		                        // 传入右值引用就是右值引用
		if (full())
			expand();
		_allocator.construct(_last, std::forward<Ty>(vec));
		_last++;

		// forward：类型的完美转发，如果实参是左值引用，返回的就是左值引用
		//                          如果实参是右值引用，返回的就是右值引用
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
		// !=  必须有效，且容器类型一致
		bool operator!=(const iterator& it) const {
			if (_pVec == nullptr || _pVec != it._pVec) {
				throw "iterator incompatable!";
			}
			//cout << (_ptr == nullptr) << " " << (it._ptr == nullptr) << endl;
			return _ptr != it._ptr;
		}
		// 前置递增
		//iterator& operator++() {
		//	if (_pVec == nullptr) {  // 必须有效
		//		throw "iterator invalid!";
		//	}
		//	++_ptr;
		//	return *this;
		//}
		void operator++() {
			_ptr++;
		}
		// 解引用
		T& operator*() {
			//if (_pVec == nullptr) {  // 必须有效
			//	throw "iterator invalid!";
			//}
			return *_ptr;
		}
	private:
		T* _ptr;
		vector<T, Alloc>* _pVec;  // 当前迭代器是哪个容器的一些
	};

	iterator begin() { return iterator(this, _first); }
	iterator end() { return iterator(this, _last); }

	// 检查迭代器是否失效
	void verify(T* first, T* last) {
		Iterator_Base* pre = &this->_head;     // 头
		Iterator_Base* it = this->_head._next;  // 第一个
		while (it != nullptr) {  // 遍历链表
			if (it->_cur->_ptr > first && it->_cur->_ptr <= last) {
				// 迭代器失效，把iterator持有的容器指针置为nullptr
				it->_cur->_pVec = nullptr;
				// 删除当前迭代器节点，继续判断后面的迭代器节点是否失效
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

	// 自定义vector容器insert方法的实现
	iterator insert(iterator it, const T& val) {
		// 不考虑扩容，以及it._ptr的合法性
		verify(it._ptr - 1, _last);
		T* p = _last;
		while (p > it._ptr) {
			_allocator.construct(p, *(p - 1));  // 在当前位置p构造一个值为*(p-1)的新对象
			_allocator.destroy(p - 1);
			p--;
		}
		_allocator.construct(p, val);
		_last++;
		return iterator(this, p);  // 新的迭代器
	}

	// 自定义vector容器erase方法的实现
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
	T* _first;  // 指向数组起始的位置
	T* _last;  // 指向数组中有效元素的后继位置
	T* _end;  // 指向数组空间的后继位置
	Alloc _allocator;  // 定义空间配置器

	// 容器迭代器失效增加代码
	struct Iterator_Base {  // 一个链表结点，把所有迭代器记录下来
		Iterator_Base(iterator* c = nullptr, Iterator_Base* n = nullptr)
			:_cur(c), _next(n) {}
		iterator* _cur;  // 维护一个指向某个迭代器的指针
		Iterator_Base* _next;  // 下一个Iterator_Base
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
			_allocator.destroy(p); // 把_first指针指向的数组的有效元素进行析构操作
		}
		_allocator.deallocate(_first);  // 释放堆上的数组内存 

		_first = ptmp;
		_last = _first + size;
		_end = _first + 2 * size;
	}
};


int main() {
	CMyString str1 = "aaa";
	vector<CMyString> vec;

	cout << "--------------------------------" << endl;
	vec.push_back(str1); // 在vector上拷贝构造一个新的对象
	cout << "--------------------------------" << endl;
	vec.push_back(CMyString("bbb"));  // 临时对象，右值，调用带右值引用参数的拷贝构造
	cout << "--------------------------------" << endl;

	return 0;
}