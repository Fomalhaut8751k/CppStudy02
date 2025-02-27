#pragma warning( disable : 4996)
#include<iostream>
#include<vector>

using namespace std;


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

// operator+ version 1.0
//CMyString operator+(const CMyString& s1, const CMyString& s2) {
//	char* ptmp = new char[strlen(s1.mptr) + strlen(s2.mptr) + 1]; // new没有对应的delete
//	strcpy(ptmp, s1.mptr);
//	strcat(ptmp, s2.mptr);
//	//return CMyString(ptmp);  修改为
//	CMyString tmpStr(ptmp);  // 这里有涉及了内存的开辟和释放
//	delete[] ptmp;
//	return tmpStr;
// 
//  /*
//		char* ptmp定义时new开辟了空间
//		CMyString tmpStr(ptmp)本身的构造函数里边new开辟了空间
//      然后把ptmp的数据一个一个拷贝给tmpStr的mptr
//		再把ptmp删除掉，返回tmpStr，又要把tmpStr的mptr拷贝到main函数栈帧的临时对象上
//      又涉及到了new开辟了空间，然后tmpStr析构掉，以及main函数栈帧临时对象的析构
//  */
//}


// operator+ version 2.0
CMyString operator+(const CMyString& s1, const CMyString& s2) {
	// char* ptmp = new char[strlen(s1.mptr) + strlen(s2.mptr) + 1]; // new没有对应的delete
	CMyString tmpStr;
	tmpStr.mptr = new char[strlen(s1.mptr) + strlen(s2.mptr) + 1];
	strcpy(tmpStr.mptr, s1.mptr);
	strcat(tmpStr.mptr, s2.mptr);
	return tmpStr;  // 这里触发的是带右值引用参数的拷贝构造
					// 就直接main函数栈帧上临时对象的pmtr指向tmpStr的pmtr的内容
	                // 然后tmpStr.pmtr = nullptr;
	                // 同时由于CMyString str3 = str1 + str2; 又触发了优化
	                // 相当于CMyString str3(tmpStr.ptmp);
	/*
		CMyString tmpStr; 默认构造下的mptr只有一个字节的大小
		接着开辟到足够长度......返回，在main函数栈帧上开辟.....
		相对于第一版少了一个大的new开辟
	*/
}



// cout
ostream& operator<<(ostream& cout, const CMyString& str) {
	cout << str.mptr;
	return cout;
}

CMyString GetString(CMyString& str) {  // 【引用形式传参】
	const char* pstr = str.c_str();
	CMyString tmpStr(pstr);  // 3. CMyString(const char*)
	return tmpStr;  // 4. CMyString(const CMyString&)
					// 拷贝构造，在main函数栈帧上开辟一个临时对象
}					// 5. ~CMyString()  这是tmpStr的析构


CMyString GetString(CMyString& str, int label) {
	const char* pstr = str.c_str();
	return CMyString(pstr);  // 直接返回临时对象
	/*
		然后：
		CMyString str2 = GetString(str1, 1);被优化为
		CMyString str2 = CMyString(pstr);
		CMyString str2(pstr);
		就只剩下了CMyString
	*/
}

int main() {
#if 0
	CMyString str1("aaaaaaaaaaaaaaaaaaaaaaa");  // 1. CMyString(const char*)
	CMyString str2;  // 2. CMyString(const char*)
	str2 = GetString(str1); // 6. CMyString::operator=()
							// 7. ~CMyString()  这是拷贝出来再main函数栈帧上的临时对象的
	cout << str2.c_str() << endl;
#endif
#if 0
	CMyString str1("aaaaaaaaaaaaaaaaaaaaaaa");  // 1. CMyString(const char*)
	CMyString str2 = GetString(str1, 1); // 2. CMyString(const char*)

	cout << str2.c_str() << endl;
#endif
#if 0
	// 右值引用
	int a = 10;
	int& b = a;  // 左值引用
	// int&& c = a; // a不是右值，右值没内存，没名字
	// int& c = 20;  // 左值引用也不能绑定右值
	const int& c = 20;  /* 相当于int tmp = 20;
							const int& c = tmp;
						*/
	int&& d = 20; // 右值引用

	// 一个右值引用本身是左值
	int& e = d;

	// 临时变量没有名字,除了语句就释放，可以当成右值
	const CMyString& cmys = CMyString("averio");
	CMyString&& cmys1 = CMyString("5003");
#endif
#if 0
	CMyString str1 = "pdc";
	CMyString str2 = "HelloWorld";
	cout << "-------------------------------------" << endl;
	CMyString str3 = str1 + str2;
	cout << "-------------------------------------" << endl;
	cout << str3 << endl;
#endif
#if 1
	CMyString str1 = "aaa";
	vector<CMyString> vec; 
	vec.reserve(10);  // 预留空间，避免多次扩容

	vec.push_back(str1); // 在vector上拷贝构造一个新的对象
	vec.push_back(CMyString("bbb"));  // 临时对象，右值，调用带右值引用参数的拷贝构造
	system("pause");

#endif
	return 0;
	// 8. ~CMyString()    str2
	// 9. ~CMyString()    str1
}