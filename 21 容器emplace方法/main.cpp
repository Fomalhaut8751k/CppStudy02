#include<iostream>
#include<vector>

using namespace std;

/*
c++11 STL  push/insert => emplace  都是添加
*/


class Test 
{
public:
	Test(int a) { cout << "Test(int)" << endl; }
	Test(int a, int b) { cout << "Test(int, int)" << endl; }
	Test(const Test& t) { cout << "Test(const Test&)" << endl; }
	Test(Test&& t) noexcept{ cout << "Test(Test&&)" << endl; }
	~Test() { cout << "~Test()" << endl; }
};

#if 0
int main() 
{
	Test t1(10);
	vector<Test> v;
	v.reserve(100);

	cout << "============================================" << endl;
	v.push_back(t1); // Test(const Test&)
					 
	v.emplace_back(t1);  // Test(const Test&)
	// 直接插入对象，两个是没有区别的
	cout << "============================================" << endl;
	v.push_back(Test(10));  // Test(int)
	                        // Test(Test&&)
		                    // ~Test()
	v.emplace_back(Test(10));  // Test(int)
	                           // Test(Test&&)
		                       // ~Test()
	// 同上
	cout << "============================================" << endl;
	v.push_back(10);  // Test(int)
	                  // Test(Test&&)
		              // ~Test()
	cout << "---------------" << endl;
	v.emplace_back(10);  // Test(int)
	v.emplace_back(10, 20);  // Test(int, int)
	// 不会产生额外的临时对象
	cout << "============================================" << endl;

	return 0;
}
#endif

#if 1
// emplace的实现

template<typename T>
struct MyAllocator 
{
public:
	T* allocate(size_t size) 
	{
		return (T*)malloc(sizeof(T) * size);
	}

	template<typename... Types>
	void construct(T* ptr, Types&&... args) 
	{	
		// args只是一个参数，而且是Test对象
		new(ptr) T(std::forward<Types>(args)...);
	}

	void destroy(T* ptr)
	{
		ptr->~T();
	}

	void deallocate(T* ptr) 
	{
		free(ptr);
	}
};

template<typename T, typename Alloc = MyAllocator<T>>
class MyVector 
{
public:
	MyVector() :vec_(nullptr), size_(0), idx_(0) 
	{	// 容器的构造函数	
	};

	~MyVector()
	{	// 容器的析构函数
		for (int i = 0; i < idx_; ++i)
		{
			allocator_.destroy(vec_ + i);
		}
		allocator_.deallocate(vec_);
	}

	void reserve(size_t size)
	{	// 预留空间
		vec_ = allocator_.allocate(size);
		size_ = size;
	}

	void push_back(const T& val) 
	{	// 带左值引用参数的插入
		allocator_.construct(vec_ + idx_, val);
		idx_++;
	}

	void push_back(T&& val)
	{	// 带右值引用参数的插入
		allocator_.construct(vec_ + idx_, std::move(val));
		idx_++;
	}

	template<typename... Types>
	void emplace_back(Types&&... args)  // 引用折叠(左值右值)，可变参
	{	// emplate的插入方法
		allocator_.construct(vec_ + idx_, std::forward<Types>(args)...);
		idx_++;
	}

private:
	T* vec_;
	int size_;
	int idx_;
	Alloc allocator_;
};

int main() 
{
	Test t1(10);
	MyVector<Test> v;
	v.reserve(100);

	cout << "============================================" << endl;
	v.push_back(t1); // Test(const Test&)
	v.emplace_back(t1);  // Test(const Test&)
	// 直接插入对象，两个是没有区别的
	cout << "============================================" << endl;
	v.push_back(Test(10));  // Test(int)
							// Test(Test&&)
							// ~Test()
	v.emplace_back(Test(10));  // Test(int)
							   // Test(Test&&)
							   // ~Test()
	// 同上
	cout << "============================================" << endl;

	v.push_back(10);  // Test(int)
					  // Test(Test&&)
					  // ~Test()
	cout << "---------------" << endl;
	v.emplace_back(10);  // Test(int)
	v.emplace_back(10, 20);  // Test(int, int)

	// 不会产生额外的临时对象
	cout << "============================================" << endl;

	return 0;
}

#endif