FROM debian:12

RUN apt-get -y update
RUN apt-get -y upgrade

# C
RUN apt-get install -y \
    build-essential \
    gcc-aarch64-linux-gnu \
    clang-format \
    clang-tidy \
    cppcheck

# Python
RUN apt-get install -y \
    python3 \
    python3-pip \
    python3-ipython \
    pipenv

# Other tools
RUN apt-get install -y git
