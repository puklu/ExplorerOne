# Use an official Ubuntu base image
FROM ubuntu:latest

# Set the working directory
WORKDIR /workspace

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages
RUN apt-get update && apt-get install -y \
    gcc-arm-none-eabi \
    gdb-multiarch \
    openocd \
    make \
    git \
    cmake \
    cppcheck \
    clang-format \
    build-essential \
    libusb-1.0-0-dev \
    libncurses5-dev \
    libncursesw5-dev \
    python3 \
    python3-pip \
    python3-venv \
    wget \
    vim \
    nano \
    htop \
    fish \
    minicom \
    doxygen \
    graphviz \
    default-jdk \
    wget \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Download PlantUML
RUN wget https://github.com/plantuml/plantuml/releases/download/v1.2025.1/plantuml-1.2025.1.jar -O /usr/local/bin/plantuml.jar

# Set Fish as the default shell
RUN chsh -s /usr/bin/fish

# Expose the GDB and OpenOCD ports for debugging
EXPOSE 3333 4444

# Tell Git to trust the /workspace directory
RUN git config --global --add safe.directory /workspace

# Default command: run a shell
CMD ["/usr/bin/fish"]
