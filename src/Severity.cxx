#include <ers/ers.h>
#include <sstream>

namespace 
{
    const char * SeverityNames[] =
    { 
	"DEBUG",
	"INFO",
	"WARNING",
	"ERROR",
	"FATAL"
    };
}

ERS_DECLARE_ISSUE(	ers,
			BadSeverity,
			"string \"" << severity << "\" does not contain valid severity",
                        ((std::string)severity) )


/** 
 * \brief Transforms a severity type into the corresponding string
 * \param s severity
 * \return reference to string with associated text 
 */
const std::string
ers::to_string( ers::severity severity )
{
    ERS_RANGE_CHECK( ers::Debug, severity, ers::Fatal )
    return SeverityNames[severity];
}

/** 
 * \brief Transforms a severity type into the corresponding string
 * \param s severity
 * \return pointer to string with associated text 
 */
const std::string
ers::to_string( ers::Severity severity )
{
    ERS_RANGE_CHECK( ers::Debug, severity, ers::Fatal )

    if ( severity.type == ers::Debug )
    {
	std::ostringstream out;
	out << SeverityNames[severity] << "_" << severity.rank;
	return out.str();
    }
    return SeverityNames[severity.type];
}

/** Parses a string and extracts a severity 
 * \param s the string to parse 
 * \return a severity value
 */
ers::severity
ers::parse( const std::string & string, ers::severity & s )
{
    for( ers::Severity ss = ers::Debug; ss <= ers::Fatal; ++((size_t&)ss) )
    {
	if ( string == SeverityNames[ss] )
	{
            return ( s = ss );
	}
    }
    throw ers::BadSeverity( ERS_HERE, string ); 
}

/** Parses a string and extracts a severity 
 * \param s the string to parse 
 * \return a severity value
 */
ers::Severity
ers::parse( const std::string & string, ers::Severity & s )
{
    int level = 0;
    ers::severity type;
    std::string::size_type pos = string.find( '_' );
    if ( pos )
    {
        std::istringstream in( string.substr( pos + 1 ) );
        in >> level;
	ers::parse( string.substr( 0, pos ), type );
    }
    else
    {
	ers::parse( string, type );
    }
    return ( s = ers::Severity( type, level ) );
}

