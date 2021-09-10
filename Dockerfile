FROM ubuntu:20.04
MAINTAINER Shaobo He <polarishehn@gmail.com>

RUN apt-get update && \
      apt-get -y install \
      software-properties-common \
      wget \
      sudo

# Install LLVM 11
RUN wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    add-apt-repository "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-11 main" && \
    apt-get update && apt-get install -y clang-11

# Add the current directory to `/home/user/sgxray`
ADD . /root

# Install SGXRay
RUN (cd root; ./install.sh)
