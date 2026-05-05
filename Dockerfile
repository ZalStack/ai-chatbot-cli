FROM fedora:43

# Install dependencies
RUN dnf update -y && \
    dnf install -y \
    gcc-c++ \
    make \
    cmake \
    libcurl-devel \
    nlohmann-json-devel \
    mysql-devel \
    git \
    vim \
    && dnf clean all

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build the application
RUN make clean && make

# Command to run
CMD ["./build/chatbot"]