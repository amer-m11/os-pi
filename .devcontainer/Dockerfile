FROM debian:12

RUN apt-get -y update
RUN apt-get -y upgrade

# GCC and Binutils
RUN apt-get install -y build-essential

# Cross Compiler Dependencies
RUN apt-get install -y bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev

# Python
RUN apt-get install -y python3 python3-pip python3-ipython pipenv

# Other tools
RUN apt-get install -y git wget

# Add cross-compiler script
ADD build-cross-compiler.sh /usr/local/bin/build-cross-compiler.sh
RUN chmod +x /usr/local/bin/build-cross-compiler.sh

ENV PATH="/opt/cross/bin:${PATH}"
