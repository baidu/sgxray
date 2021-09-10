#!/bin/bash

INSTALL_DIR="/opt/sgxray"

# Parse command line options
while [[ $# > 0 ]]
do
  case "$1" in
  --prefix)
    INSTALL_DIR="${2%/}"
    shift
    shift
    ;;

  *)
    exit 1
    ;;
  esac
done

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SGXRAYENV=${INSTALL_DIR}/sgxray.environment


# Restore dirs
mkdir -p $INSTALL_DIR/share/smack/
cp -r ${ROOT_DIR}/models $INSTALL_DIR/share/smack/lib
cp -r ${ROOT_DIR}/include $INSTALL_DIR/share/smack/include 
cp ${ROOT_DIR}/tools/*.py $INSTALL_DIR/share/smack
cp -r ${ROOT_DIR}/tools/svcomp $INSTALL_DIR/share/smack
cp -r ${ROOT_DIR}/tools/bin $INSTALL_DIR/
mkdir -p $INSTALL_DIR/share/smack/lib/sgx-libs/
cp -r ${ROOT_DIR}/sdk/oe $INSTALL_DIR/share/smack/lib/sgx-libs/oe-libs

INSTALL_LLVM=0 BUILD_SMACK=0 TEST_SMACK=0 ${INSTALL_DIR}/bin/build.sh

mv $INSTALL_DIR/smack.environment $SGXRAYENV
echo export PATH=\"${INSTALL_DIR}/bin:\$PATH\" >> ${SGXRAYENV}
