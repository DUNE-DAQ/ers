


#include "ers/PosixIssue.h"

namespace ers {
    
class OpenFail : public PosixIssue {
protected:
    static std::string open_fail_message(const char* path, const std::string &host, const std::string &message) ;
    static std::string open_flag_message(int flags) ; 
public:
    static const char* PATH_KEY ; 
    static const char* FLAG_KEY ; 
    static const char* OPENFAIL_CLASS ; 
    OpenFail(); 
    OpenFail(const char *path, int flags, const Context &context) ; 
    virtual const char *get_class_name() const throw();
} ; // open_fail   
        
} // namespace ers 

