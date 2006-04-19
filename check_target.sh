#!/bin/sh
#################################################################
#
#       Check target script for the ERS package
#       Created by Sergei Kolos; 26.04.06
#
#################################################################

$1ers_test

if test $? -ne 13
then
        echo
        echo "*************** Check target failed ***************"
        exit 1
else
        echo
        echo "*************** Check target completed successfully ***************"
fi

