/*
 *  Issue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 08.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

/** \page ERS 
  * The Error Reporting System package offers basic facilites to do error handling. 
  * 
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
    *  \author Matthias Wiesmann
    *  \version 1.1
    *  \note Issue chaining is handled by copying the cause issue on the stack and keeping a pointer to it.
    *  When the object is destroyed, it destroys the pointed issue. This means we can only chain issues that 
    *  correctly implement the factory methods required by the IssueFactory class
    *  \see ers::IssueFactory
    *  \brief Root Issue class
    */
    
    class Issue : public std::exception  {  
	friend class Stream ; 
	friend class IssueFactory ; 
public:	
	static const char *CLASS_KEY ;                               /**< \brief key for class information */
	static const char *COMPILATION_TIME_KEY ;                    /**< \brief key for compilation time */
	static const char *COMPILATION_TARGET_KEY ;                  /**< \brief key for compilation target */
	static const char *COMPILER_KEY ;                            /**< \brief key for compilator type */
	static const char *CPP_CLASS_KEY ;                           /**< \brief key for c++ class (mangled) */
	static const char *ERS_VERSION_KEY ;                         /**< \brief key for ERS version */
	static const char *HOST_NAME_KEY ;                           /**< \brief key for hostname */
	static const char *HOST_TYPE_KEY ;                           /**< \brief key for host type (architecture / os) */
	static const char *MESSAGE_KEY ;                             /**< \brief key for human readable */
	static const char *PROCESS_ID_KEY ;                          /**< \brief key for the process id (number)*/
	static const char *PROCESS_PWD_KEY ;                         /**< \brief key for the process working directory */
	static const char *PROGRAM_NAME_KEY ;                        /**< \brief key for the name of the program */
	static const char *RESPONSIBILITY_KEY ;                      /**< \brief key for the responsibility of the issue (text) */
	static const char *SEVERITY_KEY ;                            /**< \brief key for the severity of the issue */
	static const char *SOURCE_POSITION_KEY ;                     /**< \brief key for position in the source code */
	static const char *TIME_KEY ;                                /**< \brief key for the time of the issue (text) */
	static const char *TRANSIENCE_KEY ;                          /**< \brief key for the transience of the issue (text) */
	static const char *USER_ID_KEY ;                             /**< \brief key for the user-id of the owner of the process */
	static const char *USER_NAME_KEY ;                           /**< \brief key for the user-name of the owner of the process */
	static const char *CAUSE_PSEUDO_KEY ;                        /**< \brief key used when serializing the cause issue, this key is not used in the value table */
	static const char *CAUSE_TEXT_KEY ;                          /**< \brief key used to store the cause issue's message */
	
	static const char *ISSUE_CLASS_NAME ;                        /**< \brief name of the class, used for serialisation */
	
protected:
	Issue *m_cause ;                                               /**< \brief Issue that caused the current issue */
	std::string m_human_description ;                              /**< \brief Human readable description */
	string_map_type m_value_table  ;                               /**< \brief Optional properties. */
	void insert(const Context *context) ;                          /**< \brief Inserts the context */
	void insert_hostname();                                        /**< \brief Inserts the hostname */
	void insert_processid();                                       /**< \brief Inserts process id */
	void insert_time() ;                                           /**< \brief Inserts current time */
	void insert_userid(); 
	void insert_pwd();                                             /**< \brief Inserts the current pwd */
	void insert_env(const char*env_key, const char* issue_key);    /**< \brief Inserts environnement variable */
	virtual std::string build_human_description() const;           /**< \brief Builds human description for Issue. */
        void setup_common(const Context *context) ;                    /**< \brief Sets up the common fields. */
        void finish_setup(const std::string &message) ;                /**< \brief Finishes the setup of the Issue */
        Issue(const Context &context, ers_severity s);                 /**< \brief Constructor for subclasses */
        void set_values(const string_map_type &values);                /**< \brief sets the value table */
public:
	Issue();  
	Issue(const Issue &issue); 
        Issue(const string_map_type &values); 
	Issue(const Context &context, ers_severity s, const std::string &message);   
        Issue(const Context &context, ers_severity s, const std::exception *cause); 
	virtual ~Issue() throw() ;
	Issue *clone() const ; 
	const Issue *cause() const throw() ;
	void cause(const std::exception *cause=0);                     /**< \brief Initialises the cause field */
	operator std::string() const ; 
	Issue operator=(const Issue &issue); 
	bool operator==(Issue other) ; 
	const std::string get_value(const std::string &key) const ;    /**< \brief Reads the property list. */
	int get_int_value(const std::string &key) const ; 
	int values_number() const ;                                    /**< \brief How many key / values */
	void set_value(const std::string &key, long value);            /**< \brief Sets a value (numerical) */
	void set_value(const std::string &key, const std::string &value); /**< \brief Sets a value (string) */
	void set_value(const std::string &key, const char* value) ;    /**< \brief Sets a value (c-string) */
	virtual const char *get_class_name() const throw() ;           /**< \brief Get key for class (used for serialisation)*/
	const string_map_type* get_value_table() const ; 
        ers_severity severity() const throw()  ;
        void severity(ers_severity s) ; 
	bool is_error(); 
	std::string severity_message() const ; 
	void responsibility(ers_responsibility r) ; 
        ers_responsibility responsibility() const throw() ; 
     	void transience(bool tr);
	int transience() const throw() ; 
	const std::string human_description() const throw()  ;        /**< \brief Human description message. */
        const char* what() const throw() ;                            /**< \brief Human description message. */
	
    } ; // Issue
    
    std::ostream& operator<<(std::ostream&, const Issue&);
    Stream& operator<<(Stream&, const Issue&); 
} // ers

#endif

