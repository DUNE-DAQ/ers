/*
 *  Issue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 08.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_Issue_
#define ERS_Issue_

#include <map>
#include <string>
#include "ers/Core.h"
#include "ers/Stream.h"
#include "ers/Context.h"
#include "ers/IssueFactory.h"

namespace ers {
    
   /** This class is the root Issue object.  
    *  The class does not contain any fields with information, instead everything is stored in a hashmap 
    *  as key - value paris (both strings). 
    *  The object contains utility methods to allow the manipulation of those key / values and
    *  code to insert common values into it, like time, compilation information, host information etc.
    *  For an example of how to build an actual subclass of issue look at the source of ExampleIssue.
    *  \author Matthias Wiesmann
    *  \version 1.1
    *  \note Issue chaining is handled by copying the cause issue on the stack and keeping a pointer to it.
    *  When the object is destroyed, it destroys the pointed issue. This means we can only chain issues that 
    *  correctly implement the factory methods required by the IssueFactory class
    *  \see ers::IssueFactory
    *  \see ExampleIssue
    *  \brief Root Issue class
    */
    
    class Issue : public std::exception  {  
	friend class Stream ; 
	friend class IssueFactory ; 
public:	
	static const char *const CLASS_KEY ;                               /**< \brief key for class information */
	static const char *const COMPILATION_TIME_KEY ;                    /**< \brief key for compilation time */
	static const char *const COMPILATION_TARGET_KEY ;                  /**< \brief key for compilation target */
	static const char *const COMPILER_KEY ;                            /**< \brief key for compilator type */
	static const char *const CPP_CLASS_KEY ;                           /**< \brief key for c++ class (mangled) */
	static const char *const ERS_VERSION_KEY ;                         /**< \brief key for ERS version */
	static const char *const HOST_NAME_KEY ;                           /**< \brief key for hostname */
	static const char *const HOST_TYPE_KEY ;                           /**< \brief key for host type (architecture / os) */
	static const char *const HOST_IP_ADDR_KEY ;                        /**< \brief key for host ip address */
	static const char *const MESSAGE_KEY ;                             /**< \brief key for human readable */
	static const char *const PROCESS_ID_KEY ;                          /**< \brief key for the process id (number)*/
	static const char *const PROCESS_PWD_KEY ;                         /**< \brief key for the process working directory */
	static const char *const PROGRAM_NAME_KEY ;                        /**< \brief key for the name of the program */
	static const char *const RESPONSIBILITY_KEY ;                      /**< \brief key for the responsibility of the issue (text) */
	static const char *const SEVERITY_KEY ;                            /**< \brief key for the severity of the issue */
	static const char *const SOURCE_POSITION_KEY ;                     /**< \brief key for position in the source code */
	static const char *const TIME_KEY ;                                /**< \brief key for the time of the issue (text) */
	static const char *const TRANSIENCE_KEY ;                          /**< \brief key for the transience of the issue (text) */
	static const char *const USER_ID_KEY ;                             /**< \brief key for the user-id of the owner of the process */
	static const char *const USER_NAME_KEY ;                           /**< \brief key for the user-name of the owner of the process */
	static const char *const CAUSE_PSEUDO_KEY ;                        /**< \brief key used when serializing the cause issue, this key is not used in the value table */
	static const char *const CAUSE_TEXT_KEY ;                          /**< \brief key used to store the cause issue's message */
	
	static const char *const ISSUE_CLASS_NAME ;                        /**< \brief name of the class, used for serialisation */
	
protected:
	Issue *m_cause ;                                               /**< \brief Issue that caused the current issue */
	mutable std::string m_human_description ;                      /**< \brief Human readable description (cache)*/
	string_map_type m_value_table  ;                               /**< \brief Optional properties. */
	void insert(const Context *context) throw() ;                  /**< \brief Inserts the context */
	void insert_time() throw() ;                                   /**< \brief Inserts current time */
	void insert_env(const char*env_key, const char* issue_key) throw() ;  /**< \brief Inserts environnement variable */
        void setup_common(const Context *context) throw() ;            /**< \brief Sets up the common fields. */
        void finish_setup(const std::string &message) throw() ;        /**< \brief Finishes the setup of the Issue */
        Issue(const Context &context, ers_severity s);                 /**< \brief Constructor for subclasses */
        void set_values(const string_map_type &values) throw();        /**< \brief sets the value table */
public:
	Issue();  
	Issue(const Issue &issue); 
        Issue(const string_map_type &values); 
	Issue(const Context &context, ers_severity s, const std::string &message);   
        Issue(const Context &context, ers_severity s, const std::exception *cause); 
	virtual ~Issue() throw() ;
	Issue *clone() const ; 
	
	const Issue *cause() const throw() ;                           /**< \brief return the cause Issue of this Issue */
	void cause(const std::exception *cause=0);                     /**< \brief Initialises the cause field */
	operator std::string() const ;                                 /**< \brief Converts the issue into a string */
	
	Issue operator=(const Issue &issue);                           /**< \brief Affectation operator */
	bool operator==(const Issue &other) const throw();             /**< \brief Equality operator */
	const std::string& operator[](const std::string &key) const throw(); 
	
	const std::string &get_value(const std::string &key) const throw() ;    /**< \brief Reads the property list. */
	int get_int_value(const std::string &key) const throw() ;      /**< \brief Get a value of the table as an integer */
	int values_number() const ;                                    /**< \brief How many key / values */
	void set_value(const std::string &key, long value) throw() ;   /**< \brief Sets a value (numerical) */
	void set_value(const std::string &key, const std::string &value) throw() ; /**< \brief Sets a value (string) */
	void set_value(const std::string &key, const char* value) throw() ;        /**< \brief Sets a value (c-string) */
	
	virtual const char *get_class_name() const throw() ;           /**< \brief Get key for class (used for serialisation)*/
	const string_map_type* get_value_table() const ;               /**< \brief extract value table */
        ers_severity severity() const throw()  ;                       /**< \brief severity of the issue */
        void severity(ers_severity s) ;                                /**< \brief sets the severity of the issue */
	bool is_error();                                               /**< \brief is the issue an error (or fatal). */
	std::string severity_message() const ;                         /**< \brief message associated with the severity of the issue */
	void responsibility(ers_responsibility r) ;                    /**< \brief set the responsability of the issue */
        ers_responsibility responsibility() const throw() ;            /**< \brief get the responsability level of the issue */
     	void transience(bool tr);                                      /**< \brief sets if the issue is transient */
	int transience() const throw() ;                               /**< \brief is the issue transient */
	const std::string &human_description() const throw()  ;        /**< \brief Human description message. */
        const char* what() const throw() ;                             /**< \brief Human description message. */
	const std::string &message() const throw() ;                   /**< \brief Message */
    } ; // Issue
    
    std::ostream& operator<<(std::ostream&, const Issue&);
    Stream& operator<<(Stream&, const Issue&); 
} // ers

#endif

