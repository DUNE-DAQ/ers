ROOT = ../

INCLUDES 	= -I. -I${ROOT}/installed/include

LIBS =  -lpthread 

INSTALL_DIR = ${ROOT}/installed
INCLUDE_DIR = ${INSTALL_DIR}/include/
LIBRARY_DIR = ${INSTALL_DIR}/${BUILD}/lib
BINARY_DIR  = ${INSTALL_DIR}/${BUILD}/bin

LIB_PATH =  -L${LIBRARY_DIR} -L${BUILD}/ 

CC      = ${GPP} ${CCFLAGS} ${INCLUDES}
LD		= g++ ${LIB_PATH} ${LIBS}


ERS_HEADS = ers/util.h ers/Issue.h ers/IssueFactory.h ers/Context.h \
	 ers/Precondition.h ers/Assertion.h  ers/NotImplemented.h \
	 ers/Stream.h ers/STLOutStream.h ers/XMLOutStream.h \
	 ers/HumanStream.h ers/SyncStream.h ers/TabOutStream.h \
	 ers/PosixIssue.h ers/FileIssue.h ers/OpenFail.h 

ERS_SRCS = ${ERS_HEADS:ers/%.h=src/%.cxx}

ERS_OBJS = ${ERS_HEADS:ers/%.h=${BUILD}/%.o}

LIB_OBJS	 = ${ERS_OBJS}

LIB_NAME  = libers.dylib
           
all:   ${BUILD}/test

${BUILD}/${LIB_NAME}: ${LIB_OBJS}
	${LD} -dynamiclib ${LIB_OBJS} -dynamic-library -install_name ${LIB_NAME} -o $@
	
${BUILD}/Context.o: src/Context.cxx ers/Context.h
	${CC} $< -c -o $@
	
${BUILD}/util.o: src/util.cxx ers/util.h
	${CC} $< -c -o $@
	
${BUILD}/IssueFactory.o: src/IssueFactory.cxx ers/IssueFactory.h
	${CC} $<  -c -o$@
	
${BUILD}/Issue.o: src/Issue.cxx ers/Issue.h ers/Context.h ers/IssueFactory.h ers/Stream.h
	${CC} $<  -c -o $@
	
${BUILD}/test.o: test/test.cxx
	${CC} $<  -c -o $@
	
${BUILD}/%.o: src/%.cxx ${ERS_HEADS}
	${CC} $<  -c -o $@
	
${BINARY_BUILD}/test: ${BUILD}/test.o ${ERS_OBJS}
	${CC} $< ${ERS_OBJS} -o $@
		
clean:
	rm ${LIB_OBJS} ${BUILD}/${LIB_NAME}
	
install_head: 	
	install -C ${ERS_HEADS} ${INCLUDE_DIR}/ers/  
	
install_lib:
	install -C ${BUILD}/${LIB_NAME} ${LIBRARY_DIR}
	
install: install_lib install_head
	
doc: ${ERS_HEADS}
	${DOXYGEN} Doxyfile


	
