FROM cromo/devkitarm-3ds
MAINTAINER Cristian Romo "cristian.g.romo@gmail.com"

RUN apt-get update && \
  apt-get install -y \
    automake \
    libjpeg-dev \
    python3-pip \
    python3 && \
  rm -rf /var/lib/apt/lists/*
RUN pip-3.2 install Pillow

WORKDIR /opt/devkitPro
RUN git clone --depth=1 https://github.com/devkitPro/3dstools.git
WORKDIR /opt/devkitPro/3dstools
RUN ./autogen.sh && ./configure && make && \
    cp 3dsxtool $DEVKITARM/bin && \
    cp 3dsxdump $DEVKITARM/bin && \
    cp smdhtool $DEVKITARM/bin

WORKDIR /source
CMD ["bash"]
