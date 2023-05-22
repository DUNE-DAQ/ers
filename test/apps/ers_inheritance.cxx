#include <iostream>

#include <ers/ers.hpp>

ERS_DECLARE_ISSUE( test, Base, "Exception test", ERS_EMPTY )

ERS_DECLARE_ISSUE_BASE(test,                                         // namespace name
		       daughter,                                     // issue name
                       test::Base,                                   // base issue name
		       "I'm inheriting ",                            // message
		       ERS_EMPTY,                                    // base class attributes
                       ERS_EMPTY                                     // this class attributes
)

ERS_DECLARE_ISSUE_BASE(other,                                        // namespace name
		       A,                                            // issue name
                       test::daughter,                               // base issue name
		       "I'm inheriting from daughter",               // message
		       ERS_EMPTY,                                    // base class attributes
                       ERS_EMPTY                                     // this class attributes
)

ERS_DECLARE_ISSUE_BASE(weird,                                        // namespace name
		       B,                                            // issue name
                       other::A,                                     // base issue name
		       "I'm inheriting from A",                      // message
		       ERS_EMPTY,                                    // base class attributes
                       ERS_EMPTY                                     // this class attributes
)




int main(int ac, char** av) {

  std::vector<std::unique_ptr<ers::Issue>> issues;
  issues.push_back( std::make_unique<test::Base>( ERS_HERE ) );
  issues.push_back( std::make_unique<test::daughter>(ERS_HERE) );
  issues.push_back( std::make_unique<other::A>(ERS_HERE) );
  issues.push_back( std::make_unique<weird::B>(ERS_HERE) );
  
  for ( const auto & i : issues ) {
    auto chain = i->get_class_inheritance();
    for ( const auto & c : chain ) {
      std::cout << c << "\t" ;
    }
    std::cout << std::endl;
  }
  
  return 0;
}
