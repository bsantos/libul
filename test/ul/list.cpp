#include <ul/list.hpp>
#include <boost/ref.hpp>

struct foo {
	void bar() { }
	void bar() const { }

	ul::list_node node;
};

int main()
{
	ul::list<foo, &foo::node> list;
	ul::list<foo, &foo::node> slist;
	ul::list<foo, &foo::node> const& clist = list;
	ul::list<foo, &foo::node>::pointer p;
	ul::list<foo, &foo::node>::iterator i;
	ul::list<foo, &foo::node>::const_iterator ci;
	ul::list<foo, &foo::node>::reverse_iterator ri;
	ul::list<foo, &foo::node>::const_reverse_iterator cri;
	bool b;
	foo v;

	list.push_back(v);
	list.push_front(v);

	p = list.pop_back();
	p = list.pop_front();

	foo& r1 = list.front();
	foo& r2 = list.back();
	foo const& r3 = clist.front();
	foo const& r4 = clist.back();
	(void)r1;
	(void)r2;
	(void)r3;
	(void)r4;

	i = list.begin();
	i = list.end();
	ri = list.rbegin();
	ri = list.rend();
	ci = clist.begin();
	ci = clist.end();
	cri = clist.rbegin();
	cri = clist.rend();

	list.remove(i);
	list.remove(v);

	list.reverse();
	list.swap(slist);
	b = list.empty();

	i->bar();
	ci->bar();

	return 0;
}
