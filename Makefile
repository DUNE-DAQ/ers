ifeq (${OSTYPE},linux)
        include linux.mk
endif
ifeq (${OSTYPE},darwin) 
        include darwin.mk
endif

include common.mk
