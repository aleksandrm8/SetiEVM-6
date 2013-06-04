#include <string>

class Base64Coder  
{

public:

	static inline bool Base64Coder::is_base64(unsigned char c);
	static std::string base64_encode(unsigned char const* , unsigned int len);
	static std::string base64_decode(std::string const& s);

};
