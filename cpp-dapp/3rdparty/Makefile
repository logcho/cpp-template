.PHONY: clean

all: cpp-httplib picojson

cpp-httplib:
	wget https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.10.4.tar.gz && \
  tar xvf v0.10.4.tar.gz && \
  rm v0.10.4.tar.gz && \
  mv cpp-httplib-0.10.4 cpp-httplib

picojson:
	wget https://github.com/kazuho/picojson/archive/refs/tags/v1.3.0.tar.gz && \
  tar xvf v1.3.0.tar.gz && \
  rm v1.3.0.tar.gz && \
  mv picojson-1.3.0 picojson

boost:
  wget https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz
  tar xzf boost_1_87_0.tar.gz && \
  rm boost_1_87_0.tar.gzz && \
  mv boost_1_87_0 boost

clean:
	rm -rf picojson cpp-httplib boost
