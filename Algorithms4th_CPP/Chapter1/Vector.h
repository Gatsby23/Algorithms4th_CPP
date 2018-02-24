#pragma once
#include<memory>
//�ο�C++ Primerʵ��Vector
//��Ҫ�����˿������ƺ�����ʹ��

template<typename Item>
class Vector
{
public:
	/**
	*Ĭ�Ϲ��캯��
	*/
	Vector() {}
	/**
	*��������
	*/
	Vector(const Vector &s)
	{
		auto newdata = alloc_n_copy(s.begin(), s.end());
		elements = newdata.first;
		first_free = cap = newdata.second;
	}

	Vector &operator=(const Vector &rhs)
	{
		auto newdata = alloc_n_copy(rhs.begin(), rhs.end());
		free();
		elements = newdata.first;
		first_free = cap = newdata.second;
		return *this;
	}

	Vector(Vector &&s) noexcept
		:elements(s.elements), first_free(s.first_free), cap(s.cap)
	{
		s.elements = s.first_free = s.cap = nullptr;
	}

	Vector &operator=(Vector &&rhs) noexcept
	{
		if (this != rhs)
		{
			free();

			//take over resources from rhs
			elements = rhs.elements;
			first_free = rhs.first_free;
			cap = rhs.cap;

			//leave rhs in a destructible state
			rhs.elements = rhs.first_free = rhs.cap = nullptr;
		}

		return *this;
	}

	~Vector() noexcept
	{
		free();
	}

	/**
	*list assignment
	*/
	Vector &operator=(std::initializer_list<Item> il)
	{
		auto newdata = alloc_n_copy(il.begin(), il.end());
		free();
		elements = newdata.first;
		first_free = cap = data.second;
	}

	/**
	*add item to vector
	*/
	void push_back(const Item &s)
	{
		chk_n_alloc();
		alloc.construct(first_free++, s);
	}

	void push_back(Item &&s)
	{
		chk_n_alloc();
		alloc.construct(first_free++, std::move(s));
	}

	template<class...Args> void emplace_back(Args&&... args)
	{
		chk_n_alloc();
		alloc.construct(first_free++, std::forward<Args>(args)...);
	}

	/**
	*size and capacity
	*/
	size_t size() const { return first_free - elements; }
	size_t capacity() const { return cap - elements; }

	/**
	*item access
	*/
	Item &operator[](size_t n) { return elements[n]; }
	const Item &operator[](size_t n) const { return elements[n]; }

	/**
	*iterator interface
	*/
	Item* begin() const { return elements; }
	Item* end() const { return first_free; }

private:
	static std::allocator<Item> alloc;

	/**
	*check whether have memory,
	*if not, reallocate
	*/
	void chk_n_alloc()
	{
		if (first_free == cap)
			reallocate();
	}

	std::pair<Item*, Item*> alloc_n_copy(const Item *b, const Item *e)
	{
		auto data = alloc.allocate(e - b);
		return{ data, std::uninitialized_copy(b,e,data) };
	}

	/**
	*����Ԫ�أ��ͷ��ڴ�
	*/
	void free()
	{
		//destory the older items in reverse order
		for (auto p = first_free; p != elements;/*empty*/)
			alloc.destroy(--p);

		//deallocate memory
		if (elements)
			alloc.deallocate(elements, cap - elements);
	}

	void reallocate()
	{
		auto newcapacity = size() ? 2 * size() : 2;

		//allocate new space
		auto first = alloc.allocate(newcapacity);
		auto dest = first;
		auto elem = elements;

		//move the elements
		for (size_t i = 0; i != size(); ++i)
			alloc.construct(dest++, std::move(*elem++));
		free();

		elements = first;
		first_free = dest;
		cap = elements + newcapacity;
	}


	Item *elements = nullptr;//ָ��������Ԫ�صĵ�ַ
	Item* first_free = nullptr;//ָ�������һ������Ԫ��
	Item* cap = nullptr;//ָ������β��λ�õ�ָ��
};
//definition for the static data member
template<typename Item> std::allocator<Item> Vector<Item>::alloc;

