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
#include "ers/util.h"
#include "ers/Stream.h"
#include "ers/Context.h"
#include "ers/IssueFactory.h"

namespace ers {
    
   /** This class is the root Issue object.  
    *  The class does not contain any fields with information, instead everything is stored in a hashmap 
    *  as key - value paris (both strings). 
    *  The object contains utility methods to allow the manipulation of those key / values and
    *  code to insert common values into it, like time, compilation information, host information etc.
    *  @author Matthias Wiesmann
    *  @version 1.0
    */
    
    class Issue : public std::exception  {  
	friend class Stream ; 
	friend class IssueFactory ; 
public:	
	static const char *CLASS_KEY ; 
	static const char *COMPILATION_TIME_KEY ; 
	static const char *COMPILATION_TARGET_KEY ;
	static const char *COMPILER_KEY ; 
	static const char *CPP_CLASS_KEY ; 
	static const char *ERS_VERSION_KEY ; 
	static const char *HOST_NAME_KEY ; 
	static const char *HOST_TYPE_KEY ; 
	static const char *MESSAGE_KEY ; 
	static const char *PROCESS_ID_KEY ; 
	static const char *PROCESS_PWD_KEY ; 
	static const char *PROGRAM_NAME_KEY ; 
	static const char *RESPONSIBILITY_KEY ; 
	static const char *SEVERITY_KEY ; 
	static const char *SOURCE_POSITION_KEY ;
	static const char *TIME_KEY ; 
	static const char *TRANSIENCE_KEY ; 
	static const char *USER_ID_KEY ; 
	static const char *USER_NAME_KEY ; 
	static const char *CAUSE_PSEUDO_KEY ; 
	static const char *CAUSE_TEXT_KEY ; 
	
	static const char *ISSUE_CLASS_NAME ; 
protected:
	std::string m_human_description ;                              /**< \brief Human readable description */
	string_map_type m_value_table  ;                               /**< \brief Optional properties. */
	void cause(const std::exception *cause=0);
	void insert(const Context *context) ;                          /**< \brief Inserts the context */
	void insert_hostname();                                        /**< \brief Inserts the hostname */
	void insert_processid();                                       /**< \brief Inserts process id */
	void insert_time() ;                                           /**< \brief Inserts current time */
	void insert_userid(); 
	void insert_env(const char*env_key, const char* issue_key);    /**< \brief Inserts environnement variable */
	virtual std::string build_human_description() const;           /**< \brief Builds human description for Issue. */
        void setup_common(const Context *context) ;                    /**< \brief Sets up the common fields. */
        void finish_setup(const std::string &message) ;                /**< \brief Finishes the setup of the Issue */
        Issue(const Context &context, ers_severity s);                 /**< \brief Constrcutor for subclasses */
        void set_values(const string_map_type &values); 
public:
	Issue();  
	Issue(const Issue &issue); 
        Issue(const string_map_type &values); 
	Issue(const Context &context, ers_severity s, const std::string &message);   
        Issue(const Context &context, ers_severity s, const std::exception *cause); 
	const std::string get_value(const std::string &key) const ;   /**< \brief Reads the property list. */
	virtual const char *get_class_name() const throw() ;          /**< \brief Get key for class (used for serialisation)*/
	const string_map_type* get_value_table() const ; 
        ers_severity severity() const throw()  ;
        void severity(ers_severity s) ; 
	std::string severity_message() const ; 
	void responsibility(ers_responsibility r) ; 
        ers_responsibility responsibility() const throw() ; 
     	void transience(bool tr);
	int transience() const throw() ; 
	const std::exception *cause() const throw() ;
	const std::string human_description() const throw()  ;        /**< \brief Human description message. */
        const char* what() const throw() ;                            /**< \brief Human description message. */
        virtual ~Issue() throw() {} ;
    } ; // Issue
    
    std::ostream& operator<<(std::ostream&, const Issue&);
    Stream& operator<<(Stream&, const Issue&); 
} // ers

#endif

