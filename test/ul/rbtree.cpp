#include <ul/rbtree.hpp>

struct foo {
	struct key {
		bool operator<(foo const& lhs) const { return true; }
		bool operator>(foo const& lhs) const { return true; }
	};

	bool operator<(foo const& lhs) const
	{
		return true;
	}

	void bar() { }
	void bar() const { }

	ul::rbtree_node node;
};

int main()
{
	ul::rbtree<foo, &foo::node> tree;
	ul::rbtree<foo, &foo::node> stree;
	ul::rbtree<foo, &foo::node> const& ctree = tree;
	ul::rbtree<foo, &foo::node>::iterator i;
	ul::rbtree<foo, &foo::node>::const_iterator ci;
	ul::rbtree<foo, &foo::node>::reverse_iterator ri;
	ul::rbtree<foo, &foo::node>::const_reverse_iterator cri;
	std::pair<ul::rbtree<foo, &foo::node>::iterator, bool> ir;
	bool b;
	foo v;

	i = tree.insert_equal(v);
	ir = tree.insert_unique(v);

	i = tree.begin();
	i = tree.end();
	ri = tree.rbegin();
	ri = tree.rend();
	ci = ctree.begin();
	ci = ctree.end();
	cri = ctree.rbegin();
	cri = ctree.rend();

	i = tree.find(foo::key());
	ci = ctree.find(foo::key());

	tree.remove(i);
	tree.remove(v);
	tree.remove(foo::key());

	tree.swap(stree);
	b = tree.empty();

	i->bar();
	ci->bar();

	return 0;
}
