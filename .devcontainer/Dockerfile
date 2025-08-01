FROM ubuntu:24.04
WORKDIR /app

# avoid errors/configuration issues while installing other packages
ARG DEBIAN_FRONTEND=noninteractive
ARG TZ=Etc/UTC
RUN apt update && apt install -y tzdata

# install git for collaboration management
RUN apt update && apt install -y git

# install dependencies for building and running the project
RUN apt update && apt install -y ccache cmake curl less libncurses6 libssl-dev ninja-build pip pkg-config python3
RUN pip install --break-system-packages capstone colorama cxxfilt pyelftools python-Levenshtein toml watchdog

# install dependencies for code environment
RUN apt update && apt install -y clangd clang-format llvm

# install (outdated) libtinfo5, required for old clang version
RUN if [ "$(uname -m)" = "x86_64" ]; then \
        curl -o libtinfo5_6.3-2_amd64.deb http://archive.ubuntu.com/ubuntu/pool/universe/n/ncurses/libtinfo5_6.3-2_amd64.deb && \
        dpkg -i libtinfo5_6.3-2_amd64.deb && \
        rm -f libtinfo5_6.3-2_amd64.deb; \
    fi

ENTRYPOINT ["bash"]
