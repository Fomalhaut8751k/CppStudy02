#pragma warning( disable : 4996)
#include<iostream>
#include<vector>

using namespace std;


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

// operator+ version 1.0
//CMyString operator+(const CMyString& s1, const CMyString& s2) {
//	char* ptmp = new char[strlen(s1.mptr) + strlen(s2.mptr) + 1]; // newû�ж�Ӧ��delete
//	strcpy(ptmp, s1.mptr);
//	strcat(ptmp, s2.mptr);
//	//return CMyString(ptmp);  �޸�Ϊ
//	CMyString tmpStr(ptmp);  // �������漰���ڴ�Ŀ��ٺ��ͷ�
//	delete[] ptmp;
//	return tmpStr;
// 
//  /*
//		char* ptmp����ʱnew�����˿ռ�
//		CMyString tmpStr(ptmp)����Ĺ��캯�����new�����˿ռ�
//      Ȼ���ptmp������һ��һ��������tmpStr��mptr
//		�ٰ�ptmpɾ����������tmpStr����Ҫ��tmpStr��mptr������main����ջ֡����ʱ������
//      ���漰����new�����˿ռ䣬Ȼ��tmpStr���������Լ�main����ջ֡��ʱ���������
//  */
//}


// operator+ version 2.0
CMyString operator+(const CMyString& s1, const CMyString& s2) {
	// char* ptmp = new char[strlen(s1.mptr) + strlen(s2.mptr) + 1]; // newû�ж�Ӧ��delete
	CMyString tmpStr;
	tmpStr.mptr = new char[strlen(s1.mptr) + strlen(s2.mptr) + 1];
	strcpy(tmpStr.mptr, s1.mptr);
	strcat(tmpStr.mptr, s2.mptr);
	return tmpStr;  // ���ﴥ�����Ǵ���ֵ���ò����Ŀ�������
					// ��ֱ��main����ջ֡����ʱ�����pmtrָ��tmpStr��pmtr������
	                // Ȼ��tmpStr.pmtr = nullptr;
	                // ͬʱ����CMyString str3 = str1 + str2; �ִ������Ż�
	                // �൱��CMyString str3(tmpStr.ptmp);
	/*
		CMyString tmpStr; Ĭ�Ϲ����µ�mptrֻ��һ���ֽڵĴ�С
		���ſ��ٵ��㹻����......���أ���main����ջ֡�Ͽ���.....
		����ڵ�һ������һ�����new����
	*/
}



// cout
ostream& operator<<(ostream& cout, const CMyString& str) {
	cout << str.mptr;
	return cout;
}

CMyString GetString(CMyString& str) {  // ��������ʽ���Ρ�
	const char* pstr = str.c_str();
	CMyString tmpStr(pstr);  // 3. CMyString(const char*)
	return tmpStr;  // 4. CMyString(const CMyString&)
					// �������죬��main����ջ֡�Ͽ���һ����ʱ����
}					// 5. ~CMyString()  ����tmpStr������


CMyString GetString(CMyString& str, int label) {
	const char* pstr = str.c_str();
	return CMyString(pstr);  // ֱ�ӷ�����ʱ����
	/*
		Ȼ��
		CMyString str2 = GetString(str1, 1);���Ż�Ϊ
		CMyString str2 = CMyString(pstr);
		CMyString str2(pstr);
		��ֻʣ����CMyString
	*/
}

int main() {
#if 0
	CMyString str1("aaaaaaaaaaaaaaaaaaaaaaa");  // 1. CMyString(const char*)
	CMyString str2;  // 2. CMyString(const char*)
	str2 = GetString(str1); // 6. CMyString::operator=()
							// 7. ~CMyString()  ���ǿ���������main����ջ֡�ϵ���ʱ�����
	cout << str2.c_str() << endl;
#endif
#if 0
	CMyString str1("aaaaaaaaaaaaaaaaaaaaaaa");  // 1. CMyString(const char*)
	CMyString str2 = GetString(str1, 1); // 2. CMyString(const char*)

	cout << str2.c_str() << endl;
#endif
#if 0
	// ��ֵ����
	int a = 10;
	int& b = a;  // ��ֵ����
	// int&& c = a; // a������ֵ����ֵû�ڴ棬û����
	// int& c = 20;  // ��ֵ����Ҳ���ܰ���ֵ
	const int& c = 20;  /* �൱��int tmp = 20;
							const int& c = tmp;
						*/
	int&& d = 20; // ��ֵ����

	// һ����ֵ���ñ�������ֵ
	int& e = d;

	// ��ʱ����û������,���������ͷţ����Ե�����ֵ
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
	vec.reserve(10);  // Ԥ���ռ䣬����������

	vec.push_back(str1); // ��vector�Ͽ�������һ���µĶ���
	vec.push_back(CMyString("bbb"));  // ��ʱ������ֵ�����ô���ֵ���ò����Ŀ�������
	system("pause");

#endif
	return 0;
	// 8. ~CMyString()    str2
	// 9. ~CMyString()    str1
}