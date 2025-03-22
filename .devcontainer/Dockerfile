FROM debian:latest

RUN apt-get -y update
RUN apt-get -y upgrade

# GNU C Compiler (gcc)
RUN apt-get install -y build-essential

# Python interpreter
RUN apt-get install -y python3 \
    && apt-get install -y python3-pip \
    && apt-get install -y python3-ipython
RUN apt-get install -y pipenv

# git
RUN apt-get install -y git
