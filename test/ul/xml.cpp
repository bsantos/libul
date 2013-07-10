#include <ul/xml.hpp>
#include <fstream>
#include <vector>

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "usage: " << argv[0] << " xml-file\n";
		return 1;
	}

	std::ifstream in(argv[1], std::ios::binary | std::ios::ate);
	if (!in) {
		std::cerr << "error: failed to open \"" << argv[1] << "\"\n";
		return 1;
	}

	std::vector<char> storage(in.tellg());
	in.seekg(0);
	in.read(storage.data(), storage.size());

	ul::xml::parse_iterator it(storage.data());
	ul::xml::parse_iterator ed(storage.data() + storage.size());
    ul::xml::doc xml;

    try {
		ul::xml::parse(it, ed, xml);

	} catch(ul::xml::parse_failure& e) {
		ul::xml::parse_range here(e.first, std::find(e.first, e.last, '\n'));

		std::cerr << "error["      << e.first.position()
		          << "]: expected " << e.what_
		          << " at \""   << here
		          << "\"\n";
	}

    ul::xml::generate(std::cout, xml);
    return 0;
}
