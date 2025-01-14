FROM ubuntu:24.04
WORKDIR /app

# avoid errors/configuration issues while installing other packages
ARG DEBIAN_FRONTEND=noninteractive
ARG TZ=Etc/UTC
RUN apt update
RUN apt install -y tzdata

# install git for collaboration management
RUN apt install -y git

# install dependencies for building and running the project
RUN apt install -y ccache clang cmake curl less libncurses6 libssl-dev ninja-build pip pkg-config python3-full xdelta3
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
RUN pip install --break-system-packages capstone colorama cxxfilt pyelftools python-Levenshtein toml watchdog

# install dependencies for code environment
RUN apt install -y clangd clang-format clang-tidy

# install (outdated) libtinfo5, required for old clang version
RUN curl -o libtinfo5_6.3-2_amd64.deb http://archive.ubuntu.com/ubuntu/pool/universe/n/ncurses/libtinfo5_6.3-2_amd64.deb && dpkg -i libtinfo5_6.3-2_amd64.deb && rm -f libtinfo5_6.3-2_amd64.deb

ENTRYPOINT ["bash"]
