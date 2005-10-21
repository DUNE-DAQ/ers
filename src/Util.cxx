#include <ers/internal/Util.h>

void
ers::tokenize(	const std::string & text,
		const std::string & separators,
		std::vector<std::string> & result )
{
    std::string::size_type start_p, end_p;
    start_p = 0;
    do
    {
	end_p = text.find_first_of(separators,start_p);
	if (end_p == std::string::npos)
        {
	    end_p = text.length(); 
	}
	result.push_back( text.substr( start_p, end_p - start_p ) );
	start_p = text.find_first_not_of( separators, end_p );
    }
    while( start_p != std::string::npos );
}




