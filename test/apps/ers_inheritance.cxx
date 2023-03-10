#include <iostream>

#include <ers/ers.hpp>

ERS_DECLARE_ISSUE( test, ExceptionTest, "Exception test", ERS_EMPTY )

ERS_DECLARE_ISSUE_BASE(test,                                         // namespace name
		       daughter,                                     // issue name
                       test::ExceptionTest,                          // base issue name
		       "I'm inheriting ",                            // message
		       ERS_EMPTY,                                    // base class attributes
                       ERS_EMPTY                                     // this class attributes
)



int main(int ac, char** av) {

  test::ExceptionTest a( ERS_HERE );

  test::daughter b( ERS_HERE );

  std::cout << a.inheritance_chain() << std::endl;
  std::cout << b.inheritance_chain() << std::endl;

    
  return 0;
}
