FROM ubuntu:25.10

RUN apt update && apt install -y \
    build-essential \
    gdb \
    sudo \
    libgl1-mesa-dev \
    x11-apps \
    libsdl2-2.0-0 \
    clang \
    lldb \
    lld \
    freedoom -y \
    llvm \
    llvm-dev \
    llvm-runtime \
    valgrind \
    libx11-dev \
    libsdl2-dev \
    fish \
    neovim \
    chocolate-doom \
    && rm -rf /var/lib/apt/lists/*

ENV DISPLAY=:0

WORKDIR /project

CMD ["fish"]
